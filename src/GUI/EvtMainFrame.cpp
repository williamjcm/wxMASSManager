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

#include <Corrade/version.h>

#if !(CORRADE_VERSION_YEAR * 100 + CORRADE_VERSION_MONTH >= 202006)
    #error This application requires Corrade 2020.06 or later to build.
#endif

#include <wx/busyinfo.h>
#include <wx/filedlg.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include <wx/regex.h>
#include <wx/scrolwin.h>

#include <Corrade/Containers/Optional.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FormatStl.h>

#include "EvtNameChangeDialog.h"

#include "EvtMainFrame.h"

#include "../Maps/LastMissionId.h"
#include "../Maps/StoryProgress.h"

EvtMainFrame::EvtMainFrame(wxWindow* parent):
    MainFrame(parent),
    _profileManager{_mbManager.saveDirectory()}
{
    SetIcon(wxIcon("MAINICON"));

    warningMessage(wxString::FromUTF8("Before you start using this app, a few things you should know:\n\n"
                                      "For this application to work properly, it is recommended to disable Steam Cloud syncing for the game.\nTo disable it, right-click the game in your Steam library, click \"Properties\", go to the \"General\" tab, and uncheck \"Keep game saves in the Steam Cloud for M.A.S.S. Builder\".\n\n"
                                      "DISCLAIMER: The developer of this application (Guillaume Jacquemin) isn't associated with Vermillion Digital, and both parties cannot be held responsible for data loss or corruption this app might cause. PLEASE USE AT YOUR OWN RISK!\n\n"
                                      "Last but not least, this application is released under the terms of the GNU General Public Licence version 3. Please see the COPYING file for more details."));

    if(!_mbManager.ready()) {
        errorMessage("There was an error initialising the manager:\n\n" + _mbManager.lastError());
        return;
    }

    if(!_profileManager.ready()) {
        errorMessage("There was an error initialising the manager:\n\n" + _profileManager.lastError());
        return;
    }

    std::size_t default_profile = 0;
    int counter = 0;

    for(const Profile& p : _profileManager.profiles()) {
        if(p.valid()) {
            _profileChoice->Append(wxString::Format("%s%s", p.companyName(), p.type() == ProfileType::Demo ? " (Demo)" : ""));

            if(p.type() == ProfileType::FullGame && default_profile == 0) {
                default_profile = counter;
            }

            counter++;
        }
    }

    _profileManager.setProfile(default_profile);
    _profileChoice->SetSelection(default_profile);

    _massManager.emplace(_profileManager.profileDirectory(),
                         _profileManager.currentProfile()->steamId(),
                         _profileManager.currentProfile()->type() == ProfileType::Demo);

    updateProfileStats();

    initialiseListView();

    isGameRunning();

    _installedListView->Connect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_ITEM_DESELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _installedListView->Connect(wxEVT_LIST_COL_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);

    _watcher.Connect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(EvtMainFrame::fileUpdateEvent), nullptr, this);
    _watcher.AddTree(wxFileName{Utility::Directory::toNativeSeparators(_massManager->saveDirectory()), wxPATH_WIN},
                     wxFSW_EVENT_CREATE|wxFSW_EVENT_DELETE|wxFSW_EVENT_MODIFY|wxFSW_EVENT_RENAME,
                     wxString::Format("%s*%s.sav", _profileManager.currentProfile()->type() == ProfileType::Demo ? "Demo" : "", _profileManager.currentProfile()->steamId()));

    auto staged_masses = _massManager->stagedMasses();
    for(const auto& s : staged_masses) {
        _stagingList->Append(wxString::Format("%s (%s)", s.second, s.first));
    }

    _watcher.AddTree(wxFileName(Utility::Directory::toNativeSeparators(_massManager->stagingAreaDirectory()), wxPATH_WIN),
                     wxFSW_EVENT_CREATE|wxFSW_EVENT_DELETE|wxFSW_EVENT_MODIFY|wxFSW_EVENT_RENAME, "*.sav");

    _gameCheckTimer.Start(2000);

    initStoryProgressMenu();
}

