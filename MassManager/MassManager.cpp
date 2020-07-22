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

#include <cstring>

#include <algorithm>

#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>

#include <shlobj.h>
#include <wtsapi32.h>

#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/String.h>
#include <Corrade/Utility/Unicode.h>

#include "MassManager.h"

constexpr unsigned char mass_name_locator[] = { 'N', 'a', 'm', 'e', '_', '4', '5', '_', 'A', '0', '3', '7', 'C', '5', 'D', '5', '4', 'E', '5', '3', '4', '5', '6', '4', '0', '7', 'B', 'D', 'F', '0', '9', '1', '3', '4', '4', '5', '2', '9', 'B', 'B', '\0', 0x0C, '\0', '\0', '\0', 'S', 't', 'r', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

constexpr unsigned char steamid_locator[] = { 'A', 'c', 'c', 'o', 'u', 'n', 't', '\0', 0x0C, '\0', '\0', '\0', 'S', 't', 'r', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

constexpr unsigned char active_slot_locator[] = { 'A', 'c', 't', 'i', 'v', 'e', 'F', 'r', 'a', 'm', 'e', 'S', 'l', 'o', 't', '\0', 0x0C, '\0', '\0', '\0', 'I', 'n', 't', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

constexpr unsigned char credits_locator[] = { 'C', 'r', 'e', 'd', 'i', 't', '\0', 0x0C, '\0', '\0', '\0', 'I', 'n', 't', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

MassManager::MassManager() {
    _ready = findSaveDirectory() && findSteamId();

    if(!_ready) {
        return;
    }

    std::string executable_location = Utility::Directory::path(Utility::Directory::executableLocation());

    _stagingAreaDirectory = Utility::Directory::join(executable_location, "staging");

    _profileSaveName = Utility::formatString("Profile{}.sav", _steamId);

    for(int i = 0; i < 32; ++i) {
        _hangars[i]._filename = Utility::formatString("Unit{:.2d}{}.sav", i, _steamId);
        refreshHangar(i);
    }
}

auto MassManager::ready() -> bool {
    return _ready;
}

auto MassManager::lastError() -> std::string const& {
    return _lastError;
}

auto MassManager::hasDemoUnits() -> bool {
    using Utility::Directory::Flag;
    std::vector<std::string> list = Utility::Directory::list(_saveDirectory, Flag::SkipSpecial|Flag::SkipDirectories|Flag::SkipDotAndDotDot);

    for(const std::string& file : list) {
        if(Utility::String::beginsWith(file, "DemoUnit") && Utility::String::endsWith(file, ".sav")) {
            return true;
        }
    }

    return false;
}

void MassManager::addDemoUnitsToStaging() {
    using Utility::Directory::Flag;
    std::vector<std::string> list = Utility::Directory::list(_saveDirectory, Flag::SkipSpecial|Flag::SkipDirectories|Flag::SkipDotAndDotDot);

    auto predicate = [](const std::string& name)->bool{
        return !(Utility::String::endsWith(name, ".sav") && Utility::String::beginsWith(name, "DemoUnit"));
    };

    list.erase(std::remove_if(list.begin(), list.end(), predicate), list.end());

    for(const std::string& file : list) {
        Utility::Directory::move(Utility::Directory::join(_saveDirectory, file),
                                 Utility::Directory::join(_stagingAreaDirectory, file));
    }
}

auto MassManager::saveDirectory() -> std::string const& {
    return _saveDirectory;
}

auto MassManager::stagingAreaDirectory() -> std::string const& {
    return _stagingAreaDirectory;
}

auto MassManager::screenshotDirectory() -> std::string const& {
    return _screenshotDirectory;
}

auto MassManager::steamId() -> std::string const& {
    return _steamId;
}

auto MassManager::profileSaveName() -> std::string const& {
    return _profileSaveName;
}

auto MassManager::checkGameState() -> GameState {
    WTS_PROCESS_INFOW* process_infos = nullptr;
    unsigned long process_count = 0;

    if(WTSEnumerateProcessesW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &process_infos, &process_count)) {
        for(unsigned long i = 0; i < process_count; ++i) {
            if(std::wcscmp(process_infos[i].pProcessName, L"MASS_Builder-Win64-Shipping.exe") == 0) {
                _gameState = GameState::Running;
                break;
            }
            else {
                _gameState = GameState::NotRunning;
            }
        }
    }
    else {
        _gameState = GameState::Unknown;
    }

    if(process_infos != nullptr) {
        WTSFreeMemory(process_infos);
        process_infos = nullptr;
    }

    return _gameState;
}

auto MassManager::gameState() -> GameState {
    return _gameState;
}

auto MassManager::getActiveSlot() -> char{
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_saveDirectory, _profileSaveName));

    auto iter = std::search(mmap.begin(), mmap.end(), &active_slot_locator[0], &active_slot_locator[31]);

    if(iter == mmap.end()) {
        if(std::search(mmap.begin(), mmap.end(), &credits_locator[0], &credits_locator[22]) != mmap.end()) {
            _activeSlot = 0;
        }
        else {
            _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
            _activeSlot = -1;
        }
    }
    else {
        _activeSlot = *(iter + 41);
    }

    return _activeSlot;
}

auto MassManager::activeSlot() -> char {
    return _activeSlot;
}

auto MassManager::importMass(const std::string& source, int hangar) -> bool {
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar out of range in MassManager::importMass()";
        return false;
    }

    Utility::Directory::copy(source, source + ".tmp");

    {
        auto mmap = Utility::Directory::map(source + ".tmp");

        auto iter = std::search(mmap.begin(), mmap.end(), &steamid_locator[0], &steamid_locator[23]);

        if(iter == mmap.end()) {
            _lastError = "The M.A.S.S. file at " + source + " seems to be corrupt.";
            Utility::Directory::rm(source + ".tmp");
            return false;
        }

        iter += 37;

        if(std::strncmp(iter, _steamId.c_str(), _steamId.length()) != 0) {
            for(int i = 0; i < 17; ++i) {
                *(iter + i) = _steamId[i];
            }
        }
    }

    const std::string dest = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

    if(Utility::Directory::exists(dest)) {
        Utility::Directory::rm(dest);
    }

    Utility::Directory::move(source + ".tmp", dest);

    return true;
}

