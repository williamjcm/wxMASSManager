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

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include <wx/regex.h>
#include <wx/scrolwin.h>

#include <Corrade/Containers/Optional.h>
#include <Corrade/Utility/Directory.h>

#include "EvtNameChangeDialog.h"

#include "EvtMainFrame.h"

using namespace Corrade;

EvtMainFrame::EvtMainFrame(wxWindow* parent): MainFrame(parent) {
    SetIcon(wxIcon("MAINICON"));

    if(!_manager.ready()) {
        errorMessage("There was an issue initialising the manager:\n\n" + _manager.lastError());
        return;
    }

    initialiseListView();

    isGameRunning();

    _installedListView->Connect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_ITEM_DESELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_COL_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);

    warningMessage(wxString::FromUTF8("Before you start using this app, a few things you should know:\n\n"
                                      "For this application to work properly, Steam Cloud syncing needs to be disabled for the game.\nTo disable it, right-click the game in your Steam library, click \"Properties\", go to the \"Updates\" tab, and uncheck \"Enable Steam Cloud synchronization for M.A.S.S. Builder\".\n\n"
                                      "DISCLAIMER: The developer of this application cannot be held responsible for data loss or corruption. PLEASE USE AT YOUR OWN RISK!\n\n"
                                      "Last but not least, this application is released under the terms of the GNU General Public Licence version 3. Please see the COPYING file for more details."));

    _watcher.Connect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(EvtMainFrame::fileUpdateEvent), nullptr, this);
    _watcher.AddTree(wxFileName(Utility::Directory::toNativeSeparators(_manager.saveDirectory()), wxPATH_WIN),
                     wxFSW_EVENT_CREATE|wxFSW_EVENT_DELETE|wxFSW_EVENT_MODIFY|wxFSW_EVENT_RENAME, wxString::Format("*%s.sav", _manager.steamId()));
    _watcher.AddTree(wxFileName(Utility::Directory::toNativeSeparators(_manager.stagingAreaDirectory()), wxPATH_WIN),
                     wxFSW_EVENT_CREATE|wxFSW_EVENT_DELETE|wxFSW_EVENT_MODIFY|wxFSW_EVENT_RENAME, "*.sav");
    _watcher.AddTree(wxFileName(Utility::Directory::toNativeSeparators(_manager.screenshotDirectory()), wxPATH_WIN),
                     wxFSW_EVENT_CREATE|wxFSW_EVENT_DELETE, "*.png"); // Not monitoring wxFSW_EVENT_{MODIFY,RENAME}, because they're a massive pain to handle. Ugh.

    std::vector<std::string> v = _manager.initialiseStagingArea();
    for(const std::string& s : v) {
        _stagingList->Append(s);
    }

    _gameCheckTimer.Start(3000);

    _screenshotsList->SetImageList(&_screenshotThumbs, wxIMAGE_LIST_NORMAL);

    updateScreenshotList();
}

EvtMainFrame::~EvtMainFrame() {
    _installedListView->Disconnect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_ITEM_DESELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_COL_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _watcher.Disconnect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(EvtMainFrame::fileUpdateEvent), nullptr, this);
}

bool EvtMainFrame::ready() {
    return _manager.ready();
}

void EvtMainFrame::importMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Importing failed:\n\n";

    long selected_hangar = _installedListView->GetFirstSelected();
    HangarState hangar_state = _manager.hangarState(selected_hangar);

    int staged_selection = _stagingList->GetSelection();

    int confirmation;

    if(hangar_state == HangarState::Filled) {
        confirmation = wxMessageBox(wxString::Format("Hangar %.2d is already occupied by the M.A.S.S. named \"%s\". Are you sure you want to import the M.A.S.S. named \"%s\" to this hangar ?",
                                      selected_hangar + 1, *(_manager.massName(selected_hangar)), _manager.stagedMassName(staged_selection)),
                                    "Question", wxYES_NO|wxCENTRE|wxICON_QUESTION, this);
    }
    else {
        confirmation = wxMessageBox(wxString::Format("Are you sure you want to import the M.A.S.S. named \"%s\" to hangar %.2d ?", _manager.stagedMassName(staged_selection), selected_hangar + 1),
                                    "Question", wxYES_NO|wxCENTRE|wxICON_QUESTION, this);
    }

    if(confirmation == wxNO) {
        return;
    }

    switch(_manager.gameState()) {
        case GameState::Unknown:
            errorMessage(error_prefix + "For security reasons, importing is disabled if the game's status is unknown.");
            break;
        case GameState::NotRunning:
            if(!_manager.importMass(staged_selection, selected_hangar)) {
                errorMessage(error_prefix + _manager.lastError());
            }
            break;
        case GameState::Running:
            errorMessage(error_prefix + "Importing a M.A.S.S. is disabled while the game is running.");
            break;
    }
}

