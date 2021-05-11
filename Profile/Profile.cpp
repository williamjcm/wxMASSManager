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

#include "Locators.h"

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

auto Profile::getCompanyName() -> std::string const& {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto it = std::search(mmap.begin(), mmap.end(), &company_name_locator[0], &company_name_locator[27]);

    if(it == mmap.end()) {
        _lastError = "Couldn't find a company name in " + _filename;
        _companyName = "";
    }
    else {
        _companyName = std::string{it + 41};
    }

    return _companyName;
}

auto Profile::renameCompany(const std::string& new_name) -> bool {
    char length_difference = static_cast<char>(_companyName.length() - new_name.length());

    std::string profile_data = Utility::Directory::readString(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(profile_data.begin(), profile_data.end(), &company_name_locator[0], &company_name_locator[27]);

    if(iter != profile_data.end()) {

        *(iter + 0x1C) = *(iter + 0x1C) - length_difference;
        *(iter + 0x25) = *(iter + 0x25) - length_difference;

        while(*(iter + 0x29) != '\0') {
            profile_data.erase(iter + 0x29);
        }

        profile_data.insert(iter + 0x29, new_name.cbegin(), new_name.cend());

        if(!Utility::Directory::writeString(Utility::Directory::join(_profileDirectory, _filename), profile_data)) {
            _lastError = "The file" + _filename + " couldn't be written to.";
            return false;
        }

        _companyName = new_name;

        return true;
    }
    else {
        _lastError = "Couldn't find the company name in " + _filename;

        return false;
    }
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
        _credits = *reinterpret_cast<const std::int32_t*>(iter + 0x20);
    }
    else{
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _credits = -1;
    }

    return _credits;
}

auto Profile::storyProgress() const -> std::int32_t {
    return _storyProgress;
}

auto Profile::getStoryProgress() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &story_progress_locator[0], &story_progress_locator[29]);

    if(iter != mmap.end()) {
        _storyProgress = *reinterpret_cast<const std::int32_t*>(iter + 0x27);
    }
    else{
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _storyProgress = -1;
    }

    return _storyProgress;
}

auto Profile::setStoryProgress(std::int32_t progress) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &story_progress_locator[0], &story_progress_locator[29]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x27) = progress;
        return true;
    }
    else{
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::lastMissionId() const -> std::int32_t {
    return _lastMissionId;
}

auto Profile::getLastMissionId() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &last_mission_id_locator[0], &last_mission_id_locator[29]);

    if(iter != mmap.end()) {
        _lastMissionId = *reinterpret_cast<const std::int32_t*>(iter + 0x27);
    }
    else{
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _lastMissionId = -1;
    }

    return _lastMissionId;
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