auto MassManager::importMass(int staged_index, int hangar) -> bool {
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar out of range in MassManager::importMass()";
        return false;
    }

    int i = 0;
    for(const auto& mass_info : _stagedMasses) {
        if(i != staged_index) {
            ++i;
            continue;
        }

        std::string source = Utility::Directory::join(_stagingAreaDirectory, mass_info.first);

        Utility::Directory::copy(source, source + ".tmp");

        {
            auto mmap = Utility::Directory::map(source + ".tmp");

            auto iter = std::search(mmap.begin(), mmap.end(), &steamid_locator[0], &steamid_locator[23]);

            if(iter == mmap.end()) {
                _lastError = "The M.A.S.S. file at " + source + " seems to be corrupt.";
                Utility::Directory::rm(source + ".tmp");
                return false;
            }

            iter += 37;

            if(std::strncmp(iter, _steamId.c_str(), _steamId.length()) != 0) {
                for(int i = 0; i < 17; ++i) {
                    *(iter + i) = _steamId[i];
                }
            }
        }

        const std::string dest = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

        if(Utility::Directory::exists(dest)) {
            Utility::Directory::rm(dest);
        }

        Utility::Directory::move(source + ".tmp", dest);

        return true;
    }

    _lastError = "";
    return false;
}

auto MassManager::exportMass(int hangar) -> bool {
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar out of range in MassManager::exportMass()";
        return false;
    }

    if(_hangars[hangar]._state == HangarState::Empty ||
       _hangars[hangar]._state == HangarState::Invalid) {
        _lastError = Utility::formatString("There is no valid data to export in hangar {:.2d}", hangar);
    }

    auto name = _hangars[hangar]._massName;

    if(!name) {
        _lastError = "There was an unexpected error in MassManager::exportMass()";
        return false;
    }

    std::string source = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);
    std::string dest = Utility::Directory::join(_stagingAreaDirectory, Utility::formatString("{}_{}.sav", _steamId, *(_hangars[hangar]._massName)));

    if(!Utility::Directory::copy(source, dest)) {
        _lastError = Utility::formatString("Couldn't export data from hangar {:.2d} to {}", hangar, dest);
        return false;
    }

    return true;
}