void EvtMainFrame::exportMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Export failed:\n\n";

    long slot = _installedListView->GetFirstSelected();

    if(!_manager.exportMass(slot)) {
        errorMessage(error_prefix + _manager.lastError());
    }
}

void EvtMainFrame::moveMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Move failed:\n\n";

    long source_slot = _installedListView->GetFirstSelected();

    long choice = wxGetNumberFromUser(wxString::Format("Which hangar do you want to move the M.A.S.S. named \"%s\" to ?\nNotes:\n"
                                                       "- If the destination hangar is the same as the source, nothing will happen.\n"
                                                       "- If the destination already contains a M.A.S.S., the two will be swapped.\n"
                                                       "- If the destination contains invalid data, it will be cleared first.",
                                                       *(_manager.massName(source_slot))),
                                      "Slot", "Choose a slot", source_slot + 1, 1, 32, this);

    if(choice == -1 || choice == source_slot) {
        return;
    }

    switch(_manager.gameState()) {
        case GameState::Unknown:
            errorMessage(error_prefix + "For security reasons, moving a M.A.S.S. is disabled if the game's status is unknown.");
            break;
        case GameState::NotRunning:
            if(!_manager.moveMass(source_slot, choice - 1)) {
                errorMessage(error_prefix + _manager.lastError());
            }
            break;
        case GameState::Running:
            errorMessage(error_prefix + "Moving a M.A.S.S. is disabled while the game is running.");
            break;
    }
}

void EvtMainFrame::deleteMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Deletion failed:\n\n";

    if(wxMessageBox(wxString::Format("Are you sure you want to delete the data in hangar %.2d ? This operation cannot be undone.", _installedListView->GetFirstSelected() + 1),
                    "Are you sure ?", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    switch(_manager.gameState()) {
        case GameState::Unknown:
            errorMessage(error_prefix + "For security reasons, deleting a M.A.S.S. is disabled if the game's status is unknown.");
            break;
        case GameState::NotRunning:
            if(!_manager.deleteMass(_installedListView->GetFirstSelected())) {
                errorMessage(error_prefix + _manager.lastError());
            }
            break;
        case GameState::Running:
            errorMessage(error_prefix + "Deleting a M.A.S.S. is disabled while the game is running.");
            break;
    }
}

void EvtMainFrame::renameMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Rename failed:\n\n";

    EvtNameChangeDialog dialog{this};
    dialog.setName(*(_manager.massName(_installedListView->GetFirstSelected())));
    int result = dialog.ShowModal();

    if(result == wxID_OK) {
        switch(_manager.gameState()) {
            case GameState::Unknown:
                errorMessage(error_prefix + "For security reasons, renaming a M.A.S.S. is disabled if the game's status is unknown.");
                break;
            case GameState::NotRunning:
                if(!_manager.renameMass(_installedListView->GetFirstSelected(), dialog.getName())) {
                    errorMessage(error_prefix + _manager.lastError());
                }
                break;
            case GameState::Running:
                errorMessage(error_prefix + "Renaming a M.A.S.S. is disabled while the game is running.");
                break;
        }
    }
}

void EvtMainFrame::backupEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Backup failed:\n\n";

    wxString current_timestamp = wxDateTime::Now().Format("%Y-%m-%d_%H-%M-%S");

    wxFileDialog save_dialog{this, "Choose output location", _manager.saveDirectory(),
                             wxString::Format("backup_%s_%s.zip", _manager.steamId(), current_timestamp), "Zip archive (*zip)|*zip",
                             wxFD_SAVE|wxFD_OVERWRITE_PROMPT};

    if(save_dialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    if(!_manager.backupSaves(save_dialog.GetPath().ToStdString())) {
        errorMessage(error_prefix + _manager.lastError());
    }
}

