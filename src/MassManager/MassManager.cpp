// wxMASSManager
// Copyright (C) 2020-2021 Guillaume Jacquemin
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Corrade/version.h>

#if !(CORRADE_VERSION_YEAR * 100 + CORRADE_VERSION_MONTH >= 202006)
    #error This application requires Corrade 2020.06 or later to build.
#endif

#include <algorithm>

#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/String.h>

#include "MassManager.h"

static const std::string empty_string = "";

const std::string MassManager::_stagingAreaDirectory =
    Utility::Directory::join(Utility::Directory::path(Utility::Directory::executableLocation()), "staging");

MassManager::MassManager(const std::string& save_path, const std::string& steam_id, bool demo) {
    _saveDirectory = save_path;
    _steamId = steam_id;
    _demo = demo;

    Containers::arrayReserve(_hangars, 32);

    std::string mass_filename = "";
    for(int i = 0; i < 32; i++) {
        mass_filename =
            Utility::Directory::join(_saveDirectory, Utility::formatString("{}Unit{:.2d}{}.sav", demo ? "Demo" : "", i, _steamId));
        Containers::arrayAppend(_hangars, Mass{mass_filename});
    }

    if(!Utility::Directory::exists(_stagingAreaDirectory)) {
        Utility::Directory::mkpath(_stagingAreaDirectory);
    }

    using Utility::Directory::Flag;
    std::vector<std::string> file_list =
        Utility::Directory::list(_stagingAreaDirectory, Flag::SkipSpecial|Flag::SkipDirectories|Flag::SkipDotAndDotDot);

    auto iter = std::remove_if(file_list.begin(), file_list.end(), [](std::string& file){
        return !Utility::String::endsWith(file, ".sav");
    });

    file_list.erase(iter, file_list.end());

    for(const std::string& file : file_list) {
        std::string name = Mass::getNameFromFile(Utility::Directory::join(_stagingAreaDirectory, file));

        if(name != "") {
            _stagedMasses[file] = name;
        }
    }
}

auto MassManager::saveDirectory() -> std::string const& {
    return _saveDirectory;
}

auto MassManager::stagingAreaDirectory() -> std::string const& {
    return _stagingAreaDirectory;
}

auto MassManager::lastError() -> std::string const& {
    return _lastError;
}

auto MassManager::massName(int hangar) -> std::string const& {
    if(hangar < 0 || hangar >= 32) {
        return empty_string;
    }

    return _hangars[hangar].name();
}

auto MassManager::massState(int hangar) -> MassState {
    if(hangar < 0 || hangar >= 32) {
        return MassState::Empty;
    }

    return _hangars[hangar].state();
}

void MassManager::refreshHangar(int hangar) {
    if(hangar < 0 || hangar >= 32) {
        return;
    }

    std::string mass_filename =
        Utility::Directory::join(_saveDirectory, Utility::formatString("{}Unit{:.2d}{}.sav", _demo ? "Demo" : "", hangar, _steamId));
    _hangars[hangar] = Mass{mass_filename};
}

auto MassManager::importMass(int staged_index, int hangar) -> bool {
    if(hangar < 0 || hangar >= 32) {
        _lastError = "Hangar out of range in MassManager::importMass()";
        return false;
    }

    int i = 0;
    for(const auto& mass_info : _stagedMasses) {
        if(i != staged_index) {
            ++i;
            continue;
        }

        std::string source = Utility::Directory::join(_stagingAreaDirectory, mass_info.first);

        Utility::Directory::copy(source, source + ".tmp");

        if(!Mass{source + ".tmp"}.updateSteamId(_steamId))
        {
            _lastError = "The M.A.S.S. file at " + source + " seems to be corrupt.";
            Utility::Directory::rm(source + ".tmp");
            return false;
        }

        if(Utility::Directory::exists(_hangars[hangar].filename())) {
            Utility::Directory::rm(_hangars[hangar].filename());
        }

        Utility::Directory::move(source + ".tmp", _hangars[hangar].filename());

        return true;
    }

    _lastError = "";
    return false;
}

