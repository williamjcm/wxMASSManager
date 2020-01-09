///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainFrame.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,600 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerMainUi;
	bSizerMainUi = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sbSizerInstalled;
	sbSizerInstalled = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Installed M.A.S.S.es") ), wxVERTICAL );

	_installedListView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES);
	_installedListView->AppendColumn("Hangar", wxLIST_FORMAT_LEFT);
	_installedListView->AppendColumn("M.A.S.S. name", wxLIST_FORMAT_LEFT);
	sbSizerInstalled->Add( _installedListView, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerInstalledButtons;
	bSizerInstalledButtons = new wxBoxSizer( wxHORIZONTAL );

	_importButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Import"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerInstalledButtons->Add( _importButton, 1, wxALL|wxEXPAND, 5 );

	_moveButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Move"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerInstalledButtons->Add( _moveButton, 1, wxALL|wxEXPAND, 5 );

	_deleteButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerInstalledButtons->Add( _deleteButton, 1, wxALL|wxEXPAND, 5 );


	sbSizerInstalled->Add( bSizerInstalledButtons, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerSecondRow;
	bSizerSecondRow = new wxBoxSizer( wxHORIZONTAL );


	bSizerSecondRow->Add( 0, 0, 1, wxEXPAND, 5 );

	_openSaveDirButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Open save directory"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerSecondRow->Add( _openSaveDirButton, 1, wxALL|wxEXPAND, 5 );


	bSizerSecondRow->Add( 0, 0, 1, wxEXPAND, 5 );


	sbSizerInstalled->Add( bSizerSecondRow, 0, wxEXPAND, 5 );


	bSizerMainUi->Add( sbSizerInstalled, 1, wxEXPAND|wxALL, 5 );


	bSizerMain->Add( bSizerMainUi, 1, wxEXPAND, 5 );

	_riskLabel = new wxStaticText( this, wxID_ANY, wxT("USE THIS TOOL AT YOUR OWN RISK!"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	_riskLabel->Wrap( -1 );
	_riskLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	_riskLabel->SetForegroundColour( wxColour( 255, 0, 0 ) );

	bSizerMain->Add( _riskLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	_aboutText = new wxStaticText( this, wxID_ANY, wxT("This version of the application was tested on M.A.S.S. Builder early access version 0.2.4.\nIt may or may not work with other versions of the game.\nMade for the M.A.S.S. Builder community by Guillaume Jacquemin.\nhttps://github.com/williamjcm/wxMASSManager"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	_aboutText->Wrap( -1 );
	bSizerMain->Add( _aboutText, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	_refreshTimer.SetOwner( this, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	_importButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::importEvent ), NULL, this );
	_moveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::moveEvent ), NULL, this );
	_deleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteEvent ), NULL, this );
	_openSaveDirButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openSaveDirEvent ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::timerEvent ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	_importButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::importEvent ), NULL, this );
	_moveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::moveEvent ), NULL, this );
	_deleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteEvent ), NULL, this );
	_openSaveDirButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openSaveDirEvent ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::timerEvent ) );

}