EvtMainFrame::~EvtMainFrame() {
    _watcher.RemoveAll();
    _watcher.Disconnect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(EvtMainFrame::fileUpdateEvent), nullptr, this);

    _installedListView->Disconnect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_ITEM_DESELECTED, wxListEventHandler(EvtMainFrame::installedSelectionEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
    _installedListView->Disconnect(wxEVT_LIST_COL_BEGIN_DRAG, wxListEventHandler(EvtMainFrame::listColumnDragEvent), nullptr, this);
}

bool EvtMainFrame::ready() {
    return _mbManager.ready() && _profileManager.ready();
}

void EvtMainFrame::profileSelectionEvent(wxCommandEvent&) {
    _watcher.Remove(wxFileName{Utility::Directory::toNativeSeparators(Utility::Directory::path(_massManager->saveDirectory())) + "\\", wxPATH_WIN});
        // Yeah, I really need that `+ "\\"`, because wxWidgets is *that* stupid.
    int selection = _profileChoice->GetSelection();
    if(_profileManager.setProfile(selection)) {
        _massManager.emplace(_profileManager.profileDirectory(),
                             _profileManager.currentProfile()->steamId(),
                             _profileManager.currentProfile()->type() == ProfileType::Demo);

        _watcher.AddTree(wxFileName{Utility::Directory::toNativeSeparators(_massManager->saveDirectory()), wxPATH_WIN},
                         wxFSW_EVENT_CREATE|wxFSW_EVENT_DELETE|wxFSW_EVENT_MODIFY|wxFSW_EVENT_RENAME,
                         wxString::Format("%s*%s.sav", _profileManager.currentProfile()->type() == ProfileType::Demo ? "Demo" : "", _profileManager.currentProfile()->steamId()));

        updateProfileStats();
        refreshListView();
    }
}

void EvtMainFrame::backupSelectedProfileEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Backup failed:\n\n";

    wxString current_timestamp = wxDateTime::Now().Format("%Y-%m-%d_%H-%M-%S");

    wxFileDialog save_dialog{this, "Choose output location", _massManager->saveDirectory(),
                             wxString::Format("backup_%s%s_%s_%s.zip",
                                              _profileManager.currentProfile()->type() == ProfileType::Demo ? "demo_" : "",
                                              _profileManager.currentProfile()->companyName(),
                                              _profileManager.currentProfile()->steamId(),
                                              current_timestamp),
                             "Zip archive (*.zip)|*.zip",
                             wxFD_SAVE|wxFD_OVERWRITE_PROMPT};

    if(save_dialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    if(!_profileManager.currentProfile()->backup(save_dialog.GetPath().ToStdString())) {
        errorMessage(error_prefix + _massManager->lastError());
    }
}

void EvtMainFrame::companyRenameEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Rename failed:\n\n";

    EvtNameChangeDialog dialog{this};
    dialog.setName(_profileManager.currentProfile()->companyName());
    int result = dialog.ShowModal();

    if(result == wxID_OK) {
        if(_unsafeMode == false) {
            switch(_mbManager.gameState()) {
                case GameState::Unknown:
                    errorMessage(error_prefix + "For security reasons, renaming the company is disabled if the game's status is unknown.");
                    break;
                case GameState::NotRunning:
                    if(!_profileManager.currentProfile()->renameCompany(dialog.getName())) {
                        errorMessage(error_prefix + _profileManager.currentProfile()->lastError());
                    }
                    else {
                        _profileChoice->SetString(_profileChoice->GetCurrentSelection(),
                                                  wxString::Format("%s%s",
                                                                   _profileManager.currentProfile()->companyName(),
                                                                   _profileManager.currentProfile()->type() == ProfileType::Demo ? " (Demo)" : ""));
                    }
                    break;
                case GameState::Running:
                    errorMessage(error_prefix + "Renaming the company is disabled while the game is running.");
                    break;
            }
        }
        else {
            if(!_profileManager.currentProfile()->renameCompany(dialog.getName())) {
                errorMessage(error_prefix + _profileManager.currentProfile()->lastError());
            }
            else {
                _profileChoice->SetString(_profileChoice->GetCurrentSelection(),
                                          wxString::Format("%s%s",
                                                           _profileManager.currentProfile()->companyName(),
                                                           _profileManager.currentProfile()->type() == ProfileType::Demo ? " (Demo)" : ""));
            }
        }
    }
}

