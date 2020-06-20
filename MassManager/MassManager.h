#ifndef MASSMANAGER_H
#define MASSMANAGER_H

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

#include <map>
#include <string>
#include <vector>

#include <Corrade/Containers/StaticArray.h>
#include <Corrade/Containers/Optional.h>

#include <wx/datetime.h>
#include <wx/image.h>

using namespace Corrade;

enum class GameState : uint8_t {
    Unknown, NotRunning, Running
};

enum class HangarState : uint8_t {
    Empty, Invalid, Filled
};

enum class SortType : uint8_t {
    Filename, CreationDate
};

enum class SortOrder: uint8_t {
    Ascending, Descending
};

struct Screenshot {
    std::string _filename;
    wxDateTime  _creationDate;
    wxImage     _thumbnail;
};

class MassManager {
    public:
        MassManager();

        auto ready() -> bool;
        auto lastError() -> std::string const&;

        auto saveDirectory() -> std::string const&;
        auto stagingAreaDirectory() -> std::string const&;
        auto screenshotDirectory() -> std::string const&;

        auto steamId() -> std::string const&;
        auto profileSaveName() -> std::string const&;

        auto checkGameState() -> GameState;
        auto gameState() -> GameState;

        auto getActiveSlot() -> char;
        auto activeSlot() -> char;

        auto importMass(const std::string& source, int hangar) -> bool;
        auto importMass(int staged_index, int hangar) -> bool;
        auto exportMass(int hangar) -> bool;

        auto moveMass(int source, int destination) -> bool;
        auto deleteMass(int hangar) -> bool;
        auto renameMass(int hangar, const std::string& new_name) -> bool;

        auto backupSaves(const std::string& filename) -> bool;

        void refreshHangar(int hangar);
        auto hangarState(int hangar) -> HangarState;
        auto massName(int hangar) -> Containers::Optional<std::string>;

        auto getMassName(const std::string& filename) -> Containers::Optional<std::string>;

        auto initialiseStagingArea() -> std::vector<std::string>;
        auto updateStagedMass(const std::string& filename) -> int;
        auto removeStagedMass(const std::string& filename) -> int;
        void deleteStagedMass(int index);
        auto stagedMassName(int index) -> std::string;
        auto stagedMassName(const std::string& filename) -> std::string;

        void loadScreenshots();
        auto screenshots() -> std::vector<Screenshot> const&;
        void sortScreenshots(SortType type);
        void sortScreenshots(SortOrder order);
        void sortScreenshots();
        auto updateScreenshot(const std::string& filename) -> int;
        void removeScreenshot(int index);
        void deleteScreenshot(int index);

    private:
        auto findSaveDirectory() -> bool;
        auto findSteamId() -> bool;

        auto findScreenshotDirectory() -> bool;
        void addScreenshot(const std::string& filename);

        bool _ready = false;

        std::string _lastError = "";

        std::string _stagingAreaDirectory = "";
        std::string _saveDirectory = "";
        std::string _screenshotDirectory = "";
        std::string _steamId = "";
        std::string _profileSaveName = "";

        GameState _gameState = GameState::Unknown;

        char _activeSlot = -1;

        struct Hangar {
            HangarState _state = HangarState::Empty;
            Containers::Optional<std::string> _massName = Containers::NullOpt;
            std::string _filename = "";
        };

        Containers::StaticArray<32, Hangar> _hangars{Containers::ValueInit};

        std::map<std::string, std::string> _stagedMasses;

        std::vector<Screenshot> _screenshots;
        SortType  _sortType  = SortType::Filename;
        SortOrder _sortOrder = SortOrder::Ascending;
};

#endif //MASSMANAGER_H