auto MassManager::moveMass(int source, int destination) -> bool {
    if(source < 0 && source >= 32) {
        _lastError = "Source hangar out of range in MassManager::moveMass()";
        return false;
    }

    if(destination < 0 && destination >= 32) {
        _lastError = "Destination hangar out of range in MassManager::moveMass()";
        return false;
    }

    std::string source_file = Utility::Directory::join(_saveDirectory, _hangars[source]._filename);
    std::string dest_file = Utility::Directory::join(_saveDirectory, _hangars[destination]._filename);
    HangarState dest_state = _hangars[destination]._state;

    switch(dest_state) {
        case HangarState::Empty:
            break;
        case HangarState::Invalid:
            Utility::Directory::rm(dest_file);
            break;
        case HangarState::Filled:
            Utility::Directory::move(dest_file, dest_file + ".tmp");
            break;
    }

    Utility::Directory::move(source_file, dest_file);

    if(dest_state == HangarState::Filled) {
        Utility::Directory::move(dest_file + ".tmp", source_file);
    }

    return true;
}

auto MassManager::deleteMass(int hangar) -> bool {
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar number out of range in MassManager::deleteMass()";
        return false;
    }

    std::string file = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

    if(Utility::Directory::exists(file)) {
        if(!Utility::Directory::rm(file)) {
            _lastError = "The M.A.S.S. file couldn't be deleted.";
            return false;
        }
    }

    return true;
}

auto MassManager::renameMass(int hangar, const std::string& new_name) -> bool{
    if(hangar < 0 && hangar >= 32) {
        _lastError = "Hangar number out of range in MassManager::renameMass()";
        return false;
    }

    if(new_name.length() > 32) {
        _lastError = "The new name is longer than 32 characters in MassManager::renameMass()";
        return false;
    }

    char length_difference = static_cast<char>(massName(hangar)->length() - new_name.length());

    std::string mass_data = Utility::Directory::readString(Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename));

    auto iter = std::search(mass_data.begin(), mass_data.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mass_data.end()) {
        *(iter - 45) = *(iter - 45) - length_difference;
        *(iter + 57) = *(iter + 57) - length_difference;
        *(iter + 66) = *(iter + 66) - length_difference;
        while(*(iter + 70) != '\0') {
            mass_data.erase(iter + 70);
        }
        mass_data.insert(iter + 70, new_name.cbegin(), new_name.cend());

        if(!Utility::Directory::writeString(Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename), mass_data)) {
            _lastError = "The file" + _hangars[hangar]._filename + " couldn't be written to.";
            return false;
        }

        return true;
    }
    else {
        _lastError = "Couldn't find the M.A.S.S. name in " + _hangars[hangar]._filename;

        return false;
    }
}

auto MassManager::backupSaves(const std::string& filename) -> bool {
    if(filename.empty() || (filename.length() < 5 && !Utility::String::endsWith(filename, ".zip"))) {
        _lastError = "Invalid filename " + filename + " in MassManager::backupSaves()";
        return false;
    }

    if(Utility::Directory::exists(filename)) {
        if(!Utility::Directory::rm(filename)) {
            _lastError = "Couldn't overwrite " + filename + " in MassManager::backupSaves()";
        }
    }

    wxFFileOutputStream out{filename};
    wxZipOutputStream zip{out};

    {
        zip.PutNextEntry(_profileSaveName);
        wxFFileInputStream profile_stream{Utility::Directory::toNativeSeparators(Utility::Directory::join(_saveDirectory, _profileSaveName)), "rb"};
        zip.Write(profile_stream);
    }

    for(int i = 0; i < 32; ++i) {
        std::string unit_file = Utility::Directory::join(_saveDirectory, _hangars[i]._filename);
        if(Utility::Directory::exists(unit_file)) {
            zip.PutNextEntry(_hangars[i]._filename);
            wxFFileInputStream unit_stream{Utility::Directory::toNativeSeparators(unit_file)};
            zip.Write(unit_stream);
        }
    }

    return true;
}

void MassManager::refreshHangar(int hangar) {
    if(hangar < 0 && hangar >= 32) {
        return;
    }

    std::string unit_file = Utility::Directory::join(_saveDirectory, _hangars[hangar]._filename);

    if(!Utility::Directory::exists(unit_file)) {
        _hangars[hangar]._state = HangarState::Empty;
        _hangars[hangar]._massName = Containers::NullOpt;
    }
    else {
        Containers::Optional<std::string> name = getMassName(unit_file);

        _hangars[hangar]._state = name ? HangarState::Filled : HangarState::Invalid;
        _hangars[hangar]._massName = name;
    }
}

auto MassManager::hangarState(int hangar) -> HangarState {
    if(hangar < 0 && hangar >= 32) {
        return HangarState::Empty;
    }

    return _hangars[hangar]._state;
}

