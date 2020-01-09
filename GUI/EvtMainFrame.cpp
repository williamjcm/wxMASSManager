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

#include <algorithm>
#include <vector>

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include <wx/regex.h>

#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FormatStl.h>

#include "EvtMainFrame.h"

using namespace Corrade;

constexpr unsigned char mass_name_locator[] = { 'N', 'a', 'm', 'e', '_', '4', '5', '_', 'A', '0', '3', '7', 'C', '5', 'D', '5', '4', 'E', '5', '3', '4', '5', '6', '4', '0', '7', 'B', 'D', 'F', '0', '9', '1', '3', '4', '4', '5', '2', '9', 'B', 'B', '\0', 0x0C, '\0', '\0', '\0', 'S', 't', 'r', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

constexpr unsigned char steamid_locator[] = { 'A', 'c', 'c', 'o', 'u', 'n', 't', '\0', 0x0C, '\0', '\0', '\0', 'S', 't', 'r', 'P', 'r', 'o', 'p', 'e', 'r', 't', 'y', '\0' };

EvtMainFrame::EvtMainFrame(wxWindow* parent): MainFrame(parent) {
    SetIcon(wxIcon("MAINICON"));

    getSaveDirectory();
    getLocalSteamId();
    initialiseListView();

    _installedListView->Connect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_ITEM_DESELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_COL_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);

    warningMessage(wxString::FromUTF8("Before you start using this app, a few things you should know:\n\n"
                                      "For this application to work properly, Steam Cloud syncing needs to be disabled for the game.\nTo disable it, right-click the game in your Steam library, click \"Properties\", go to the \"Updates\" tab, and uncheck \"Enable Steam Cloud synchronization for M.A.S.S. Builder\".\n\n"
                                      "Please avoid using this application while the game is running. Bad Things™ could happen to your data.\n\n"
                                      "DISCLAIMER: The developer of this application cannot be held responsible for data loss or corruption. PLEASE USE AT YOUR OWN RISK!\n\n"
                                      "Last but not least, this application is released under the terms of the GNU General Public Licence version 3. Please see the COPYING file for more details."));
}

EvtMainFrame::~EvtMainFrame() {
    _installedListView->Disconnect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_ITEM_DESELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_COL_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
}

