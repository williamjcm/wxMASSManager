///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/timer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	private:

	protected:
		wxListView* _installedListView;
		wxButton* _importButton;
		wxButton* _moveButton;
		wxButton* _deleteButton;
		wxButton* _openSaveDirButton;
		wxStaticText* _riskLabel;
		wxStaticText* _aboutText;
		wxTimer _refreshTimer;

		// Virtual event handlers, overide them in your derived class
		virtual void importEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void moveEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void deleteEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openSaveDirEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void timerEvent( wxTimerEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("M.A.S.S. Manager 0.2.0-pre"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );

		~MainFrame();

};