void EvtMainFrame::creditsEditEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Credits change failed:\n\n";

    if(_unsafeMode == true || _mbManager.gameState() == GameState::NotRunning) {
        long number = wxGetNumberFromUser("Please enter a number of credits between 0 and 2 000 000 000 included:", "Credits:", "Input credits",
                                          _profileManager.currentProfile()->credits(), 0, 2000000000, this);
        if(number == -1 || number == _profileManager.currentProfile()->credits()) {
            return;
        }

        if(!_profileManager.currentProfile()->setCredits(number)) {
            errorMessage(error_prefix + _profileManager.currentProfile()->lastError());
        }

        updateProfileStats();
    }
    else if(_mbManager.gameState() == GameState::Unknown) {
        errorMessage(error_prefix + "For security reasons, changing credits is disabled if the game's status is unknown.");
    }
    else if(_mbManager.gameState() == GameState::Running) {
        errorMessage(error_prefix + "For security reasons, changing credits is disabled if the game is running.");
    }
}

void EvtMainFrame::storyProgressSelectionEvent(wxCommandEvent& event) {
    const static std::string error_prefix = "StoryProgress change failed:\n\n";

    std::int32_t story_progress = event.GetId() ^ (-10000);

    if(_unsafeMode == false) {
        switch(_mbManager.gameState()) {
            case GameState::Unknown:
                errorMessage(error_prefix + "For security reasons, changing the story progression is disabled if the game's status is unknown.");
                break;
            case GameState::NotRunning:
                if(!_profileManager.currentProfile()->setStoryProgress(story_progress)) {
                    errorMessage(error_prefix + _profileManager.currentProfile()->lastError());
                }
                break;
            case GameState::Running:
                errorMessage(error_prefix + "Changing the story progression is disabled while the game is running.");
                break;
        }
    }
    else if(!_profileManager.currentProfile()->setStoryProgress(story_progress)) {
        errorMessage(error_prefix + _profileManager.currentProfile()->lastError());
    }

    updateProfileStats();
}

void EvtMainFrame::openStoryProgressMenuEvent(wxCommandEvent&) {
    PopupMenu(_storyProgressSelectionMenu.get());
}