void EvtMainFrame::importEvent(wxCommandEvent&) {
    std::string slot_state = _installedListView->GetItemText(_installedListView->GetFirstSelected(), 1).ToStdString();

    if(slot_state != "<Empty>" && slot_state != "<Invalid data>" &&
       wxMessageBox(wxString::Format("Hangar %.2d is already occupied by the M.A.S.S. named \"%s\". Are you sure you want to import a M.A.S.S. to this hangar ?", _installedListView->GetFirstSelected() + 1, slot_state),
                    "Question", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    wxFileDialog dialog(this, "Select a unit file", wxEmptyString, wxEmptyString, "M.A.S.S. Builder unit files (*.sav)|*.sav", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if(dialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    const std::string source_file = dialog.GetPath().ToUTF8().data();

    const std::string mass_name = getMassName(source_file);

    if(mass_name == "") {
        return;
    }

    if(wxMessageBox(wxString::Format("Are you sure you want to import the M.A.S.S. named \"%s\" to hangar %.2d ?", mass_name, _installedListView->GetFirstSelected() + 1),
                    "Question", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    const std::string dest_file = _saveDirectory + Utility::formatString("/Unit{:.2d}{}.sav", _installedListView->GetFirstSelected(), _localSteamId);

    if(Utility::Directory::exists(dest_file)) {
        Utility::Directory::rm(dest_file);
    }

    Utility::Directory::copy(source_file, dest_file);

    {
        auto mmap = Utility::Directory::map(dest_file);

        auto iter = std::search(mmap.begin(), mmap.end(), &steamid_locator[0], &steamid_locator[23]);

        if(iter == mmap.end()) {
            errorMessage("Couldn't find the SteamID in the unit file at " + source_file + ". Aborting...");
            Utility::Directory::rm(dest_file);
            return;
        }

        iter += 37;

        for(int i = 0; i < 17; ++i) {
            *(iter + i) = _localSteamId[i];
        }
    }

    refreshListView();
}

void EvtMainFrame::moveEvent(wxCommandEvent&) {
    long source_slot = _installedListView->GetFirstSelected();

    long choice = wxGetNumberFromUser(wxString::Format("Which hangar do you want to move the M.A.S.S. named \"%s\" to ?\nNotes:\n- If the destination hangar is the same as the source, nothing will happen.\n- If the destination already contains a M.A.S.S., the two will be swapped.\n- If the destination contains invalid data, it will be cleared first.", _installedListView->GetItemText(source_slot, 1)),
                                      "Slot", "Choose a slot", source_slot + 1, 1, 32, this) - 1;

    if(choice == -1 || choice == source_slot) {
        return;
    }

    std::string orig_file = Utility::formatString("{}/Unit{:.2d}{}.sav", _saveDirectory, source_slot, _localSteamId);
    std::string dest_status = _installedListView->GetItemText(choice, 1).ToStdString();
    std::string dest_file = Utility::formatString("{}/Unit{:.2d}{}.sav", _saveDirectory, choice, _localSteamId);

    if(dest_status == "<Invalid data>") {
        Utility::Directory::rm(dest_file);
    }
    else if(dest_status != "<Empty>") {
        Utility::Directory::move(dest_file, dest_file + ".tmp");
    }

    Utility::Directory::move(orig_file, dest_file);

    if(dest_status != "<Empty>") {
        Utility::Directory::move(dest_file + ".tmp", orig_file);
    }

    refreshListView();
}

void EvtMainFrame::deleteEvent(wxCommandEvent&) {
    if(wxMessageBox(wxString::Format("Are you sure you want to delete the data in hangar %.2d ? This operation cannot be undone.", _installedListView->GetFirstSelected() + 1),
                    "Are you sure ?", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    std::string file = Utility::formatString("{}/Unit{:.2d}{}.sav", _saveDirectory, _installedListView->GetFirstSelected(), _localSteamId);

    if(Utility::Directory::exists(file)) {
        Utility::Directory::rm(file);
    }

    refreshListView();
}

void EvtMainFrame::openSaveDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(_saveDirectory));
}

void EvtMainFrame::installedSelectionEvent(wxListEvent&) {
    updateCommandsState();
}

void EvtMainFrame::listColumnDragEvent(wxListEvent& event) {
    event.Veto();
}

void EvtMainFrame::timerEvent(wxTimerEvent&) {
    refreshListView();
}

void EvtMainFrame::getSaveDirectory() {
    _saveDirectory = Utility::Directory::path(Utility::Directory::home()) + "/AppData/Local/MASS_Builder/Saved/SaveGames";

    if(!Utility::Directory::exists(_saveDirectory)) {
        errorMessage("Couldn't find the M.A.S.S. Builder save directory at " + _saveDirectory + ". Please run the game at least once to create it.");
        Close();
    }
}

void EvtMainFrame::getLocalSteamId() {
    std::vector<std::string> listing = Utility::Directory::list(_saveDirectory);

    wxRegEx regex;
    if(!regex.Compile("Profile([0-9]{17}).sav", wxRE_ADVANCED)) {
        errorMessage("Couldn't compile the regex.");
        Close();
        return;
    }

    for(const std::string& s : listing) {
        if(regex.Matches(s)) {
            _localSteamId = regex.GetMatch(s, 1);
            return;
        }
    }

    errorMessage("Couldn't find your save files. Please play at least once.");
    Close();
}

void EvtMainFrame::initialiseListView() {
    for(long i = 0; i < 32; i++) {
        _installedListView->InsertItem(i, wxString::Format("%.2i", i + 1));
    }

    _installedListView->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    _installedListView->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);

    refreshListView();

    updateCommandsState();

    _refreshTimer.Start(1000);
}

void EvtMainFrame::refreshListView() {
    for(long i = 0; i < 32; i++) {
        _installedListView->SetItem(i, 1, getSlotMassName(i));
    }

    updateCommandsState();
}

void EvtMainFrame::updateCommandsState() {
    long selection = _installedListView->GetFirstSelected();

    if(selection != -1) {
        _importButton->Enable();

        wxString state = _installedListView->GetItemText(selection, 1);
        if(state != "<Empty>" && state != "<Invalid data>") {
            _moveButton->Enable();
        }
        else {
            _moveButton->Disable();
        }

        if(state != "<Empty>") {
            _deleteButton->Enable();
        }
        else {
            _deleteButton->Disable();
        }
    }
    else {
        _importButton->Disable();
        _moveButton->Disable();
        _deleteButton->Disable();
    }
}

std::string EvtMainFrame::getSlotMassName(int index) {
    std::string unit_file = Utility::formatString("{}/Unit{:.2d}{}.sav", _saveDirectory, index, _localSteamId);
    if(Utility::Directory::exists(unit_file)) {
        std::string mass_name = getMassName(unit_file);
        return (mass_name == "" ? "<Invalid data>" : mass_name);
    }
    else {
        return "<Empty>";
    }
}

std::string EvtMainFrame::getMassName(const std::string& filename) {
    auto mmap = Utility::Directory::mapRead(filename);

    auto iter = std::search(mmap.begin(), mmap.end(), &mass_name_locator[0], &mass_name_locator[56]);

    if(iter == mmap.end()) {
        return "";
    }

    return iter + 70;
}

void EvtMainFrame::infoMessage(const wxString& message) {
    wxMessageBox(message, "Information", wxOK|wxCENTRE|wxICON_INFORMATION, this);
}

void EvtMainFrame::warningMessage(const wxString& message) {
    wxMessageBox(message, "Warning", wxOK|wxCENTRE|wxICON_WARNING, this);
}

void EvtMainFrame::errorMessage(const wxString& message) {
    wxMessageBox(message, "Error", wxOK|wxCENTRE|wxICON_ERROR, this);
}
