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
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _credits = -1;
    }

    return _credits;
}

auto Profile::setCredits(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &credits_locator[0], &credits_locator[22]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x20) = amount;
        _credits = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
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
    else {
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
        _storyProgress = progress;
        return true;
    }
    else {
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
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _lastMissionId = -1;
    }

    return _lastMissionId;
}

auto Profile::verseSteel() const -> std::int32_t {
    return _verseSteel;
}

auto Profile::getVerseSteel() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &verse_steel_locator[0], &verse_steel_locator[129]);

    if(iter != mmap.end()) {
        _verseSteel = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _verseSteel = -1;
    }

    return _verseSteel;
}

auto Profile::setVerseSteel(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &verse_steel_locator[0], &verse_steel_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _verseSteel = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::undinium() const -> std::int32_t {
    return _undinium;
}

auto Profile::getUndinium() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &undinium_locator[0], &undinium_locator[129]);

    if(iter != mmap.end()) {
        _undinium = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _undinium = -1;
    }

    return _undinium;
}

auto Profile::setUndinium(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &undinium_locator[0], &undinium_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _undinium = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::necriumAlloy() const -> std::int32_t {
    return _necriumAlloy;
}

auto Profile::getNecriumAlloy() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &necrium_alloy_locator[0], &necrium_alloy_locator[129]);

    if(iter != mmap.end()) {
        _necriumAlloy = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _necriumAlloy = -1;
    }

    return _necriumAlloy;
}

auto Profile::setNecriumAlloy(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &necrium_alloy_locator[0], &necrium_alloy_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _necriumAlloy = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::lunarite() const -> std::int32_t {
    return _lunarite;
}

auto Profile::getLunarite() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &lunarite_locator[0], &lunarite_locator[129]);

    if(iter != mmap.end()) {
        _lunarite = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _lunarite = -1;
    }

    return _lunarite;
}

auto Profile::setLunarite(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &lunarite_locator[0], &lunarite_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _lunarite = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::asterite() const -> std::int32_t {
    return _asterite;
}

auto Profile::getAsterite() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &asterite_locator[0], &asterite_locator[129]);

    if(iter != mmap.end()) {
        _asterite = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _asterite = -1;
    }

    return _asterite;
}

auto Profile::setAsterite(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &asterite_locator[0], &asterite_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _asterite = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::ednil() const -> std::int32_t {
    return _ednil;
}

auto Profile::getEdnil() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &ednil_locator[0], &ednil_locator[129]);

    if(iter != mmap.end()) {
        _ednil = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _ednil = -1;
    }

    return _ednil;
}

auto Profile::setEdnil(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &ednil_locator[0], &ednil_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _ednil = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::nuflalt() const -> std::int32_t {
    return _nuflalt;
}

auto Profile::getNuflalt() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &nuflalt_locator[0], &nuflalt_locator[129]);

    if(iter != mmap.end()) {
        _nuflalt = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _nuflalt = -1;
    }

    return _nuflalt;
}

auto Profile::setNuflalt(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &nuflalt_locator[0], &nuflalt_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _nuflalt = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::aurelene() const -> std::int32_t {
    return _aurelene;
}

auto Profile::getAurelene() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &aurelene_locator[0], &aurelene_locator[129]);

    if(iter != mmap.end()) {
        _aurelene = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _aurelene = -1;
    }

    return _aurelene;
}

auto Profile::setAurelene(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &aurelene_locator[0], &aurelene_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _aurelene = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::soldus() const -> std::int32_t {
    return _soldus;
}

auto Profile::getSoldus() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &soldus_locator[0], &soldus_locator[129]);

    if(iter != mmap.end()) {
        _soldus = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _soldus = -1;
    }

    return _soldus;
}

auto Profile::setSoldus(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &soldus_locator[0], &soldus_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _soldus = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::synthesizedN() const -> std::int32_t {
    return _synthesizedN;
}

auto Profile::getSynthesizedN() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &synthesized_n_locator[0], &synthesized_n_locator[129]);

    if(iter != mmap.end()) {
        _synthesizedN = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _synthesizedN = -1;
    }

    return _synthesizedN;
}

auto Profile::setSynthesizedN(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &synthesized_n_locator[0], &synthesized_n_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _synthesizedN = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::alcarbonite() const -> std::int32_t {
    return _alcarbonite;
}

auto Profile::getAlcarbonite() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &alcarbonite_locator[0], &alcarbonite_locator[129]);

    if(iter != mmap.end()) {
        _alcarbonite = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _alcarbonite = -1;
    }

    return _alcarbonite;
}

