#ifndef MASSBUILDERMANAGER_H
#define MASSBUILDERMANAGER_H

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

#include <cstdint>

#include <string>

enum class GameState : std::uint8_t {
    Unknown, NotRunning, Running
};

class MassBuilderManager {
    public:
        MassBuilderManager();

        auto ready() -> bool;
        auto lastError() -> std::string const&;

        auto saveDirectory() -> std::string const&;

        auto checkGameState() -> GameState;
        auto gameState() -> GameState;

    private:
        auto findSaveDirectory() -> bool;

        bool _ready = false;

        std::string _lastError = "";

        std::string _saveDirectory = "";

        GameState _gameState = GameState::Unknown;
};

#endif //MASSBUILDERMANAGER_H
