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

#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/String.h>

#include <wx/filename.h>

#include "ScreenshotManager.h"

using namespace Corrade;

ScreenshotManager::ScreenshotManager(const std::string& base_path) {
    _screenshotDirectory = Utility::Directory::join(base_path, "Saved/Screenshots/WindowsNoEditor");

    if(!Utility::Directory::exists(_screenshotDirectory)) {
        Utility::Directory::mkpath(_screenshotDirectory);
    }

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

auto ScreenshotManager::screenshotDirectory() -> std::string const& {
    return _screenshotDirectory;
}

auto ScreenshotManager::screenshots() -> std::vector<Screenshot> const& {
    return _screenshots;
}

void ScreenshotManager::sortScreenshots(SortType type) {
    _sortType = type;
    sortScreenshots();
}

void ScreenshotManager::sortScreenshots(SortOrder order) {
    _sortOrder = order;
    sortScreenshots();
}

void ScreenshotManager::sortScreenshots() {
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

auto ScreenshotManager::updateScreenshot(const std::string& filename) -> int {
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

void ScreenshotManager::removeScreenshot(int index) {
    if(static_cast<size_t>(index + 1) > _screenshots.size()) {
        return;
    }

    auto it = _screenshots.begin() + index;
    _screenshots.erase(it);
}

void ScreenshotManager::deleteScreenshot(int index) {
    if(static_cast<size_t>(index + 1) > _screenshots.size()) {
        return;
    }

    Utility::Directory::rm(Utility::Directory::join(_screenshotDirectory, _screenshots[index]._filename));
}

void ScreenshotManager::addScreenshot(const std::string& filename) {
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
