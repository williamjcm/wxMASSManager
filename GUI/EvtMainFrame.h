#ifndef __EvtMainFrame__
#define __EvtMainFrame__

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

#include <wx/fswatcher.h>
#include <wx/imaglist.h>

#include "../MassManager/MassManager.h"

#include "MainFrame.h"

class EvtMainFrame: public MainFrame {
    public:
        EvtMainFrame(wxWindow* parent);
        ~EvtMainFrame();

        auto ready() -> bool;

    protected:
        // M.A.S.S.-related events
        void importMassEvent(wxCommandEvent&);
        void exportMassEvent(wxCommandEvent&);
        void moveMassEvent(wxCommandEvent&);
        void deleteMassEvent(wxCommandEvent&);
        void renameMassEvent(wxCommandEvent&);
        void backupSavesEvent(wxCommandEvent&);
        void openSaveDirEvent(wxCommandEvent&);
        void stagingSelectionEvent(wxCommandEvent&);
        void deleteStagedEvent(wxCommandEvent&);
        void openStagingDirEvent(wxCommandEvent&);
        void installedSelectionEvent(wxListEvent&);
        void listColumnDragEvent(wxListEvent&);

        // Screenshot events
        void screenshotListSelectionEvent(wxListEvent&);
        void screenshotFilenameSortingEvent(wxCommandEvent&);
        void screenshotCreationDateSortingEvent(wxCommandEvent&);
        void screenshotAscendingSortingEvent(wxCommandEvent&);
        void screenshotDescendingSortingEvent(wxCommandEvent&);
        void viewScreenshotEvent(wxCommandEvent&);
        void viewScreenshotEvent(wxListEvent&);
        void deleteScreenshotEvent(wxCommandEvent&);
        void openScreenshotDirEvent(wxCommandEvent&);

        // General events
        void fileUpdateEvent(wxFileSystemWatcherEvent& event);
        void gameCheckTimerEvent(wxTimerEvent&);

    private:
        void unitFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event);
        void stagingFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event);
        void screenshotFileEventHandler(int event_type, const wxString& event_file);

        void initialiseListView();
        void isGameRunning();
        void refreshListView();
        void getActiveSlot();
        void updateCommandsState();
        void refreshHangar(int slot);

        void updateScreenshotList();
        void viewScreenshot();

        void infoMessage(const wxString& message);
        void warningMessage(const wxString& message);
        void errorMessage(const wxString& message);

        MassManager _manager;

        wxFileSystemWatcher _watcher;
        int _lastWatcherEventType = 0;

        wxImageList _screenshotThumbs{160, 160, true, 0};
};

#endif // __EvtMainFrame__
