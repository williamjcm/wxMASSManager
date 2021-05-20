#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

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

#include <string>
#include <vector>

#include "../Profile/Profile.h"

using namespace Corrade;

class ProfileManager {
    public:
        ProfileManager(const std::string& base_path);

        auto ready() -> bool;
        auto lastError() -> std::string const&;

        auto profileDirectory() -> std::string const&;

        auto profiles() -> std::vector<Profile> const&;

        auto currentProfile() -> Profile*;
        auto setProfile(std::size_t index) -> bool;

    private:
        bool _ready = false;
        std::string _lastError = "";

        std::string _profileDirectory;

        std::vector<Profile> _profiles;

        Profile* _currentProfile = nullptr;
};

#endif //PROFILEMANAGER_H