auto MassManager::exportMass(int hangar) -> bool {
    if(hangar < 0 || hangar >= 32) {
        _lastError = "Hangar out of range in MassManager::exportMass()";
        return false;
    }

    if(_hangars[hangar].state() == MassState::Empty ||
       _hangars[hangar].state() == MassState::Invalid) {
        _lastError = Utility::formatString("There is no valid data to export in hangar {:.2d}", hangar);
        return false;
    }

    auto name = _hangars[hangar].name();

    std::string source = Utility::Directory::join(_saveDirectory, _hangars[hangar].filename());
    std::string dest = Utility::Directory::join(_stagingAreaDirectory, Utility::formatString("{}_{}.sav", _steamId, _hangars[hangar].name()));

    if(!Utility::Directory::copy(source, dest)) {
        _lastError = Utility::formatString("Couldn't export data from hangar {:.2d} to {}", hangar, dest);
        return false;
    }

    return true;
}

auto MassManager::moveMass(int source, int destination) -> bool {
    if(source < 0 || source >= 32) {
        _lastError = "Source hangar out of range.";
        return false;
    }

    if(destination < 0 || destination >= 32) {
        _lastError = "Destination hangar out of range.";
        return false;
    }

    std::string source_file = _hangars[source].filename();
    std::string dest_file = _hangars[destination].filename();
    MassState dest_state = _hangars[destination].state();

    switch(dest_state) {
        case MassState::Empty:
            break;
        case MassState::Invalid:
            Utility::Directory::rm(dest_file);
            break;
        case MassState::Valid:
            Utility::Directory::move(dest_file, dest_file + ".tmp");
            break;
    }

    Utility::Directory::move(source_file, dest_file);

    if(dest_state == MassState::Valid) {
        Utility::Directory::move(dest_file + ".tmp", source_file);
    }

    return true;
}

auto MassManager::deleteMass(int hangar) -> bool {
    if(hangar < 0 || hangar >= 32) {
        _lastError = "Hangar out of bounds";
        return false;
    }

    bool result = Utility::Directory::rm(_hangars[hangar].filename());
    if(!result) {
        _lastError = "Deletion failed. Maybe the file was already deleted, or it's locked by another application.";
    }
    return result;
}

auto MassManager::renameMass(int hangar, const std::string& new_name) -> bool {
    if(hangar < 0 || hangar >= 32) {
        _lastError = "Hangar number out of range.";
        return false;
    }

    if(new_name.length() > 32) {
        _lastError = "The new name is longer than 32 characters.";
        return false;
    }

    if(!_hangars[hangar].rename(new_name)) {
        _lastError = _hangars[hangar].lastError();
        return false;
    }

    return true;
}

auto MassManager::stagedMasses() -> std::map<std::string, std::string> const& {
    return _stagedMasses;
}

auto MassManager::stagedMassName(int index) -> std::string const& {
    int i = 0;
    for(const auto& mass_info : _stagedMasses) {
        if(i != index) {
            ++i;
            continue;
        }

        return mass_info.second;
    }

    return empty_string;
}

auto MassManager::updateStagedMass(const std::string& filename) -> int {
    std::string file = Utility::Directory::join(_stagingAreaDirectory, filename);

    if(!Utility::Directory::exists(file)) {
        return -1;
    }

    auto name = Mass::getNameFromFile(file);

    if(name == "") {
        return -1;
    }

    _stagedMasses[filename] = name;

    int index = 0;

    for(const auto& mass: _stagedMasses) {
        if(mass.first != filename) {
            ++index;
            continue;
        }

        return index;
    }

    return -1;
}

auto MassManager::removeStagedMass(const std::string& filename) -> int {
    int index = 0;

    for(auto it = _stagedMasses.begin(); it != _stagedMasses.end(); ++it, ++index) {
        if(it->first == filename) {
            _stagedMasses.erase(it);
            return index;
        }
    }

    return -1;
}

void MassManager::deleteStagedMass(int index) {
    int i = 0;
    for(auto it = _stagedMasses.begin(); it != _stagedMasses.end(); ++it, ++i) {
        if(i == index) {
            Utility::Directory::rm(Utility::Directory::join(_stagingAreaDirectory, it->first));
            break;
        }
    }
}
