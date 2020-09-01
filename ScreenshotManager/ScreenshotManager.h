#ifndef SCREENSHOTMANAGER_H
#define SCREENSHOTMANAGER_H

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

#include <string>
#include <vector>

#include <wx/datetime.h>
#include <wx/image.h>

enum class SortType : uint8_t {
    Filename, CreationDate
};

enum class SortOrder: uint8_t {
    Ascending, Descending
};

struct Screenshot {
    std::string _filename;
    wxDateTime  _creationDate;
    wxImage     _thumbnail;
};

class ScreenshotManager {
    public:
        ScreenshotManager(const std::string& base_path);

        auto screenshotDirectory() -> std::string const&;

        auto screenshots() -> std::vector<Screenshot> const&;

        void sortScreenshots(SortType type);
        void sortScreenshots(SortOrder order);
        void sortScreenshots();

        auto updateScreenshot(const std::string& filename) -> int;
        void removeScreenshot(int index);
        void deleteScreenshot(int index);

    private:
        void addScreenshot(const std::string& filename);

        std::string _screenshotDirectory = "";

        std::vector<Screenshot> _screenshots;

        SortType  _sortType  = SortType::Filename;
        SortOrder _sortOrder = SortOrder::Ascending;
};

#endif //SCREENSHOTMANAGER_H