void EvtMainFrame::openSaveDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(_manager.saveDirectory()));
}

void EvtMainFrame::stagingSelectionEvent(wxCommandEvent&) {
    updateCommandsState();
}

void EvtMainFrame::deleteStagedEvent(wxCommandEvent&) {
    if(wxMessageBox("Are you sure you want to delete the selected M.A.S.S. ? This operation cannot be undone.",
                    "Are you sure ?", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    int selection = _stagingList->GetSelection();

    if(selection != wxNOT_FOUND) {
        _manager.deleteStagedMass(selection);
    }
}

void EvtMainFrame::openStagingDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(_manager.stagingAreaDirectory()));
}

void EvtMainFrame::installedSelectionEvent(wxListEvent&) {
    updateCommandsState();
}

void EvtMainFrame::listColumnDragEvent(wxListEvent& event) {
    event.Veto();
}

void EvtMainFrame::screenshotListSelectionEvent(wxListEvent&) {
    updateCommandsState();
}

void EvtMainFrame::screenshotFilenameSortingEvent(wxCommandEvent&) {
    _manager.sortScreenshots(SortType::Filename);
    updateScreenshotList();
}

void EvtMainFrame::screenshotCreationDateSortingEvent(wxCommandEvent&) {
    _manager.sortScreenshots(SortType::CreationDate);
    updateScreenshotList();
}

void EvtMainFrame::screenshotAscendingSortingEvent(wxCommandEvent&) {
    _manager.sortScreenshots(SortOrder::Ascending);
    updateScreenshotList();
}

void EvtMainFrame::screenshotDescendingSortingEvent(wxCommandEvent&) {
    _manager.sortScreenshots(SortOrder::Descending);
    updateScreenshotList();
}

void EvtMainFrame::viewScreenshotEvent(wxCommandEvent&) {
    viewScreenshot();
}

void EvtMainFrame::viewScreenshotEvent(wxListEvent&) {
    viewScreenshot();
}

void EvtMainFrame::deleteScreenshotEvent(wxCommandEvent&) {
    if(wxMessageBox("Are you sure you want to delete the selected screenshot ? This operation cannot be undone.",
                    "Are you sure ?", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    long selection = _screenshotsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selection != -1) {
        _manager.deleteScreenshot(selection);
    }
}

void EvtMainFrame::openScreenshotDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(_manager.screenshotDirectory()));
}

void EvtMainFrame::fileUpdateEvent(wxFileSystemWatcherEvent& event) {
    int event_type = event.GetChangeType();
    wxString event_file = event.GetPath().GetFullName();

    if(event_type == wxFSW_EVENT_MODIFY && _lastWatcherEventType == wxFSW_EVENT_RENAME) {
        _lastWatcherEventType = event_type;
        return;
    }

    wxMilliSleep(50);

    if(event.GetPath().GetPath(wxPATH_GET_VOLUME, wxPATH_WIN) == Utility::Directory::toNativeSeparators(_manager.saveDirectory())) {
        unitFileEventHandler(event_type, event_file, event);
    }
    else if(event.GetPath().GetPath(wxPATH_GET_VOLUME, wxPATH_WIN) == Utility::Directory::toNativeSeparators(_manager.stagingAreaDirectory())) {
        stagingFileEventHandler(event_type, event_file, event);
    }
    else if(event.GetPath().GetPath(wxPATH_GET_VOLUME, wxPATH_WIN) == Utility::Directory::toNativeSeparators(_manager.screenshotDirectory())) {
        screenshotFileEventHandler(event_type, event_file);
    }

    _lastWatcherEventType = event_type;

    updateCommandsState();
}

void EvtMainFrame::gameCheckTimerEvent(wxTimerEvent&) {
    isGameRunning();
}

