// wxMASSManager
// Copyright (C) 2020 Guillaume Jacquemin
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

#include <cstring>

#include <algorithm>
#include <vector>

#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>

#include <shlobj.h>
#include <wtsapi32.h>

#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/String.h>
#include <Corrade/Utility/Unicode.h>

#include "MassManager.h"

constexpr unsigned char mass_name_locator[] = { 'N', 'a', 'm', 'e', '_', '4', '5', '_', 'A', '0', '3', '7', 'C', '5', 'D', '5', '4', 'E', '5', '3', '4', '5', '6', '4', '0', '7', 'B', 'D', 'F', '0', '9', '1', '3', '4', '4', '5', '2', '9', 'B', 'B', '\0', 0x0C, '\0', '\0', '\0', 'S', 't', 'r', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

constexpr unsigned char steamid_locator[] = { 'A', 'c', 'c', 'o', 'u', 'n', 't', '\0', 0x0C, '\0', '\0', '\0', 'S', 't', 'r', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

constexpr unsigned char active_slot_locator[] = { 'A', 'c', 't', 'i', 'v', 'e', 'F', 'r', 'a', 'm', 'e', 'S', 'l', 'o', 't', '\0', 0x0C, '\0', '\0', '\0', 'I', 'n', 't', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

MassManager::MassManager() {
    _ready = findSaveDirectory() && findSteamId();

    if(!_ready) {
        return;
    }

    _profileSaveName = Utility::formatString("Profile{}.sav", _steamId);

    for(int i = 0; i < 32; ++i) {
        _hangars[i]._filename = Utility::formatString("Unit{:.2d}{}.sav", i, _steamId);
        refreshHangar(i);
    }
}

auto MassManager::ready() -> bool {
    return _ready;
}

auto MassManager::lastError() -> std::string const& {
    return _lastError;
}

auto MassManager::saveDirectory() -> std::string const& {
    return _saveDirectory;
}

auto MassManager::steamId() -> std::string const& {
    return _steamId;
}

auto MassManager::profileSaveName() -> std::string const& {
    return _profileSaveName;
}

auto MassManager::checkGameState() -> GameState {
    WTS_PROCESS_INFOW* process_infos = nullptr;
    unsigned long process_count = 0;

    if(WTSEnumerateProcessesW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &process_infos, &process_count)) {
        for(unsigned long i = 0; i < process_count; ++i) {
            if(std::wcscmp(process_infos[i].pProcessName, L"MASS_Builder-Win64-Shipping.exe") == 0) {
                _gameState = GameState::Running;
                break;
            }
            else {
                _gameState = GameState::NotRunning;
            }
        }
    }
    else {
        _gameState = GameState::Unknown;
    }

    if(process_infos != nullptr) {
        WTSFreeMemory(process_infos);
        process_infos = nullptr;
    }

    return _gameState;
}

auto MassManager::gameState() -> GameState {
    return _gameState;
}

auto MassManager::getActiveSlot() -> char{
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_saveDirectory, _profileSaveName));

    auto iter = std::search(mmap.begin(), mmap.end(), &active_slot_locator[0], &active_slot_locator[31]);

    if(iter == mmap.end()) {
        if(std::strncmp(&mmap[0x3F6], "Credit", 6) == 0) {
            _activeSlot = 0;
        }
        else {
            _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
            _activeSlot = -1;
        }
    }
    else {
        _activeSlot = *(iter + 41);
    }

    return _activeSlot;
}

auto MassManager::activeSlot() -> char {
    return _activeSlot;
}

auto MassManager::importMass(const std::string& source, int hangar) -> bool {
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar out of range in MassManager::importMass()";
        return false;
    }

    Utility::Directory::copy(source, source + ".tmp");

    {
        auto mmap = Utility::Directory::map(source + ".tmp");

        auto iter = std::search(mmap.begin(), mmap.end(), &steamid_locator[0], &steamid_locator[23]);

        if(iter == mmap.end()) {
            _lastError = "The M.A.S.S. file at " + source + " seems to be corrupt.";
            Utility::Directory::rm(source + ".tmp");
            return false;
        }

        iter += 37;

        if(std::strncmp(iter, _steamId.c_str(), _steamId.length()) != 0) {
            for(int i = 0; i < 17; ++i) {
                *(iter + i) = _steamId[i];
            }
        }
    }

    const std::string dest = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

    if(Utility::Directory::exists(dest)) {
        Utility::Directory::rm(dest);
    }

    Utility::Directory::move(source + ".tmp", dest);

    return true;
}

