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

#include <Corrade/version.h>

#if !(CORRADE_VERSION_YEAR * 100 + CORRADE_VERSION_MONTH >= 202006)
    #error This application requires Corrade 2020.06 or later to build.
#endif

#include <algorithm>
#include <regex>

#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/String.h>

#include "ProfileManager.h"

using namespace Corrade;

ProfileManager::ProfileManager(const std::string& base_path) {
    _profileDirectory = Utility::Directory::join(base_path, "Saved/SaveGames");

    if(Utility::Directory::exists(_profileDirectory) == false) {
        _lastError = "Couldn't find the profile directory. Make sure you played enough of the game.";
        return;
    }

    using Utility::Directory::Flag;
    std::vector<std::string> files = Utility::Directory::list(_profileDirectory, Flag::SkipSpecial|Flag::SkipDirectories|Flag::SkipDotAndDotDot);

    auto predicate = [](const std::string& file)->bool{
        std::regex regex("(Demo)?Profile[0-9]{17}\\.sav", std::regex::nosubs);
        std::cmatch m;
        return !std::regex_match(file.c_str(), m, regex);
    };

    files.erase(std::remove_if(files.begin(), files.end(), predicate), files.end());

    for(const std::string& file : files) {
        Profile profile{Utility::Directory::join(_profileDirectory, file)};

        if(profile.valid() == false) {
            Utility::Warning{} << "Profile" << file.c_str() << "is invalid:" << profile.lastError().c_str();
            continue;
        }

        _profiles.push_back(std::move(profile));
    }

    if(_profiles.size() == 0) {
        _lastError = "No profiles were found.";
        return;
    }

    _ready = true;
}

auto ProfileManager::ready() -> bool {
    return _ready;
}

auto ProfileManager::lastError() -> std::string const& {
    return _lastError;
}

auto ProfileManager::profileDirectory() -> std::string const& {
    return _profileDirectory;
}

auto ProfileManager::profiles() -> std::vector<Profile> const& {
    return _profiles;
}

auto ProfileManager::currentProfile() -> Profile* {
    return _currentProfile;
}

auto ProfileManager::setProfile(std::size_t index) -> bool {
    bool result = false;

    try {
        _currentProfile = &(_profiles.at(index));
        result = true;
    }
    catch(const std::out_of_range&) {
        _lastError = "Invalid profile index";
        _currentProfile = nullptr;
    }

    return result;
}