void EvtMainFrame::unitFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event) {
    wxRegEx regex;

    switch (event_type) {
        case wxFSW_EVENT_CREATE:
        case wxFSW_EVENT_DELETE:
            regex.Compile(wxString::Format("Unit([0-3][0-9])%s\\.sav", _manager.steamId()), wxRE_ADVANCED);
            if(regex.Matches(event_file)) {
                long slot;

                if(regex.GetMatch(event_file, 1).ToLong(&slot) && slot >= 0 && slot < 32) {
                    refreshHangar(slot);
                }
            }
            break;
        case wxFSW_EVENT_MODIFY:
            if(_lastWatcherEventType == wxFSW_EVENT_RENAME) {
                break;
            }
            if(event_file == _manager.profileSaveName()) {
                getActiveSlot();
            }
            else {
                regex.Compile(wxString::Format("Unit([0-3][0-9])%s\\.sav", _manager.steamId()), wxRE_ADVANCED);
                if(regex.Matches(event_file)) {
                    long slot;

                    if(regex.GetMatch(event_file, 1).ToLong(&slot) && slot >= 0 && slot < 32) {
                        refreshHangar(slot);
                    }
                }
            }
            break;
        case wxFSW_EVENT_RENAME:
            wxString new_name = event.GetNewPath().GetFullName();

            long slot;
            if(regex.Compile(wxString::Format("Unit([0-3][0-9])%s\\.sav\\.tmp", _manager.steamId()), wxRE_ADVANCED), regex.Matches(new_name)) {
                if(regex.GetMatch(new_name, 1).ToLong(&slot) && slot >= 0 && slot < 32) {
                    refreshHangar(slot);
                }
            }
            else if(regex.Compile(wxString::Format("Unit([0-3][0-9])%s\\.sav", _manager.steamId()), wxRE_ADVANCED), regex.Matches(new_name)) {
                if(regex.GetMatch(new_name, 1).ToLong(&slot) && slot >= 0 && slot < 32) {
                    refreshHangar(slot);
                    if(regex.Matches(event_file)) {
                        if(regex.GetMatch(event_file, 1).ToLong(&slot) && slot >= 0 && slot < 32) {
                            refreshHangar(slot);
                        }
                    }
                }
            }
            break;
    }
}

void EvtMainFrame::stagingFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event) {
    int index;

    switch(event_type) {
        case wxFSW_EVENT_CREATE:
            index = _manager.updateStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->Insert(wxString::Format("%s (%s)", _manager.stagedMassName(index), event_file), index);
            }
            break;
        case wxFSW_EVENT_DELETE:
            index = _manager.removeStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->Delete(index);
            }
            break;
        case wxFSW_EVENT_MODIFY:
            index = _manager.updateStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->SetString(index, wxString::Format("%s (%s)", _manager.stagedMassName(index), event_file));
            }
            break;
        case wxFSW_EVENT_RENAME:
            index = _manager.removeStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->Delete(index);
            }
            index = _manager.updateStagedMass(event.GetNewPath().GetFullName().ToUTF8().data());
            if(index != -1) {
                _stagingList->Insert(wxString::Format("%s (%s)", _manager.stagedMassName(index), event.GetNewPath().GetFullName()), index);
            }
            break;
    }
}

void EvtMainFrame::screenshotFileEventHandler(int event_type, const wxString& event_file) {
    int index = -1;

    switch(event_type) {
        case wxFSW_EVENT_CREATE:
            _manager.updateScreenshot(event_file.ToUTF8().data());
            updateScreenshotList();
            break;
        case wxFSW_EVENT_DELETE:
            index = _screenshotsList->FindItem(-1, event_file, true);
            if(index != -1) {
                _manager.removeScreenshot(index);
                _screenshotsList->DeleteItem(index);
            }
            break;
    }
}

void EvtMainFrame::initialiseListView() {
    for(long i = 0; i < 32; i++) {
        _installedListView->InsertItem(i, wxString::Format("%.2i", i + 1));
    }

    _installedListView->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    _installedListView->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);

    refreshListView();
    getActiveSlot();
}

void EvtMainFrame::isGameRunning() {
    GameState state = _manager.checkGameState();

    switch(state) {
        case GameState::Unknown:
            _gameStatus->SetLabel("unknown");
            _gameStatus->SetForegroundColour(wxColour("orange"));
            break;
        case GameState::NotRunning:
            _gameStatus->SetLabel("not running");
            _gameStatus->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));
            break;
        case GameState::Running:
            _gameStatus->SetLabel("running");
            _gameStatus->SetForegroundColour(wxColour("red"));
            break;
    }

    updateCommandsState();
}

