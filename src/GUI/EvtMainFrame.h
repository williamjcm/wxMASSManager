#ifndef __EvtMainFrame__
#define __EvtMainFrame__

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

#include <string>

#include <Corrade/Containers/Pointer.h>

#include <wx/fswatcher.h>
#include <wx/imaglist.h>

#include "../MassManager/MassManager.h"
#include "../MassBuilderManager/MassBuilderManager.h"
#include "../Profile/Profile.h"
#include "../ProfileManager/ProfileManager.h"

#include "MainFrame.h"

using namespace Corrade;

class EvtMainFrame: public MainFrame {
    public:
        EvtMainFrame(wxWindow* parent);
        ~EvtMainFrame();

        auto ready() -> bool;

    protected:
        // Profile-related events
        void profileSelectionEvent(wxCommandEvent&) override;
        void backupSelectedProfileEvent(wxCommandEvent&) override;
        void companyRenameEvent(wxCommandEvent&) override;
        void creditsEditEvent(wxCommandEvent&) override;
        void storyProgressSelectionEvent(wxCommandEvent& event);
        void openStoryProgressMenuEvent(wxCommandEvent&) override;
        void inventoryChangeEvent(wxPropertyGridEvent& event) override;

        // M.A.S.S.-related events
        void importMassEvent(wxCommandEvent&) override;
        void exportMassEvent(wxCommandEvent&) override;
        void moveMassEvent(wxCommandEvent&) override;
        void deleteMassEvent(wxCommandEvent&) override;
        void renameMassEvent(wxCommandEvent&) override;
        void openSaveDirEvent(wxCommandEvent&) override;
        void stagingSelectionEvent(wxCommandEvent&) override;
        void deleteStagedEvent(wxCommandEvent&) override;
        void openStagingDirEvent(wxCommandEvent&) override;
        void installedSelectionEvent(wxListEvent&);
        void listColumnDragEvent(wxListEvent&);

        // Screenshot-related events
        void openScreenshotDirEvent(wxCommandEvent&) override;

        // General events
        void fileUpdateEvent(wxFileSystemWatcherEvent& event);
        void gameCheckTimerEvent(wxTimerEvent&) override;
        void unsafeCheckboxEvent(wxCommandEvent& event) override;

    private:
        void saveFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event);
        void stagingFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event);

        void updateProfileStats();
        void initStoryProgressMenu();

        void initialiseListView();
        void isGameRunning();
        void refreshListView();
        void getActiveSlot();
        void updateCommandsState();
        void refreshHangar(int slot);

        void infoMessage(const wxString& message);
        void warningMessage(const wxString& message);
        void errorMessage(const wxString& message);

        bool _unsafeMode = false;

        MassBuilderManager _mbManager;
        ProfileManager _profileManager;
        Containers::Pointer<MassManager> _massManager;

        Containers::Pointer<wxMenu> _storyProgressSelectionMenu;

        wxFileSystemWatcher _watcher;
        int _lastWatcherEventType = 0;
};

#endif // __EvtMainFrame__
