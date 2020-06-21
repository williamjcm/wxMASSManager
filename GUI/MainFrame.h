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
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
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
		wxPanel* _mainPanel;
		wxNotebook* _managerNotebook;
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
		wxPanel* _screenshotsPanel;
		wxListCtrl* _screenshotsList;
		wxRadioButton* _nameRadio;
		wxRadioButton* _creationDateRadio;
		wxRadioButton* _ascendingRadio;
		wxRadioButton* _descendingRadio;
		wxButton* _viewScreenshotButton;
		wxButton* _deleteScreenshotButton;
		wxButton* _screenshotDirButton;
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
		virtual void viewScreenshotEvent( wxListEvent& event ) { event.Skip(); }
		virtual void screenshotListSelectionEvent( wxListEvent& event ) { event.Skip(); }
		virtual void screenshotFilenameSortingEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void screenshotCreationDateSortingEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void screenshotAscendingSortingEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void screenshotDescendingSortingEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void viewScreenshotEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void deleteScreenshotEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openScreenshotDirEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void gameCheckTimerEvent( wxTimerEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("M.A.S.S. Manager 1.1.1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );

		~MainFrame();

};