auto MassManager::massName(int hangar) -> Containers::Optional<std::string> {
    if(hangar < 0 && hangar >= 32) {
        return Containers::NullOpt;
    }

    return _hangars[hangar]._massName;
}

auto MassManager::getMassName(const std::string& filename) -> Containers::Optional<std::string> {
    Containers::Optional<std::string> name = Containers::NullOpt;

    auto mmap = Utility::Directory::mapRead(filename);

    auto iter = std::search(mmap.begin(), mmap.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter != mmap.end()) {
        name = std::string{iter + 70};
    }
    else {
        _lastError = "Couldn't find the M.A.S.S. name in " + filename;
    }

    return name;
}

auto MassManager::initialiseStagingArea() -> std::vector<std::string> {
    if(!Utility::Directory::exists(_stagingAreaDirectory)) {
        Utility::Directory::mkpath(_stagingAreaDirectory);
    }

    using Utility::Directory::Flag;
    std::vector<std::string> file_list = Utility::Directory::list(_stagingAreaDirectory, Flag::SkipSpecial|Flag::SkipDirectories|Flag::SkipDotAndDotDot);

    auto iter = std::remove_if(file_list.begin(), file_list.end(), [](std::string& file){
        return !Utility::String::endsWith(file, ".sav");
    });

    file_list.erase(iter, file_list.end());

    std::vector<std::string> mass_names;
    mass_names.reserve(file_list.size());

    for(const std::string& file : file_list) {
        auto name = getMassName(Utility::Directory::join(_stagingAreaDirectory, file));

        if(name) {
            mass_names.push_back(Utility::formatString("{} ({})", *name, file));
            _stagedMasses[file] = *name;
        }
    }

    mass_names.shrink_to_fit();

    return std::move(mass_names);
}

auto MassManager::updateStagedMass(const std::string& filename) -> int {
    std::string file = Utility::Directory::join(_stagingAreaDirectory, filename);

    if(!Utility::Directory::exists(file)) {
        return -1;
    }

    auto name = getMassName(file);

    if(!name) {
        return -1;
    }

    _stagedMasses[filename] = *name;

    int index = 0;

    for(const auto& mass: _stagedMasses) {
        if(mass.first != filename) {
            ++index;
            continue;
        }

        return index;
    }

    return -1;
}

auto MassManager::removeStagedMass(const std::string& filename) -> int {
    int index = 0;

    for(auto it = _stagedMasses.begin(); it != _stagedMasses.end(); ++it, ++index) {
        if(it->first == filename) {
            _stagedMasses.erase(it);
            return index;
        }
    }

    return -1;
}

void MassManager::deleteStagedMass(int index) {
    int i = 0;
    for(auto it = _stagedMasses.begin(); it != _stagedMasses.end(); ++it, ++i) {
        if(i == index) {
            Utility::Directory::rm(Utility::Directory::join(_stagingAreaDirectory, it->first));
            break;
        }
    }
}

auto MassManager::stagedMassName(int index) -> std::string {
    int i = 0;
    for(const auto& mass_info : _stagedMasses) {
        if(i != index) {
            ++i;
            continue;
        }

        return mass_info.second;
    }

    return "";
}

auto MassManager::stagedMassName(const std::string& filename) -> std::string {
    auto iter = _stagedMasses.find(filename);

    if(iter == _stagedMasses.end()) {
        return "";
    }

    return iter->second;
}

void MassManager::loadScreenshots() {
    using Utility::Directory::Flag;
    std::vector<std::string> file_list = Utility::Directory::list(_screenshotDirectory, Flag::SkipSpecial|Flag::SkipDirectories|Flag::SkipDotAndDotDot);

    auto iter = std::remove_if(file_list.begin(), file_list.end(), [](std::string& file){
        return !Utility::String::endsWith(file, ".png");
    });

    file_list.erase(iter, file_list.end());

    _screenshots.reserve(file_list.size());

    for(const std::string& file : file_list) {
        addScreenshot(file);
    }
}

auto MassManager::screenshots() -> std::vector<Screenshot> const& {
    return _screenshots;
}

void MassManager::sortScreenshots(SortType type) {
    _sortType = type;
    sortScreenshots();
}

void MassManager::sortScreenshots(SortOrder order) {
    _sortOrder = order;
    sortScreenshots();
}