void EvtMainFrame::inventoryChangeEvent(wxPropertyGridEvent& event) {
    const static std::string error_prefix = "Inventory change failed:\n\n";

    std::int32_t value = event.GetPropertyValue().GetInteger();
    Profile* profile = _profileManager.currentProfile();

    if(value > 1000000 || value < 0) {
        event.SetValidationFailureMessage(error_prefix + "The value must not be higher than 1 million or lower than 0");
        event.Veto();
        return;
    }

    if(_unsafeMode == true || _mbManager.gameState() == GameState::NotRunning) {
        wxPGProperty* prop = event.GetProperty();
        bool success = false;

        if(prop == _verseSteel) { success = profile->setVerseSteel(value); }
        else if(prop == _undinium) { success = profile->setUndinium(value); }
        else if(prop == _necriumAlloy) { success = profile->setNecriumAlloy(value); }
        else if(prop == _lunarite) { success = profile->setLunarite(value); }
        else if(prop == _asterite) { success = profile->setAsterite(value); }
        else if(prop == _ednil) { success = profile->setEdnil(value); }
        else if(prop == _nuflalt) { success = profile->setNuflalt(value); }
        else if(prop == _aurelene) { success = profile->setAurelene(value); }
        else if(prop == _soldus) { success = profile->setSoldus(value); }
        else if(prop == _synthesizedN) { success = profile->setSynthesizedN(value); }
        else if(prop == _alcarbonite) { success = profile->setAlcarbonite(value); }
        else if(prop == _keriphene) { success = profile->setKeriphene(value); }
        else if(prop == _nitinolCM) { success = profile->setNitinolCM(value); }
        else if(prop == _quarkium) { success = profile->setQuarkium(value); }
        else if(prop == _alterene) { success = profile->setAlterene(value); }

        else if(prop == _mixedComposition) { success = profile->setMixedComposition(value); }
        else if(prop == _voidResidue) { success = profile->setVoidResidue(value); }
        else if(prop == _muscularConstruction) { success = profile->setMuscularConstruction(value); }
        else if(prop == _mineralExoskeletology) { success = profile->setMineralExoskeletology(value); }
        else if(prop == _carbonizedSkin) { success = profile->setCarbonizedSkin(value); }

        if(!success) {
            event.SetValidationFailureMessage(error_prefix + profile->lastError());
            event.Veto();
        }
    }
    else if(_mbManager.gameState() == GameState::Unknown) {
        event.SetValidationFailureMessage(error_prefix + "For security reasons, changing the inventory is disabled if the game's status is unknown.");
        event.Veto();
    }
    else if(_mbManager.gameState() == GameState::Running) {
        event.SetValidationFailureMessage(error_prefix + "For security reasons, changing the inventory is disabled if the game is running.");
        event.Veto();
    }
}

void EvtMainFrame::importMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Importing failed:\n\n";

    long selected_hangar = _installedListView->GetFirstSelected();
    MassState mass_state = _massManager->massState(selected_hangar);

    int staged_selection = _stagingList->GetSelection();

    int confirmation;

    if(mass_state == MassState::Valid) {
        confirmation = wxMessageBox(wxString::Format("Hangar %.2d is already occupied by the M.A.S.S. named \"%s\". Are you sure you want to import the M.A.S.S. named \"%s\" to this hangar ?",
                                      selected_hangar + 1, _massManager->massName(selected_hangar), _massManager->stagedMassName(staged_selection)),
                                    "Question", wxYES_NO|wxCENTRE|wxICON_QUESTION, this);
    }
    else {
        confirmation = wxMessageBox(wxString::Format("Are you sure you want to import the M.A.S.S. named \"%s\" to hangar %.2d ?", _massManager->stagedMassName(staged_selection), selected_hangar + 1),
                                    "Question", wxYES_NO|wxCENTRE|wxICON_QUESTION, this);
    }

    if(confirmation == wxNO) {
        return;
    }

    if(_unsafeMode == false) {
        switch(_mbManager.gameState()) {
            case GameState::Unknown:
                errorMessage(error_prefix + "For security reasons, importing is disabled if the game's status is unknown.");
                break;
            case GameState::NotRunning:
                if(!_massManager->importMass(staged_selection, selected_hangar)) {
                    errorMessage(error_prefix + _massManager->lastError());
                }
                break;
            case GameState::Running:
                errorMessage(error_prefix + "Importing a M.A.S.S. is disabled while the game is running.");
                break;
        }
    }
    else {
        if(!_massManager->importMass(staged_selection, selected_hangar)) {
            errorMessage(error_prefix + _massManager->lastError());
        }
    }
}

void EvtMainFrame::exportMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Export failed:\n\n";

    long slot = _installedListView->GetFirstSelected();

    if(!_massManager->exportMass(slot)) {
        errorMessage(error_prefix + _massManager->lastError());
    }
}

