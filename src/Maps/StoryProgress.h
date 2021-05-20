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

#pragma once

#include <cstdint>

#include <map>

static const std::map<std::int32_t, const char*> story_progress_map {{
    {0x00, "Chapter 1 start - Company isn't named yet"},
    {0x64, "Chapter 1 - First time in the hangar"},
    {0x65, "Chapter 1 - After 1st meeting with Quin in mission section"},

    {0x66, "Chapter 1 - After training, talking with Reina and Quin in hangar"},
    {0x67, "Chapter 1 - After training, returned to hangar"},
    {0x68, "Chapter 1 - After training, talked with Quin in development section"},
    {0x69, "Chapter 1 - After training, talked with Waltz in armour section"},
    {0xC8, "Chapter 1 - After training, talked with Kael in tuning section"},
    {0xC9, "Chapter 1 - After training, got mission 2 briefing"},

    {0x12C,  "Chapter 1 - After mission 2, talking with Reina"},
    {0x12D,  "Chapter 1 - After mission 2, returned to hangar"},
    {0x12E,  "Chapter 1 - After mission 2, talked with Kael in tuning section"},
    {0x12F,  "Chapter 1 - After mission 2, talked with Reina in hangar"},
    {0x130,  "Chapter 1 - After mission 2, got mission 3 briefing"},

    {0x190, "Chapter 1 - After mission 3, talking with Reina"},
    {0x191, "Chapter 1 - After mission 3, returned to hangar"},
    {0x192, "Chapter 1 - After mission 3, talked with Waltz in armour section"},
    {0x193, "Chapter 1 - After mission 3, got mission 4 briefing"},

    {0x1F4, "Chapter 1 - After mission 4, talking with Reina"},
    {0x1F5, "Chapter 1 - After mission 4, returned to hangar"},
    {0x1F6, "Chapter 1 - After mission 4, talked with Waltz in armour section"},
    {0x1F7, "Chapter 1 - After mission 4, talked with Reina in hangar"},
    {0x1F8, "Chapter 1 - After mission 4, got mission 5 and hunt 1 briefing"},

    {0x258, "Chapter 1 - After mission 5, meeting Neon and Aine"},
    {0x259, "Chapter 1 - After mission 5, returned to hangar"},
    {0x25A, "Chapter 1 - After mission 5, got mission 6 briefing"},

    {0x2BC, "Chapter 1 - After mission 6, talking with Reina"},
    {0x2BD, "Chapter 1 - After mission 6, returned to hangar"},
    {0x2BE, "Chapter 1 - After mission 6, got hunt 2 briefing"},
    {0x2BF, "Chapter 1 - After mission 6, met Ellenier"},
    {0x2C0, "Chapter 1 - After mission 6, got mission 7 briefing"},

    {0x320, "Chapter 1 - After mission 7, talking with Nier"},
    {0x321, "Chapter 1 - After mission 7, returned to hangar"},
    {0x322, "Chapter 1 - After mission 7, talked with Quin, Reina, and Nier in development section"},
    {0x323, "Chapter 1 - After mission 7, got mission 8 briefing"},

    {0x384, "Chapter 1 - After mission 8, talking with crew in hangar"},
    {0x385, "Chapter 1 - After mission 8, returned to hangar"},
    {0x386, "Chapter 1 - After mission 8, got hunt 3 briefing"},
    {0x387, "Chapter 1 - After mission 8, talked with Reina, Nier, and Quin in development section"},

    {0x388, "Chapter 2 start"},
    {0x389, "Chapter 2 - Got mission 9 briefing"},

    {0x3E8, "Chapter 2 - After mission 9, talking with Reina in hangar"},
    {0x3E9, "Chapter 2 - After mission 9, returned to hangar"},
    {0x3EA, "Chapter 2 - After mission 9, talked with crew in armour section"},
    {0x3EB, "Chapter 2 - After mission 9, got mission 10 briefing"},

    {0x44C, "Chapter 2 - After mission 10, talking with Reina in hangar"},
    {0x44D, "Chapter 2 - After mission 10, returned to hangar"},
    {0x44E, "Chapter 2 - After mission 10, got mission 11 briefing"},

    {0x4B0, "Chapter 2 - After mission 11, talking with Reina and Nier in hangar"},
    {0x4B1, "Chapter 2 - After mission 11, returned to hangar"},
    {0x4B2, "Chapter 2 - After mission 11, got mission 12 briefing"},

    {0x514, "Chapter 2 - After mission 12, talking with Reina and Waltz in hangar"},
    {0x515, "Chapter 2 - After mission 12, returned to hangar"},
    {0x516, "Chapter 2 - After mission 12, got hunt 4 and mission 13 briefing"},

    {0x578, "Chapter 2 - After mission 13, talking with Reina in hangar"},
    {0x579, "Chapter 2 - After mission 13, returned to hangar"},
    {0x57A, "Chapter 2 - After mission 13, talked with Reina in development section"},
    {0x57B, "Chapter 2 - After mission 13, got briefing for challenges 1, 2, and 3"},
}};
