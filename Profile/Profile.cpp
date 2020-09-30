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

#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/String.h>

#include <wx/wfstream.h>
#include <wx/zipstrm.h>

#include "Profile.h"

constexpr char company_name_locator[] = "CompanyName\0\x0c\0\0\0StrProperty";
constexpr char active_slot_locator[] = "ActiveFrameSlot\0\x0c\0\0\0IntProperty";
constexpr char credits_locator[] = "Credit\0\x0c\0\0\0IntProperty";

using namespace Corrade;

Profile::Profile(const std::string& path) {
    auto map = Utility::Directory::mapRead(path);

    if(!map) {
        _lastError = "Couldn't memory-map " + Utility::Directory::filename(path);
        return;
    }

    _profileDirectory = Utility::Directory::path(path);
    _filename = Utility::Directory::filename(path);

    if(Utility::String::beginsWith(_filename, "Demo")) {
        _type = ProfileType::Demo;
    }
    else {
        _type = ProfileType::FullGame;
    }

    _steamId = Utility::String::ltrim(Utility::String::rtrim(_filename, ".sav"), (_type == ProfileType::Demo ? "Demo" : "") + std::string{"Profile"});

    auto it = std::search(map.begin(), map.end(), &company_name_locator[0], &company_name_locator[27]);

    if(it == map.end()) {
        _lastError = "Couldn't find a company name in " + _filename;
        return;
    }

    _companyName = std::string{it + 41};

    _valid = true;
}

auto Profile::valid() const -> bool {
    return _valid;
}

auto Profile::lastError() const -> std::string const& {
    return _lastError;
}

auto Profile::filename() const -> std::string const& {
    return _filename;
}

auto Profile::type() const -> ProfileType {
    return _type;
}

auto Profile::steamId() const -> std::string const& {
    return _steamId;
}

auto Profile::companyName() const -> std::string const& {
    return _companyName;
}

auto Profile::activeFrameSlot() const -> std::int8_t {
    return _activeFrameSlot;
}

auto Profile::getActiveFrameSlot() -> std::int8_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &active_slot_locator[0], &active_slot_locator[31]);

    if(iter == mmap.end()) {
        if(std::search(mmap.begin(), mmap.end(), &credits_locator[0], &credits_locator[22]) != mmap.end()) {
            _activeFrameSlot = 0;
        }
        else {
            _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
            _activeFrameSlot = -1;
        }
    }
    else {
        _activeFrameSlot = *(iter + 41);
    }

    return _activeFrameSlot;
}

auto Profile::credits() const -> std::int32_t {
    return _credits;
}

auto Profile::getCredits() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &credits_locator[0], &credits_locator[22]);

    if(iter != mmap.end()) {
        _credits = ((*(iter + 0x20)) << 0)  +
                   ((*(iter + 0x21)) << 8)  +
                   ((*(iter + 0x22)) << 16) +
                   ((*(iter + 0x23)) << 24);
    }
    else{
        _credits = -1;
    }

    return _credits;
}

auto Profile::backup(const std::string& filename) -> bool {
    if(filename.empty() || (filename.length() < 5 && !Utility::String::endsWith(filename, ".zip"))) {
        _lastError = "Invalid filename " + filename + " in Profile::backup()";
        return false;
    }

    if(Utility::Directory::exists(filename)) {
        if(!Utility::Directory::rm(filename)) {
            _lastError = "Couldn't overwrite " + filename + " in Profile::backup()";
        }
    }

    wxFFileOutputStream out{filename};
    wxZipOutputStream zip{out};

    {
        zip.PutNextEntry(_filename);
        wxFFileInputStream profile_stream{Utility::Directory::toNativeSeparators(Utility::Directory::join(_profileDirectory, _filename)), "rb"};
        zip.Write(profile_stream);
    }

    for(int i = 0; i < 32; ++i) {
        std::string unit_file = Utility::Directory::join(_profileDirectory, Utility::formatString("{}Unit{:.2d}{}.sav", _type == ProfileType::Demo ? "Demo" : "", i, _steamId));
        if(Utility::Directory::exists(unit_file)) {
            zip.PutNextEntry(Utility::Directory::filename(unit_file));
            wxFFileInputStream unit_stream{Utility::Directory::toNativeSeparators(unit_file)};
            zip.Write(unit_stream);
        }
    }

    return true;
}
