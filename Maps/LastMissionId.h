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

#pragma once

#include <cstdint>

#include <map>

static const std::map<std::int32_t, const char*> mission_id_map {{
    // Story missions
    {0x64, "Mission 1 - Training"},
    {0x65, "Mission 2 - Patrol Operation"},
    {0x66, "Mission 3 - Fusion Cells in the Snow"},
    {0x67, "Mission 4 - Earning Changes"},
    {0x68, "Mission 5 - Unexpected Coordination"},
    {0x69, "Mission 6 - Empowering Void"},
    {0x6A, "Mission 7 - Logisitics Obstacles"},
    {0x6B, "Mission 8 - Wrath of the Wastelands"},
    {0x6C, "Mission 9 - Suspicious Originator"},
    {0x6D, "Mission 10 - Researchers Data Recovery"},
    {0x6E, "Mission 11 - Tempestuous Sector"},
    {0x6F, "Mission 12 - Clashes of Metal"},
    {0x70, "Mission 13 - The Sandstorm Glutton"},

    // Hunting grounds
    {0xC8, "Hunt 1 - Desert Pathway Safety"},
    {0xC9, "Hunt 2 - Snowfield Custodian"},
    {0xCA, "Hunt 3 - Abandoned Valley Raid"},
    {0xCB, "Hunt 4 - Depths of the Machineries"},

    // Challenges
    {0x12C, "Challenge 1 - Redline Battlefront"},
    {0x140, "Challenge 2 - Void Convergence"},
    {0x190, "Challenge 3 - Gates of Ascension"}
}};