void EvtMainFrame::moveMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Move failed:\n\n";

    long source_slot = _installedListView->GetFirstSelected();

    long choice = wxGetNumberFromUser(wxString::Format("Which hangar do you want to move the M.A.S.S. named \"%s\" to ?\nNotes:\n"
                                                       "- If the destination hangar is the same as the source, nothing will happen.\n"
                                                       "- If the destination already contains a M.A.S.S., the two will be swapped.\n"
                                                       "- If the destination contains invalid data, it will be cleared first.",
                                                       _massManager->massName(source_slot)),
                                      "Slot", "Choose a slot", source_slot + 1, 1, 32, this);

    if(choice == -1 || choice == source_slot) {
        return;
    }

    if(_unsafeMode == false) {
        switch(_mbManager.gameState()) {
            case GameState::Unknown:
                errorMessage(error_prefix + "For security reasons, moving a M.A.S.S. is disabled if the game's status is unknown.");
                break;
            case GameState::NotRunning:
                if(!_massManager->moveMass(source_slot, choice - 1)) {
                    errorMessage(error_prefix + _massManager->lastError());
                }
                break;
            case GameState::Running:
                errorMessage(error_prefix + "Moving a M.A.S.S. is disabled while the game is running.");
                break;
        }
    }
    else {
        if(!_massManager->moveMass(source_slot, choice - 1)) {
            errorMessage(error_prefix + _massManager->lastError());
        }
    }
}

void EvtMainFrame::deleteMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Deletion failed:\n\n";

    if(wxMessageBox(wxString::Format("Are you sure you want to delete the data in hangar %.2d ? This operation cannot be undone.", _installedListView->GetFirstSelected() + 1),
                    "Are you sure ?", wxYES_NO|wxCENTRE|wxICON_QUESTION, this) == wxNO) {
        return;
    }

    if(_unsafeMode == false) {
        switch(_mbManager.gameState()) {
            case GameState::Unknown:
                errorMessage(error_prefix + "For security reasons, deleting a M.A.S.S. is disabled if the game's status is unknown.");
                break;
            case GameState::NotRunning:
                if(!_massManager->deleteMass(_installedListView->GetFirstSelected())) {
                    errorMessage(error_prefix + _massManager->lastError());
                }
                break;
            case GameState::Running:
                errorMessage(error_prefix + "Deleting a M.A.S.S. is disabled while the game is running.");
                break;
        }
    }
    else {
        if(!_massManager->deleteMass(_installedListView->GetFirstSelected())) {
            errorMessage(error_prefix + _massManager->lastError());
        }
    }
}

void EvtMainFrame::renameMassEvent(wxCommandEvent&) {
    const static std::string error_prefix = "Rename failed:\n\n";

    EvtNameChangeDialog dialog{this};
    dialog.setName(_massManager->massName(_installedListView->GetFirstSelected()));
    int result = dialog.ShowModal();

    if(result == wxID_OK) {
        if(_unsafeMode == false) {
            switch(_mbManager.gameState()) {
                case GameState::Unknown:
                    errorMessage(error_prefix + "For security reasons, renaming a M.A.S.S. is disabled if the game's status is unknown.");
                    break;
                case GameState::NotRunning:
                    if(!_massManager->renameMass(_installedListView->GetFirstSelected(), dialog.getName())) {
                        errorMessage(error_prefix + _massManager->lastError());
                    }
                    break;
                case GameState::Running:
                    errorMessage(error_prefix + "Renaming a M.A.S.S. is disabled while the game is running.");
                    break;
            }
        }
        else {
            if(!_massManager->renameMass(_installedListView->GetFirstSelected(), dialog.getName())) {
                errorMessage(error_prefix + _massManager->lastError());
            }
        }
    }
}

void EvtMainFrame::openSaveDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(_massManager->saveDirectory()));
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
        _massManager->deleteStagedMass(selection);
    }
}

void EvtMainFrame::openStagingDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(_massManager->stagingAreaDirectory()));
}

