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

static const std::map<std::int32_t, const char*> story_progress_map {{
    {25600,  "Chapter 1 start"},
    {25856,  "Chapter 1 - After 1st meeting with Quin in mission section"},

    {26112,  "Chapter 1 - After training, talking with Reina and Quin in hangar"},
    {26368,  "Chapter 1 - After training, returned to hangar"},
    {26624,  "Chapter 1 - After training, talked with Quin in development section"},
    {26880,  "Chapter 1 - After training, talked with Waltz in armour section"},
    {51200,  "Chapter 1 - After training, talked with Kael in tuning section"},
    {51456,  "Chapter 1 - After training, got mission 2 briefing"},

    {76800,  "Chapter 1 - After mission 2, talking with Reina"},
    {77056,  "Chapter 1 - After mission 2, returned to hangar"},
    {77312,  "Chapter 1 - After mission 2, talked with Kael in tuning section"},
    {77568,  "Chapter 1 - After mission 2, talked with Reina in hangar"},
    {77824,  "Chapter 1 - After mission 2, got mission 3 briefing"},

    {102400, "Chapter 1 - After mission 3, talking with Reina"},
    {102656, "Chapter 1 - After mission 3, returned to hangar"},
    {102912, "Chapter 1 - After mission 3, talked with Waltz in armour section"},
    {103168, "Chapter 1 - After mission 3, got mission 4 briefing"},

    {128000, "Chapter 1 - After mission 4, talking with Reina"},
    {128256, "Chapter 1 - After mission 4, returned to hangar"},
    {128512, "Chapter 1 - After mission 4, talked with Waltz in armour section"},
    {128768, "Chapter 1 - After mission 4, talked with Reina in hangar"},
    {129024, "Chapter 1 - After mission 4, got mission 5 and hunt 1 briefing"},

    {153600, "Chapter 1 - After mission 5, meeting Neon and Aine"},
    {153856, "Chapter 1 - After mission 5, returned to hangar"},
    {154112, "Chapter 1 - After mission 5, got mission 6 briefing"},

    {179200, "Chapter 1 - After mission 6, talking with Reina"},
    {179456, "Chapter 1 - After mission 6, returned to hangar"},
    {179712, "Chapter 1 - After mission 6, got hunt 2 briefing"},
    {179968, "Chapter 1 - After mission 6, met Ellenier"},
    {180224, "Chapter 1 - After mission 6, got mission 7 briefing"},

    {204800, "Chapter 1 - After mission 7, talking with Nier"},
    {205056, "Chapter 1 - After mission 7, returned to hangar"},
    {205312, "Chapter 1 - After mission 7, talked with Quin, Reina, and Nier in development section"},
    {205568, "Chapter 1 - After mission 7, got mission 8 briefing"},

    {230400, "Chapter 1 - After mission 8, talking with crew in hangar"},
    {230656, "Chapter 1 - After mission 8, returned to hangar"},
    {230912, "Chapter 1 - After mission 8, got hunt 3 briefing"},
    {231168, "Chapter 1 - After mission 8, talked with Reina, Nier, and Quin in development section"},

    {231424, "Chapter 2 start"},
    {231680, "Chapter 2 - got mission 9 briefing"},
}};