auto MassManager::moveMass(int source, int destination) -> bool {
    if(source < 0 && source >= 32) {
        _lastError = "Source hangar out of range in MassManager::moveMass()";
        return false;
    }

    if(destination < 0 && destination >= 32) {
        _lastError = "Destination hangar out of range in MassManager::moveMass()";
        return false;
    }

    std::string source_file = Utility::Directory::join(_saveDirectory, _hangars[source]._filename);
    std::string dest_file = Utility::Directory::join(_saveDirectory, _hangars[destination]._filename);
    HangarState dest_state = _hangars[destination]._state;

    switch(dest_state) {
        case HangarState::Empty:
            break;
        case HangarState::Invalid:
            Utility::Directory::rm(dest_file);
            break;
        case HangarState::Filled:
            Utility::Directory::move(dest_file, dest_file + ".tmp");
            break;
    }

    Utility::Directory::move(source_file, dest_file);

    if(dest_state == HangarState::Filled) {
        Utility::Directory::move(dest_file + ".tmp", source_file);
    }

    return true;
}

auto MassManager::deleteMass(int hangar) -> bool {
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar number out of range in MassManager::deleteMass()";
        return false;
    }

    std::string file = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

    if(Utility::Directory::exists(file)) {
        if(!Utility::Directory::rm(file)) {
            _lastError = "The M.A.S.S. file couldn't be deleted.";
            return false;
        }
    }

    return true;
}

auto MassManager::backupSaves(const std::string& filename) -> bool {
    if(filename.empty() || (filename.length() < 5 && !Utility::String::endsWith(filename, ".zip"))) {
        _lastError = "Invalid filename " + filename + " in MassManager::backupSaves()";
        return false;
    }

    if(Utility::Directory::exists(filename)) {
        if(!Utility::Directory::rm(filename)) {
            _lastError = "Couldn't overwrite " + filename + " in MassManager::backupSaves()";
        }
    }

    wxFFileOutputStream out{filename};
    wxZipOutputStream zip{out};

    {
        zip.PutNextEntry(_profileSaveName);
        wxFFileInputStream profile_stream{Utility::Directory::toNativeSeparators(Utility::Directory::join(_saveDirectory, _profileSaveName)), "rb"};
        zip.Write(profile_stream);
    }

    for(int i = 0; i < 32; ++i) {
        std::string unit_file = Utility::Directory::join(_saveDirectory, _hangars[i]._filename);
        if(Utility::Directory::exists(unit_file)) {
            zip.PutNextEntry(_hangars[i]._filename);
            wxFFileInputStream unit_stream{Utility::Directory::toNativeSeparators(unit_file)};
            zip.Write(unit_stream);
        }
    }

    return true;
}

void MassManager::refreshHangar(int hangar) {
    if(hangar < 0 && hangar >= 32) {
        return;
    }

    std::string unit_file = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

    if(!Utility::Directory::exists(unit_file)) {
        _hangars[hangar]._state = HangarState::Empty;
        _hangars[hangar]._massName = Containers::NullOpt;
    }
    else {
        Containers::Optional<std::string> name = getMassName(unit_file);

        _hangars[hangar]._state = name ? HangarState::Filled : HangarState::Invalid;
        _hangars[hangar]._massName = name;
    }
}

auto MassManager::hangarState(int hangar) -> HangarState {
    if(hangar < 0 && hangar >= 32) {
        return HangarState::Empty;
    }

    return _hangars[hangar]._state;
}

auto MassManager::massName(int hangar) -> Containers::Optional<std::string> {
    if(hangar < 0 && hangar >= 32) {
        return Containers::NullOpt;
    }

    return _hangars[hangar]._massName;
}

auto MassManager::getMassName(const std::string& filename) -> Containers::Optional<std::string> {
    Containers::Optional<std::string> name = Containers::NullOpt;

    auto mmap = Utility::Directory::mapRead(filename);

    auto iter = std::search(mmap.begin(), mmap.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mmap.end()) {
        name = std::string{iter + 70};
    }
    else {
        _lastError = "Couldn't find the M.A.S.S. name in " + filename;
    }

    return name;
}

auto MassManager::findSaveDirectory() -> bool {
    wchar_t h[MAX_PATH];
    if(!SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, h))) {
        _lastError = "SHGetFolderPathW() failed in MassManager::findSaveDirectory()";
        return false;
    }

    _saveDirectory = Utility::Directory::join(Utility::Directory::fromNativeSeparators(Utility::Unicode::narrow(h)), "MASS_Builder/Saved/SaveGames");

    if(!Utility::Directory::exists(_saveDirectory)) {
        _lastError = _saveDirectory + " wasn't found.";
        return false;
    }

    return true;
}

auto MassManager::findSteamId() -> bool {
    std::vector<std::string> listing = Utility::Directory::list(_saveDirectory);

    wxRegEx regex;
    if(!regex.Compile("Profile([0-9]{17}).sav", wxRE_ADVANCED)) {
        _lastError = "Couldn't compile the regex in MassManager::findSteamId()";
        return false;
    }

    for(const std::string& s : listing) {
        if(regex.Matches(s)) {
            _steamId = regex.GetMatch(s, 1);
            return true;
        }
    }

    _lastError = "Couldn't find the profile save.";
    return false;
}
