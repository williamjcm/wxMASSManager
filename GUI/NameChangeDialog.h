///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NameChangeDialog
///////////////////////////////////////////////////////////////////////////////
class NameChangeDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* _nameLabel;
		wxTextCtrl* _nameInput;
		wxStaticText* _conditionsLabel;
		wxStaticBitmap* _lengthBitmap;
		wxStaticText* _lengthLabel;
		wxStaticText* _nameLength;
		wxStaticBitmap* _charsBitmap;
		wxStaticText* _charsLabel;
		wxStaticBitmap* _whitespaceAtBeginOrEndBitmap;
		wxStaticText* _whitespaceAtBeginOrEndLabel;
		wxButton* _okButton;
		wxButton* _cancelButton;

		// Virtual event handlers, overide them in your derived class
		virtual void textEditEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void okButtonEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancelButtonEvent( wxCommandEvent& event ) { event.Skip(); }


	public:

		NameChangeDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Name change"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~NameChangeDialog();

};

