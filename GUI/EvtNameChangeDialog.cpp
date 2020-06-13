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

#include <wx/msgdlg.h>

#include "EvtNameChangeDialog.h"

EvtNameChangeDialog::EvtNameChangeDialog(wxWindow* parent): NameChangeDialog(parent) {
    wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST, nullptr);

    validator.SuppressBellOnError(false);
    validator.SetCharIncludes("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789- ");

    _nameInput->SetValidator(validator);
}

auto EvtNameChangeDialog::getName() -> std::string {
    std::string name = _nameInput->GetValue().ToStdString();
    return name;
}

void EvtNameChangeDialog::setName(const std::string& name) {
    _nameInput->SetValue(wxString::FromUTF8(name.c_str()));
}

void EvtNameChangeDialog::textEditEvent(wxCommandEvent&) {
    const wxString value = _nameInput->GetValue();

    if(value.length() >= 6 && value.length() <= 32) {
        _lengthBitmap->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_BUTTON));
    }
    else {
        _lengthBitmap->SetBitmap(wxArtProvider::GetBitmap(wxART_CROSS_MARK, wxART_BUTTON));
    }

    _nameLength->SetLabel(wxString::Format("%u", value.length()));

    if(_nameInput->Validate() == true) {
        _charsBitmap->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_BUTTON));
    }
    else {
        _charsBitmap->SetBitmap(wxArtProvider::GetBitmap(wxART_CROSS_MARK, wxART_BUTTON));
    }

    if(!(value.StartsWith(" ") || value.EndsWith(" "))) {
        _whitespaceAtBeginOrEndBitmap->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_BUTTON));
    }
    else {
        _whitespaceAtBeginOrEndBitmap->SetBitmap(wxArtProvider::GetBitmap(wxART_CROSS_MARK, wxART_BUTTON));
    }

    if(validateName() == true) {
        _okButton->Enable();
    }
    else {
        _okButton->Disable();
    }
}

void EvtNameChangeDialog::okButtonEvent(wxCommandEvent&) {
    if(validateName() == false) {
        wxMessageBox("The new name couldn't be validated.", "Error", wxOK|wxCENTRE|wxICON_ERROR, this);
        return;
    }

    EndModal(wxID_OK);
}

void EvtNameChangeDialog::cancelButtonEvent(wxCommandEvent&) {
    EndModal(wxID_CANCEL);
}

auto EvtNameChangeDialog::validateName() -> bool {
    const wxString value = _nameInput->GetValue();
    return ((value.length() >= 6 && value.length() <= 32) && _nameInput->Validate() && !(value.StartsWith(" ") || value.EndsWith(" ")));
}
