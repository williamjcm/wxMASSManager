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

#include <cstring>

#include <algorithm>

#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Directory.h>

#include "Mass.h"

using namespace Corrade;

constexpr char mass_name_locator[] = "Name_45_A037C5D54E53456407BDF091344529BB\0\x0c\0\0\0StrProperty";
constexpr char steamid_locator[] = "Account\0\x0c\0\0\0StrProperty";

std::string Mass::_lastError = "";

Mass::Mass(const std::string& filename) {
    _filename = filename;

    if(!Utility::Directory::exists(_filename)) {
        _lastError = "The file " + _filename + " couldn't be found.";
        return;
    }

    auto mmap = Utility::Directory::mapRead(_filename);

    auto iter = std::search(mmap.begin(), mmap.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mmap.end()) {
        _name = std::string{iter + 70};
        _state = MassState::Valid;
    }
    else {
        _lastError = "The name couldn't be found in " + filename;
        _state = MassState::Invalid;
    }
}

auto Mass::lastError() -> std::string const& {
    return _lastError;
}

auto Mass::getNameFromFile(const std::string& filename) -> std::string {
    if(!Utility::Directory::exists(filename)) {
        _lastError = "The file " + filename + " couldn't be found.";
        return "";
    }

    std::string name = "";

    auto mmap = Utility::Directory::mapRead(filename);

    auto iter = std::search(mmap.begin(), mmap.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mmap.end()) {
        name = std::string{iter + 70};
    }
    else {
        _lastError = "The name couldn't be found in " + filename;
    }

    return name;
}

auto Mass::filename() -> std::string const&{
    return _filename;
}

auto Mass::name() -> std::string const&{
    return _name;
}

auto Mass::getName() -> std::string const& {
    if(!Utility::Directory::exists(_filename)) {
        _lastError = "The file " + _filename + " couldn't be found.";
        _state = MassState::Empty;
        return _name = "";
    }

    auto mmap = Utility::Directory::mapRead(_filename);

    auto iter = std::search(mmap.begin(), mmap.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mmap.end()) {
        _state = MassState::Valid;
        return _name = std::string{iter + 70};
    }
    else {
        _lastError = "The name couldn't be found in " + _filename;
        _state = MassState::Invalid;
        return _name = "";
    }
}

auto Mass::state() -> MassState {
    return _state;
}

auto Mass::updateSteamId(const std::string& steam_id) -> bool {
    if(!Utility::Directory::exists(_filename)) {
        _lastError = "The file " + _filename + " couldn't be found.";
        _state = MassState::Empty;
        return false;
    }

    Utility::Directory::copy(_filename, _filename + ".tmp");

    {
        auto mmap = Utility::Directory::map(_filename + ".tmp");

        auto iter = std::search(mmap.begin(), mmap.end(), &steamid_locator[0], &steamid_locator[23]);

        if(iter == mmap.end()) {
            _lastError = "The M.A.S.S. file at " + _filename + " seems to be corrupt.";
            Utility::Directory::rm(_filename + ".tmp");
            return false;
        }

        iter += 37;

        if(std::strncmp(iter, steam_id.c_str(), steam_id.length()) != 0) {
            for(int i = 0; i < 17; ++i) {
                *(iter + i) = steam_id[i];
            }
        }
    }

    if(Utility::Directory::exists(_filename)) {
        Utility::Directory::rm(_filename);
    }

    Utility::Directory::move(_filename + ".tmp", _filename);

    return true;
}

auto Mass::rename(const std::string& new_name) -> bool {
    char length_difference = static_cast<char>(_name.length() - new_name.length());

    std::string mass_data = Utility::Directory::readString(_filename);

    auto iter = std::search(mass_data.begin(), mass_data.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mass_data.end()) {
        *(iter - 45) = *(iter - 45) - length_difference;
        *(iter + 57) = *(iter + 57) - length_difference;
        *(iter + 66) = *(iter + 66) - length_difference;
        while(*(iter + 70) != '\0') {
            mass_data.erase(iter + 70);
        }
        mass_data.insert(iter + 70, new_name.cbegin(), new_name.cend());

        if(!Utility::Directory::writeString(_filename, mass_data)) {
            _lastError = "The file" + _filename + " couldn't be written to.";
            return false;
        }

        return true;
    }
    else {
        _lastError = "Couldn't find the M.A.S.S. name in " + _filename;

        return false;
    }
}