void EvtMainFrame::installedSelectionEvent(wxListEvent&) {
    updateCommandsState();
}

void EvtMainFrame::listColumnDragEvent(wxListEvent& event) {
    event.Veto();
}

void EvtMainFrame::openScreenshotDirEvent(wxCommandEvent&) {
    wxExecute("explorer.exe " + Utility::Directory::toNativeSeparators(Utility::Directory::join(_profileManager.profileDirectory(), "../Screenshots/WindowsNoEditor")));
}

void EvtMainFrame::fileUpdateEvent(wxFileSystemWatcherEvent& event) {
    int event_type = event.GetChangeType();
    wxString event_file = event.GetPath().GetFullName();

    if(event_type == wxFSW_EVENT_MODIFY && _lastWatcherEventType == wxFSW_EVENT_RENAME) {
        _lastWatcherEventType = event_type;
        return;
    }

    wxMilliSleep(100);

    wxString event_path = event.GetPath().GetPath(wxPATH_GET_VOLUME, wxPATH_WIN);

    if(event_path == Utility::Directory::toNativeSeparators(_massManager->saveDirectory())) {
        saveFileEventHandler(event_type, event_file, event);
    }
    else if(_massManager && event_path == Utility::Directory::toNativeSeparators(_massManager->stagingAreaDirectory())) {
        stagingFileEventHandler(event_type, event_file, event);
    }

    _lastWatcherEventType = event_type;

    updateCommandsState();
}

void EvtMainFrame::gameCheckTimerEvent(wxTimerEvent&) {
    isGameRunning();
}

void EvtMainFrame::unsafeCheckboxEvent(wxCommandEvent& event) {
    if(event.IsChecked() == true) {
        int confirmation = wxMessageBox("Are you sure you want to enable unsafe mode ?\n\n"
                                        "Unsafe mode will allow you to perform changes even while the game is running, which can result in weird behaviour or even data corruption.",
                                        "Question", wxYES_NO|wxCENTRE|wxICON_WARNING, this);
        if(confirmation == wxYES) {
            _unsafeMode = true;
        }
        else {
            _unsafeCheckbox->SetValue(false);
        }
    }
    else {
        _unsafeMode = false;
    }

    updateCommandsState();
}

void EvtMainFrame::saveFileEventHandler(int event_type, const wxString& event_file, const wxFileSystemWatcherEvent& event) {
    wxRegEx regex;

    switch (event_type) {
        case wxFSW_EVENT_CREATE:
        case wxFSW_EVENT_DELETE:
            regex.Compile(wxString::Format("%sUnit([0-3][0-9])%s\\.sav", _profileManager.currentProfile()->type() == ProfileType::Demo ? "Demo" : "", _profileManager.currentProfile()->steamId()), wxRE_ADVANCED);
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
            if(event_file == _profileManager.currentProfile()->filename()) {
                updateProfileStats();
                getActiveSlot();
            }
            else {
                regex.Compile(wxString::Format("%sUnit([0-3][0-9])%s\\.sav", _profileManager.currentProfile()->type() == ProfileType::Demo ? "Demo" : "", _profileManager.currentProfile()->steamId()), wxRE_ADVANCED);
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
            if(regex.Compile(wxString::Format("%sUnit([0-3][0-9])%s\\.sav\\.tmp", _profileManager.currentProfile()->type() == ProfileType::Demo ? "Demo" : "", _profileManager.currentProfile()->steamId()), wxRE_ADVANCED), regex.Matches(new_name)) {
                if(regex.GetMatch(new_name, 1).ToLong(&slot) && slot >= 0 && slot < 32) {
                    refreshHangar(slot);
                }
            }
            else if(regex.Compile(wxString::Format("%sUnit([0-3][0-9])%s\\.sav", _profileManager.currentProfile()->type() == ProfileType::Demo ? "Demo" : "", _profileManager.currentProfile()->steamId()), wxRE_ADVANCED), regex.Matches(new_name)) {
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
            index = _massManager->updateStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->Insert(wxString::Format("%s (%s)", _massManager->stagedMassName(index), event_file), index);
            }
            break;
        case wxFSW_EVENT_DELETE:
            index = _massManager->removeStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->Delete(index);
            }
            break;
        case wxFSW_EVENT_MODIFY:
            index = _massManager->updateStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->SetString(index, wxString::Format("%s (%s)", _massManager->stagedMassName(index), event_file));
            }
            break;
        case wxFSW_EVENT_RENAME:
            index = _massManager->removeStagedMass(event_file.ToUTF8().data());
            if(index != -1) {
                _stagingList->Delete(index);
            }
            index = _massManager->updateStagedMass(event.GetNewPath().GetFullName().ToUTF8().data());
            if(index != -1) {
                _stagingList->Insert(wxString::Format("%s (%s)", _massManager->stagedMassName(index), event.GetNewPath().GetFullName()), index);
            }
            break;
    }
}

