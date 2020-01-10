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

#include "MainFrame.h"

class EvtMainFrame: public MainFrame {
    public:
        EvtMainFrame(wxWindow* parent);
        ~EvtMainFrame();

    protected:
        void importEvent(wxCommandEvent&);
        void moveEvent(wxCommandEvent&);
        void deleteEvent(wxCommandEvent&);
        void openSaveDirEvent(wxCommandEvent&);
        void installedSelectionEvent(wxListEvent&);
        void listColumnDragEvent(wxListEvent&);
        void fileUpdateEvent(wxFileSystemWatcherEvent& event);
        void gameCheckTimerEvent(wxTimerEvent&);

    private:
        void getSaveDirectory();
        void getLocalSteamId();
        void initialiseListView();
        void isGameRunning();
        void refreshListView();
        void getActiveSlot();
        void updateCommandsState();
        std::string getSlotMassName(int index);
        std::string getMassName(const std::string& filename);

        void infoMessage(const wxString& message);
        void warningMessage(const wxString& message);
        void errorMessage(const wxString& message);

        std::string _saveDirectory;
        std::string _localSteamId;
        bool _isGameRunning = false;
        char _activeSlot = 0;

        wxFileSystemWatcher _watcher;
        int _lastWatcherEventType = 0;
};

#endif // __EvtMainFrame__