auto Profile::setAlcarbonite(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &alcarbonite_locator[0], &alcarbonite_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _alcarbonite = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::keriphene() const -> std::int32_t {
    return _keriphene;
}

auto Profile::getKeriphene() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &keriphene_locator[0], &keriphene_locator[129]);

    if(iter != mmap.end()) {
        _keriphene = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _keriphene= -1;
    }

    return _keriphene;
}

auto Profile::setKeriphene(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &keriphene_locator[0], &keriphene_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _keriphene = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::nitinolCM() const -> std::int32_t {
    return _nitinolCM;
}

auto Profile::getNitinolCM() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &nitinol_cm_locator[0], &nitinol_cm_locator[129]);

    if(iter != mmap.end()) {
        _nitinolCM = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _nitinolCM = -1;
    }

    return _nitinolCM;
}

auto Profile::setNitinolCM(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &nitinol_cm_locator[0], &nitinol_cm_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _nitinolCM = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::quarkium() const -> std::int32_t {
    return _quarkium;
}

auto Profile::getQuarkium() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &quarkium_locator[0], &quarkium_locator[129]);

    if(iter != mmap.end()) {
        _quarkium = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _quarkium = -1;
    }

    return _quarkium;
}

auto Profile::setQuarkium(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &quarkium_locator[0], &quarkium_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _quarkium = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::alterene() const -> std::int32_t {
    return _alterene;
}

auto Profile::getAlterene() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &alterene_locator[0], &alterene_locator[129]);

    if(iter != mmap.end()) {
        _alterene = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _alterene = -1;
    }

    return _alterene;
}

auto Profile::setAlterene(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &alterene_locator[0], &alterene_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _alterene = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::mixedComposition() const -> std::int32_t {
    return _mixedComposition;
}

auto Profile::getMixedComposition() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &mixed_composition_locator[0], &mixed_composition_locator[129]);

    if(iter != mmap.end()) {
        _mixedComposition = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _mixedComposition = -1;
    }

    return _mixedComposition;
}

auto Profile::setMixedComposition(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &mixed_composition_locator[0], &mixed_composition_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _mixedComposition = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::voidResidue() const -> std::int32_t {
    return _voidResidue;
}

auto Profile::getVoidResidue() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &void_residue_locator[0], &void_residue_locator[129]);

    if(iter != mmap.end()) {
        _voidResidue = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _voidResidue = -1;
    }

    return _voidResidue;
}

auto Profile::setVoidResidue(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &void_residue_locator[0], &void_residue_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _voidResidue = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::muscularConstruction() const -> std::int32_t {
    return _muscularConstruction;
}

auto Profile::getMuscularConstruction() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &muscular_construction_locator[0], &muscular_construction_locator[129]);

    if(iter != mmap.end()) {
        _muscularConstruction = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _muscularConstruction = -1;
    }

    return _muscularConstruction;
}

auto Profile::setMuscularConstruction(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &muscular_construction_locator[0], &muscular_construction_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _muscularConstruction = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::mineralExoskeletology() const -> std::int32_t {
    return _mineralExoskeletology;
}

auto Profile::getMineralExoskeletology() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &mineral_exoskeletology_locator[0], &mineral_exoskeletology_locator[129]);

    if(iter != mmap.end()) {
        _mineralExoskeletology = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _mineralExoskeletology = -1;
    }

    return _mineralExoskeletology;
}

auto Profile::setMineralExoskeletology(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &mineral_exoskeletology_locator[0], &mineral_exoskeletology_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _mineralExoskeletology = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
}

auto Profile::carbonizedSkin() const -> std::int32_t {
    return _carbonizedSkin;
}

auto Profile::getCarbonizedSkin() -> std::int32_t {
    auto mmap = Utility::Directory::mapRead(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &carbonized_skin_locator[0], &carbonized_skin_locator[129]);

    if(iter != mmap.end()) {
        _carbonizedSkin = *reinterpret_cast<const std::int32_t*>(iter + 0x8C);
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        _carbonizedSkin = -1;
    }

    return _carbonizedSkin;
}

auto Profile::setCarbonizedSkin(std::int32_t amount) -> bool {
    auto mmap = Utility::Directory::map(Utility::Directory::join(_profileDirectory, _filename));

    auto iter = std::search(mmap.begin(), mmap.end(), &carbonized_skin_locator[0], &carbonized_skin_locator[129]);

    if(iter != mmap.end()) {
        *reinterpret_cast<std::int32_t*>(iter + 0x8C) = amount;
        _carbonizedSkin = amount;
        return true;
    }
    else {
        _lastError = "The profile save seems to be corrupted or the game didn't release the handle on the file.";
        return false;
    }
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
