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

#include <Corrade/Containers/GrowableArray.h>

#include "../Mass/Mass.h"

using namespace Corrade;

class MassManager {
    public:
        MassManager(const std::string& save_path, const std::string& steam_id, bool demo);

        auto saveDirectory() -> std::string const&;
        auto stagingAreaDirectory() -> std::string const&;

        auto lastError() -> std::string const&;

        auto massName(int hangar) -> std::string const&;
        auto massState(int hangar) -> MassState;

        void refreshHangar(int hangar);

        auto importMass(int staged_index, int hangar) -> bool;
        auto exportMass(int hangar) -> bool;

        auto moveMass(int source, int destination) -> bool;
        auto deleteMass(int hangar) -> bool;
        auto renameMass(int hangar, const std::string& new_name) -> bool;

        auto stagedMasses() -> std::map<std::string, std::string> const&;

        auto stagedMassName(int index) -> std::string const&;

        auto updateStagedMass(const std::string& filename) -> int;
        auto removeStagedMass(const std::string& filename) -> int;
        void deleteStagedMass(int index);

    private:
        std::string _saveDirectory;
        std::string _steamId;
        bool _demo;

        std::string _lastError = "";

        Containers::Array<Mass> _hangars;

        static const std::string _stagingAreaDirectory;

        std::map<std::string, std::string> _stagedMasses;
};

#endif //MASSMANAGER_H