void EvtMainFrame::updateProfileStats() {
    Profile* current_profile = _profileManager.currentProfile();
    _companyName->SetLabel(current_profile->getCompanyName());

    _credits->SetLabel(wxString::Format("%i", current_profile->getCredits()));

    if(story_progress_map.find(current_profile->getStoryProgress()) != story_progress_map.end()) {
        _storyProgress->SetLabel(story_progress_map.at(current_profile->storyProgress()));
    }
    else {
        _storyProgress->SetLabel(wxString::Format("0x%X", current_profile->storyProgress()));
    }

    if(mission_id_map.find(current_profile->getLastMissionId()) != mission_id_map.end()) {
        _lastMissionId->SetLabel(mission_id_map.at(current_profile->lastMissionId()));
    }
    else {
        _lastMissionId->SetLabel(wxString::Format("0x%X", current_profile->lastMissionId()));
    }

    _verseSteel->SetValueFromInt(current_profile->getVerseSteel());
    _undinium->SetValueFromInt(current_profile->getUndinium());
    _necriumAlloy->SetValueFromInt(current_profile->getNecriumAlloy());
    _lunarite->SetValueFromInt(current_profile->getLunarite());
    _asterite->SetValueFromInt(current_profile->getAsterite());
    _ednil->SetValueFromInt(current_profile->getEdnil());
    _nuflalt->SetValueFromInt(current_profile->getNuflalt());
    _aurelene->SetValueFromInt(current_profile->getAurelene());
    _soldus->SetValueFromInt(current_profile->getSoldus());
    _synthesizedN->SetValueFromInt(current_profile->getSynthesizedN());
    _alcarbonite->SetValueFromInt(current_profile->getAlcarbonite());
    _keriphene->SetValueFromInt(current_profile->getKeriphene());
    _nitinolCM->SetValueFromInt(current_profile->getNitinolCM());
    _quarkium->SetValueFromInt(current_profile->getQuarkium());
    _alterene->SetValueFromInt(current_profile->getAlterene());

    _mixedComposition->SetValueFromInt(current_profile->getMixedComposition());
    _voidResidue->SetValueFromInt(current_profile->getVoidResidue());
    _muscularConstruction->SetValueFromInt(current_profile->getMuscularConstruction());
    _mineralExoskeletology->SetValueFromInt(current_profile->getMineralExoskeletology());
    _carbonizedSkin->SetValueFromInt(current_profile->getCarbonizedSkin());

    updateCommandsState();
}

