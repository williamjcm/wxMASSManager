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
    {25600, "Mission 1 - Training"},
    {25856, "Mission 2 - Patrol Operation"},
    {26112, "Mission 3 - Fusion Cells in the Snow"},
    {26368, "Mission 4 - Earning Changes"},
    {26624, "Mission 5 - Unexpected Coordination"},
    {26880, "Mission 6 - Empowering Void"},
    {27136, "Mission 7 - Logisitics Obstacles"},
    {27392, "Mission 8 - Wrath of the Wastelands"},
    {27648, "Mission 9 - Suspicious Originator"},
    {27904, "Mission 10 - Researchers Data Recovery"},
    {28160, "Mission 11 - Tempestuous Sector"},
    {28416, "Mission 12 - Clashes of Metal"},
    {28672, "Mission 13 - The Sandstorm Glutton"},

    // Hunting grounds
    {51200, "Hunt 1 - Desert Pathway Safety"},
    {51456, "Hunt 2 - Snowfield Custodian"},
    {51712, "Hunt 3 - Abandoned Valley Raid"},
    {51968, "Hunt 4 - Depths of the Machineries"},

    // Challenges
    {76800,  "Challenge 1 - Redline Battlefront"},
    {81920,  "Challenge 2 - Void Convergence"},
    {102400, "Challenge 3 - Gates of Ascension"}
}};
