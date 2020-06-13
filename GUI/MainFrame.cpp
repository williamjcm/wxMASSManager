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

	wxStaticBoxSizer* sbSizerButtons;
	sbSizerButtons = new wxStaticBoxSizer( new wxStaticBox( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Hangar actions") ), wxHORIZONTAL );

	_moveButton = new wxButton( sbSizerButtons->GetStaticBox(), wxID_ANY, wxT("Move"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerButtons->Add( _moveButton, 1, wxALL|wxEXPAND, 5 );

	_deleteButton = new wxButton( sbSizerButtons->GetStaticBox(), wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerButtons->Add( _deleteButton, 1, wxALL|wxEXPAND, 5 );

	_renameButton = new wxButton( sbSizerButtons->GetStaticBox(), wxID_ANY, wxT("Rename"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerButtons->Add( _renameButton, 1, wxALL|wxEXPAND, 5 );


	sbSizerInstalled->Add( sbSizerButtons, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerSecondRow;
	bSizerSecondRow = new wxBoxSizer( wxHORIZONTAL );

	_zipButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Backup save files"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerSecondRow->Add( _zipButton, 2, wxALL|wxEXPAND, 5 );

	_openSaveDirButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Open save directory"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerSecondRow->Add( _openSaveDirButton, 2, wxEXPAND|wxALL, 5 );


	sbSizerInstalled->Add( bSizerSecondRow, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );


	bSizerMainUi->Add( sbSizerInstalled, 1, wxEXPAND|wxALL, 5 );

	wxBoxSizer* bSizerImportExport;
	bSizerImportExport = new wxBoxSizer( wxVERTICAL );

	_importButton = new wxButton( this, wxID_ANY, wxT("Import"), wxDefaultPosition, wxDefaultSize, 0 );

	_importButton->SetBitmap( wxArtProvider::GetBitmap( wxART_GO_BACK, wxART_BUTTON ) );
	bSizerImportExport->Add( _importButton, 1, wxALL|wxEXPAND, 5 );

	_exportButton = new wxButton( this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );

	_exportButton->SetBitmap( wxArtProvider::GetBitmap( wxART_GO_FORWARD, wxART_BUTTON ) );
	_exportButton->SetBitmapPosition( wxRIGHT );
	bSizerImportExport->Add( _exportButton, 0, wxALL|wxEXPAND, 5 );


	bSizerMainUi->Add( bSizerImportExport, 0, wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticBoxSizer* sbSizerStagingArea;
	sbSizerStagingArea = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Staging area") ), wxVERTICAL );

	_stagingList = new wxListBox( sbSizerStagingArea->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB|wxLB_SINGLE );
	sbSizerStagingArea->Add( _stagingList, 1, wxALL|wxEXPAND, 5 );

	_deleteStagedButton = new wxButton( sbSizerStagingArea->GetStaticBox(), wxID_ANY, wxT("Delete staged M.A.S.S."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerStagingArea->Add( _deleteStagedButton, 0, wxALL|wxEXPAND, 5 );

	_stagingAreaButton = new wxButton( sbSizerStagingArea->GetStaticBox(), wxID_ANY, wxT("Open staging area directory"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerStagingArea->Add( _stagingAreaButton, 0, wxALL|wxEXPAND, 5 );


	bSizerMainUi->Add( sbSizerStagingArea, 1, wxEXPAND|wxALL, 5 );


	bSizerMain->Add( bSizerMainUi, 1, wxEXPAND, 5 );

	_riskLabel = new wxStaticText( this, wxID_ANY, wxT("USE THIS TOOL AT YOUR OWN RISK!"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	_riskLabel->Wrap( -1 );
	_riskLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	_riskLabel->SetForegroundColour( wxColour( 255, 0, 0 ) );

	bSizerMain->Add( _riskLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerGameStatus;
	bSizerGameStatus = new wxBoxSizer( wxHORIZONTAL );

	_gameStatusLabel = new wxStaticText( this, wxID_ANY, wxT("Game status:"), wxDefaultPosition, wxDefaultSize, 0 );
	_gameStatusLabel->Wrap( -1 );
	bSizerGameStatus->Add( _gameStatusLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );

	_gameStatus = new wxStaticText( this, wxID_ANY, wxT("not running"), wxDefaultPosition, wxDefaultSize, 0 );
	_gameStatus->Wrap( -1 );
	_gameStatus->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	_gameStatus->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_CAPTIONTEXT ) );

	bSizerGameStatus->Add( _gameStatus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerMain->Add( bSizerGameStatus, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	_aboutText = new wxStaticText( this, wxID_ANY, wxT("This version of the application was tested on M.A.S.S. Builder early access version 0.2.4.\nIt may or may not work with other versions of the game.\nMade for the M.A.S.S. Builder community by Guillaume Jacquemin."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	_aboutText->Wrap( -1 );
	bSizerMain->Add( _aboutText, 0, wxEXPAND|wxRIGHT|wxLEFT|wxALIGN_CENTER_HORIZONTAL, 5 );

	_githubLink = new wxHyperlinkCtrl( this, wxID_ANY, wxT("https://github.com/williamjcm/wxMASSManager"), wxT("https://github.com/williamjcm/wxMASSManager"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizerMain->Add( _githubLink, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	_gameCheckTimer.SetOwner( this, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	_moveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::moveEvent ), NULL, this );
	_deleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteEvent ), NULL, this );
	_renameButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::renameMassEvent ), NULL, this );
	_zipButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::backupEvent ), NULL, this );
	_openSaveDirButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openSaveDirEvent ), NULL, this );
	_importButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::importEvent ), NULL, this );
	_exportButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::exportEvent ), NULL, this );
	_stagingList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( MainFrame::stagingSelectionEvent ), NULL, this );
	_deleteStagedButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteStagedEvent ), NULL, this );
	_stagingAreaButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::stagingButtonEvent ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::gameCheckTimerEvent ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	_moveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::moveEvent ), NULL, this );
	_deleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteEvent ), NULL, this );
	_renameButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::renameMassEvent ), NULL, this );
	_zipButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::backupEvent ), NULL, this );
	_openSaveDirButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openSaveDirEvent ), NULL, this );
	_importButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::importEvent ), NULL, this );
	_exportButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::exportEvent ), NULL, this );
	_stagingList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( MainFrame::stagingSelectionEvent ), NULL, this );
	_deleteStagedButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteStagedEvent ), NULL, this );
	_stagingAreaButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::stagingButtonEvent ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::gameCheckTimerEvent ) );

}
