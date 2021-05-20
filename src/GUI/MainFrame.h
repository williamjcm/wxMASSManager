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
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
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
		wxStaticText* _profileLabel;
		wxChoice* _profileChoice;
		wxButton* _backupSelectedButton;
		wxButton* _openScreenshotDirButton;
		wxCheckBox* _unsafeCheckbox;
		wxNotebook* _managerNotebook;
		wxPanel* _profilePanel;
		wxStaticText* _companyNameLabel;
		wxStaticText* _companyName;
		wxStaticText* _creditsLabel;
		wxStaticText* _credits;
		wxStaticText* _storyProgressLabel;
		wxStaticText* _storyProgress;
		wxStaticText* _lastMissionIdLabel;
		wxStaticText* _lastMissionId;
		wxButton* _companyRenameButton;
		wxButton* _creditsEditButton;
		wxButton* _storyProgressChangeButton;
		wxPropertyGrid* _researchInventoryPropGrid;
		wxPGProperty* _materialsCategory;
		wxPGProperty* _verseSteel;
		wxPGProperty* _undinium;
		wxPGProperty* _necriumAlloy;
		wxPGProperty* _lunarite;
		wxPGProperty* _asterite;
		wxPGProperty* _ednil;
		wxPGProperty* _nuflalt;
		wxPGProperty* _aurelene;
		wxPGProperty* _soldus;
		wxPGProperty* _synthesizedN;
		wxPGProperty* _alcarbonite;
		wxPGProperty* _keriphene;
		wxPGProperty* _nitinolCM;
		wxPGProperty* _quarkium;
		wxPGProperty* _alterene;
		wxPGProperty* _quarkDataCategory;
		wxPGProperty* _mixedComposition;
		wxPGProperty* _voidResidue;
		wxPGProperty* _muscularConstruction;
		wxPGProperty* _mineralExoskeletology;
		wxPGProperty* _carbonizedSkin;
		wxPanel* _massPanel;
		wxListView* _installedListView;
		wxButton* _moveButton;
		wxButton* _deleteButton;
		wxButton* _renameButton;
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
		virtual void profileSelectionEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void backupSelectedProfileEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openScreenshotDirEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void unsafeCheckboxEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void companyRenameEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void creditsEditEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openStoryProgressMenuEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void inventoryChangeEvent( wxPropertyGridEvent& event ) { event.Skip(); }
		virtual void moveMassEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void deleteMassEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void renameMassEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openSaveDirEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void importMassEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void exportMassEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void stagingSelectionEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void deleteStagedEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void openStagingDirEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void gameCheckTimerEvent( wxTimerEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("M.A.S.S. Builder Save Tool 2.3.0"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );

		~MainFrame();

};