void EvtMainFrame::refreshListView() {
    for(int i = 0; i < 32; i++) {
        refreshHangar(i);
    }

    updateCommandsState();
}

void EvtMainFrame::getActiveSlot() {
    char slot = _manager.activeSlot();

    if(slot != -1) {
        wxFont tmp_font = _installedListView->GetItemFont(slot);
        tmp_font.SetWeight(wxFONTWEIGHT_NORMAL);
        _installedListView->SetItemFont(slot, tmp_font);
    }

    slot = _manager.getActiveSlot();

    if(slot != -1) {
        _installedListView->SetItemFont(slot, _installedListView->GetItemFont(slot).Bold());
    }
}

void EvtMainFrame::updateCommandsState() {
    long selection = _installedListView->GetFirstSelected();
    int staged_selection = _stagingList->GetSelection();
    GameState game_state = _manager.gameState();
    HangarState hangar_state = _manager.hangarState(selection);

    _importButton->Enable(selection != -1 && staged_selection != -1 && game_state != GameState::Running);
    _exportButton->Enable(selection != -1);
    _moveButton->Enable(selection != -1 && game_state != GameState::Running && hangar_state != HangarState::Empty && hangar_state != HangarState::Invalid);
    _deleteButton->Enable(selection != -1 && game_state != GameState::Running && hangar_state != HangarState::Empty);
    _renameButton->Enable(selection != -1 && game_state != GameState::Running && hangar_state != HangarState::Empty);
    _deleteStagedButton->Enable(staged_selection != -1);

    long screenshot_selection = _screenshotsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    _viewScreenshotButton->Enable(screenshot_selection != -1);
    _deleteScreenshotButton->Enable(screenshot_selection != -1);
}

void EvtMainFrame::refreshHangar(int slot) {
    if(slot < 0 && slot >= 32) {
        return;
    }

    _manager.refreshHangar(slot);

    switch(_manager.hangarState(slot)) {
        case HangarState::Empty:
            _installedListView->SetItem(slot, 1, "<Empty>");
            break;
        case HangarState::Invalid:
            _installedListView->SetItem(slot, 1, "<Invalid>");
            break;
        case HangarState::Filled:
            _installedListView->SetItem(slot, 1, *(_manager.massName(slot)));
            break;
    }
}

void EvtMainFrame::updateScreenshotList() {
    _screenshotsList->DeleteAllItems();
    _screenshotThumbs.RemoveAll();

    int index = 0;
    for(const Screenshot& s : _manager.screenshots()) {
        _screenshotsList->InsertItem(index,
                                     wxString::Format("%s\n%s", wxString::FromUTF8(s._filename.c_str()), s._creationDate.Format("%d/%m/%Y %H:%M:%S")),
                                     _screenshotThumbs.Add(s._thumbnail));

        ++index;
    }
}

void EvtMainFrame::viewScreenshot() {
    long selection = _screenshotsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if(selection == -1) {
        return;
    }

    wxBitmap image(Utility::Directory::toNativeSeparators(Utility::Directory::join(_manager.screenshotDirectory(), _manager.screenshots().at(selection)._filename)), wxBITMAP_TYPE_PNG);

    wxDialog view_dialog;
    view_dialog.Create(this, wxID_ANY, "Screenshot viewer", wxDefaultPosition, wxSize{1024, 576}, wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxScrolledWindow* scroller = new wxScrolledWindow(&view_dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL);
    scroller->SetScrollRate(5, 5);

    wxBoxSizer* scroller_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBitmap* screenshot = new wxStaticBitmap(scroller, wxID_ANY, image);
    scroller_sizer->Add(screenshot, 1, wxEXPAND, 5);

    scroller->SetSizer(scroller_sizer);
    scroller->Layout();
    scroller_sizer->FitInside(scroller);
    sizer->Add(scroller, 1, wxEXPAND, 5);

    view_dialog.SetSizer(sizer);
    view_dialog.Layout();
    sizer->FitInside(&view_dialog);
    view_dialog.Centre();

    view_dialog.ShowModal();
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