void EvtMainFrame::initStoryProgressMenu() {
    _storyProgressSelectionMenu.emplace();

    if(!_storyProgressSelectionMenu) {
        errorMessage("Error initialising the story progress selection menu.");
        this->Destroy();
        return;
    }

    wxMenu* submenu = nullptr;

    for(const auto& pair : story_progress_map) {
        if(std::strncmp(pair.second + 10, "start", 5) == 0) {
            submenu = new wxMenu();

            if(!submenu) {
                errorMessage("Error initialising the story progress selection menu.");
                this->Destroy();
                return;
            }

            _storyProgressSelectionMenu->Append(wxID_ANY, wxString{pair.second, 9}, submenu);

            wxMenuItem* item = submenu->Append(pair.first ^ (-10000), "Chapter start");

            if(!item) {
                errorMessage("Error initialising the story progress selection menu.");
                this->Destroy();
                return;
            }
        }
        else {
            if(!submenu) {
                errorMessage("Error initialising the story progress selection menu.");
                this->Destroy();
                return;
            }

            wxMenuItem* item = submenu->Append(pair.first ^ (-10000), wxString{pair.second + 12});

            if(!item) {
                errorMessage("Error initialising the story progress selection menu.");
                this->Destroy();
                return;
            }
        }
    }

    _storyProgressSelectionMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(EvtMainFrame::storyProgressSelectionEvent), nullptr, this);
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
    _gameStatus->SetLabel("checking...");
    _gameStatus->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

    GameState state = _mbManager.checkGameState();

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
    auto slot = _profileManager.currentProfile()->activeFrameSlot();

    if(slot != -1) {
        wxFont tmp_font = _installedListView->GetItemFont(slot);
        tmp_font.SetWeight(wxFONTWEIGHT_NORMAL);
        _installedListView->SetItemFont(slot, tmp_font);
    }

    slot = _profileManager.currentProfile()->getActiveFrameSlot();

    if(slot != -1) {
        _installedListView->SetItemFont(slot, _installedListView->GetItemFont(slot).Bold());
    }
}

void EvtMainFrame::updateCommandsState() {
    long selection = _installedListView->GetFirstSelected();
    int staged_selection = _stagingList->GetSelection();
    GameState game_state = _mbManager.gameState();
    MassState mass_state = _massManager->massState(selection);

    _companyRenameButton->Enable(_unsafeMode == true || game_state == GameState::NotRunning);
    _creditsEditButton->Enable(_unsafeMode == true || game_state == GameState::NotRunning);
    _storyProgressChangeButton->Enable(_unsafeMode == true || game_state == GameState::NotRunning);

    wxPropertyGridConstIterator it = _researchInventoryPropGrid->GetIterator(wxPG_ITERATE_NORMAL);
    while(!it.AtEnd()) {
        if(it.GetProperty()->IsCategory() == false) {
            it.GetProperty()->Enable(it.GetProperty()->GetValue().GetInteger() != -1 && (_unsafeMode == true || game_state == GameState::NotRunning));
        }
        it.Next();
    }

    _importButton->Enable(selection != -1 && staged_selection != -1 && (_unsafeMode == true || game_state == GameState::NotRunning));
    _exportButton->Enable(selection != -1);
    _moveButton->Enable(selection != -1 && (_unsafeMode == true || game_state == GameState::NotRunning) && mass_state == MassState::Valid);
    _deleteButton->Enable(selection != -1 && (_unsafeMode == true || game_state == GameState::NotRunning) && mass_state != MassState::Empty);
    _renameButton->Enable(selection != -1 && (_unsafeMode == true || game_state == GameState::NotRunning) && mass_state == MassState::Valid);
    _deleteStagedButton->Enable(staged_selection != -1);
}

void EvtMainFrame::refreshHangar(int slot) {
    if(slot < 0 && slot >= 32) {
        return;
    }

    _massManager->refreshHangar(slot);

    switch(_massManager->massState(slot)) {
        case MassState::Empty:
            _installedListView->SetItem(slot, 1, "<Empty>");
            break;
        case MassState::Invalid:
            _installedListView->SetItem(slot, 1, "<Invalid>");
            break;
        case MassState::Valid:
            _installedListView->SetItem(slot, 1, _massManager->massName(slot));
            break;
    }
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
