#ifndef MASS_H
#define MASS_H

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

enum class MassState : std::uint8_t {
    Empty, Invalid, Valid
};

class Mass {
    public:
        Mass(const std::string& filename);

        Mass(const Mass&) = delete;
        Mass& operator=(const Mass&) = delete;

        Mass(Mass&&) = default;
        Mass& operator=(Mass&&) = default;

        static auto lastError() -> std::string const&;

        static auto getNameFromFile(const std::string& filename) -> std::string;

        auto filename() -> std::string const&;

        auto name() -> std::string const&;
        auto getName() -> std::string const&;

        auto state() -> MassState;

        auto updateSteamId(const std::string& steam_id) -> bool;

        auto rename(const std::string& new_name) -> bool;

    private:
        static std::string _lastError;

        std::string _filename = "";
        std::string _name = "";
        MassState _state = MassState::Empty;
};

#endif // MASS_H