void MassManager::sortScreenshots() {
    auto predicate = [this](const Screenshot& item_1, const Screenshot& item_2)->bool{
        switch(_sortType) {
            case SortType::Filename:
                return wxString::FromUTF8(item_1._filename.c_str()).CmpNoCase(wxString::FromUTF8(item_2._filename.c_str())) < 0;
            case SortType::CreationDate:
                return item_1._creationDate.IsEarlierThan(item_2._creationDate);
        }

        return true;
    };

    switch(_sortOrder) {
        case SortOrder::Ascending:
            std::stable_sort(_screenshots.begin(), _screenshots.end(), predicate);
            break;
        case SortOrder::Descending:
            std::stable_sort(_screenshots.rbegin(), _screenshots.rend(), predicate);
            break;
    }
}

auto MassManager::updateScreenshot(const std::string& filename) -> int {
    addScreenshot(filename);
    sortScreenshots();
    int index = 0;
    for(const Screenshot& s : _screenshots) {
        if(s._filename == filename) {
            return index;
        }
    }

    return -1;
}

void MassManager::removeScreenshot(int index) {
    if(static_cast<size_t>(index + 1) > _screenshots.size()) {
        return;
    }

    auto it = _screenshots.begin() + index;
    _screenshots.erase(it);
}

void MassManager::deleteScreenshot(int index) {
    if(static_cast<size_t>(index + 1) > _screenshots.size()) {
        return;
    }

    Utility::Directory::rm(Utility::Directory::join(_screenshotDirectory, _screenshots[index]._filename));
}

auto MassManager::findSaveDirectory() -> bool {
    wchar_t h[MAX_PATH];
    if(!SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, h))) {
        _lastError = "SHGetFolderPathW() failed in MassManager::findSaveDirectory()";
        return false;
    }

    _saveDirectory = Utility::Directory::join(Utility::Directory::fromNativeSeparators(Utility::Unicode::narrow(h)), "MASS_Builder/Saved/SaveGames");

    if(!Utility::Directory::exists(_saveDirectory)) {
        _lastError = _saveDirectory + " wasn't found.";
        return false;
    }

    return true;
}

auto MassManager::findSteamId() -> bool {
    std::vector<std::string> listing = Utility::Directory::list(_saveDirectory);

    wxRegEx regex;
    if(!regex.Compile("Profile([0-9]{17}).sav", wxRE_ADVANCED)) {
        _lastError = "Couldn't compile the regex in MassManager::findSteamId()";
        return false;
    }

    for(const std::string& s : listing) {
        if(regex.Matches(s)) {
            _steamId = regex.GetMatch(s, 1);
            return true;
        }
    }

    _lastError = "Couldn't find the profile save.";
    return false;
}

auto MassManager::findScreenshotDirectory() -> bool {
    wchar_t h[MAX_PATH];
    if(!SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, h))) {
        _lastError = "SHGetFolderPathW() failed in MassManager::findScreenshotDirectory()";
        return false;
    }

    _screenshotDirectory = Utility::Directory::join(Utility::Directory::fromNativeSeparators(Utility::Unicode::narrow(h)), "MASS_Builder/Saved/Screenshots/WindowsNoEditor");

    if(!Utility::Directory::exists(_screenshotDirectory)) {
        _lastError = _screenshotDirectory + " wasn't found.";
        return false;
    }

    return true;
}

void MassManager::addScreenshot(const std::string& filename) {
    std::string screenshot_path = Utility::Directory::toNativeSeparators(Utility::Directory::join(_screenshotDirectory, filename));

    wxFileName screenshot_meta(screenshot_path);
    wxDateTime creation_date;
    screenshot_meta.GetTimes(nullptr, nullptr, &creation_date);

    wxImage thumb{screenshot_path, wxBITMAP_TYPE_PNG};

    wxSize size = thumb.GetSize();
    if(size.GetWidth() > size.GetHeight()) {
        size.Set(160, (size.GetHeight() * 160) / size.GetWidth());
    }
    else if(size.GetHeight() > size.GetWidth()) {
        size.Set((size.GetWidth() * 160) / size.GetHeight(), 160);
    }
    else {
        size.Set(160, 160);
    }

    thumb.Rescale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH)
         .Resize(wxSize{160, 160}, wxPoint{(160 - size.GetWidth()) / 2, (160 - size.GetHeight()) / 2});

    _screenshots.push_back(Screenshot{filename, creation_date, thumb});
}
