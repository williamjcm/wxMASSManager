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
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/panel.h>
#include <wx/simplebook.h>
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
		wxSimplebook* _managerBook;
		wxPanel* _massPanel;
		wxListView* _installedListView;
		wxButton* _moveButton;
		wxButton* _deleteButton;
		wxButton* _renameButton;
		wxButton* _zipButton;
		wxButton* _openSaveDirButton;
		wxButton* _importButton;
		wxButton* _exportButton;
		wxListBox* _stagingList;
		wxButton* _deleteStagedButton;
		wxButton* _stagingAreaButton;
		wxStaticText* _riskLabel;
		wxStaticText* _gameStatusLabel;
		wxStaticText* _gameStatus;
		wxStaticText* _aboutText;
		wxHyperlinkCtrl* _githubLink;
		wxTimer _gameCheckTimer;

		// Virtual event handlers, overide them in your derived class
		virtual void moveEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void deleteEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void renameMassEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void backupEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openSaveDirEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void importEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void exportEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void stagingSelectionEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void deleteStagedEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void stagingButtonEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void gameCheckTimerEvent( wxTimerEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("M.A.S.S. Manager 1.0.9"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );

		~MainFrame();

};

