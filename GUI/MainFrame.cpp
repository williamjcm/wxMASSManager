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

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	_mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerMainPanel;
	bSizerMainPanel = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerProfile;
	bSizerProfile = new wxBoxSizer( wxHORIZONTAL );

	_profileLabel = new wxStaticText( _mainPanel, wxID_ANY, wxT("Profile to manage:"), wxDefaultPosition, wxDefaultSize, 0 );
	_profileLabel->Wrap( -1 );
	bSizerProfile->Add( _profileLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString _profileChoiceChoices;
	_profileChoice = new wxChoice( _mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, _profileChoiceChoices, 0 );
	_profileChoice->SetSelection( 0 );
	_profileChoice->SetMinSize( wxSize( 150,-1 ) );

	bSizerProfile->Add( _profileChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_backupSelectedButton = new wxButton( _mainPanel, wxID_ANY, wxT("Backup selected profile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerProfile->Add( _backupSelectedButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerProfile->Add( 0, 0, 1, wxEXPAND, 5 );

	_unsafeCheckbox = new wxCheckBox( _mainPanel, wxID_ANY, wxT("Unsafe mode"), wxDefaultPosition, wxDefaultSize, 0 );
	_unsafeCheckbox->SetToolTip( wxT("CLICK AT YOUR OWN RISK!") );

	bSizerProfile->Add( _unsafeCheckbox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerMainPanel->Add( bSizerProfile, 0, wxEXPAND, 5 );

	_managerNotebook = new wxNotebook( _mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	_profilePanel = new wxPanel( _managerNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerProfilePanel;
	bSizerProfilePanel = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerGeneralInfo;
	sbSizerGeneralInfo = new wxStaticBoxSizer( new wxStaticBox( _profilePanel, wxID_ANY, wxT("General information") ), wxVERTICAL );

	wxFlexGridSizer* fgSizerGeneralStats;
	fgSizerGeneralStats = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerGeneralStats->AddGrowableCol( 1 );
	fgSizerGeneralStats->SetFlexibleDirection( wxBOTH );
	fgSizerGeneralStats->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	_companyNameLabel = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("Company name:"), wxDefaultPosition, wxDefaultSize, 0 );
	_companyNameLabel->Wrap( -1 );
	_companyNameLabel->SetToolTip( wxT("EXPERIMENTAL: Double-click the company name to rename it.") );

	fgSizerGeneralStats->Add( _companyNameLabel, 0, wxALL, 5 );

	_companyName = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("<blank>"), wxDefaultPosition, wxDefaultSize, 0 );
	_companyName->Wrap( -1 );
	_companyName->SetToolTip( wxT("EXPERIMENTAL: Double-click the company name to rename it.") );

	fgSizerGeneralStats->Add( _companyName, 0, wxALL|wxEXPAND, 5 );

	_creditsLabel = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("Credits:"), wxDefaultPosition, wxDefaultSize, 0 );
	_creditsLabel->Wrap( -1 );
	fgSizerGeneralStats->Add( _creditsLabel, 0, wxALL, 5 );

	_credits = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	_credits->Wrap( -1 );
	fgSizerGeneralStats->Add( _credits, 0, wxALL|wxEXPAND, 5 );

	_storyProgressLabel = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("Story progress:"), wxDefaultPosition, wxDefaultSize, 0 );
	_storyProgressLabel->Wrap( -1 );
	_storyProgressLabel->SetToolTip( wxT("What this value does or corresponds to is unknown at this point in time. It is only listed here for the sake of completion.") );

	fgSizerGeneralStats->Add( _storyProgressLabel, 0, wxALL, 5 );

	_storyProgress = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	_storyProgress->Wrap( -1 );
	_storyProgress->SetToolTip( wxT("What this value does or corresponds to is unknown at this point in time. It is only listed here for the sake of completion.") );

	fgSizerGeneralStats->Add( _storyProgress, 0, wxALL, 5 );

	_lastMissionIdLabel = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("Last mission ID:"), wxDefaultPosition, wxDefaultSize, 0 );
	_lastMissionIdLabel->Wrap( -1 );
	_lastMissionIdLabel->SetToolTip( wxT("This corresponds to the last mission menu selection, not the last mission played.\nIf you see just a number instead of the mission name, please report it to the creator of this application.") );

	fgSizerGeneralStats->Add( _lastMissionIdLabel, 0, wxALL, 5 );

	_lastMissionId = new wxStaticText( sbSizerGeneralInfo->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	_lastMissionId->Wrap( -1 );
	_lastMissionId->SetToolTip( wxT("This corresponds to the last mission menu selection, not the last mission played.\nIf you see just a number instead of the mission name, please report it to the creator of this application.") );

	fgSizerGeneralStats->Add( _lastMissionId, 0, wxALL, 5 );


	sbSizerGeneralInfo->Add( fgSizerGeneralStats, 1, wxEXPAND, 5 );


	bSizerProfilePanel->Add( sbSizerGeneralInfo, 1, wxEXPAND|wxALL, 5 );


	_profilePanel->SetSizer( bSizerProfilePanel );
	_profilePanel->Layout();
	bSizerProfilePanel->Fit( _profilePanel );
	_managerNotebook->AddPage( _profilePanel, wxT("Profile details and stats"), false );
	_massPanel = new wxPanel( _managerNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerMassPanel;
	bSizerMassPanel = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sbSizerInstalled;
	sbSizerInstalled = new wxStaticBoxSizer( new wxStaticBox( _massPanel, wxID_ANY, wxT("Installed M.A.S.S.es") ), wxVERTICAL );

	_installedListView = new wxListView(_massPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES);
	_installedListView->AppendColumn("Hangar", wxLIST_FORMAT_LEFT);
	_installedListView->AppendColumn("M.A.S.S. name", wxLIST_FORMAT_LEFT);
	sbSizerInstalled->Add( _installedListView, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

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

	_openSaveDirButton = new wxButton( sbSizerInstalled->GetStaticBox(), wxID_ANY, wxT("Open save directory"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerSecondRow->Add( _openSaveDirButton, 2, wxEXPAND|wxALL, 5 );


	sbSizerInstalled->Add( bSizerSecondRow, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );


	bSizerMassPanel->Add( sbSizerInstalled, 1, wxEXPAND|wxALL, 5 );

	wxBoxSizer* bSizerImportExport;
	bSizerImportExport = new wxBoxSizer( wxVERTICAL );

	_importButton = new wxButton( _massPanel, wxID_ANY, wxT("Import"), wxDefaultPosition, wxDefaultSize, 0 );

	_importButton->SetBitmap( wxArtProvider::GetBitmap( wxART_GO_BACK, wxART_BUTTON ) );
	bSizerImportExport->Add( _importButton, 1, wxALL|wxEXPAND, 5 );

	_exportButton = new wxButton( _massPanel, wxID_ANY, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );

	_exportButton->SetBitmap( wxArtProvider::GetBitmap( wxART_GO_FORWARD, wxART_BUTTON ) );
	_exportButton->SetBitmapPosition( wxRIGHT );
	bSizerImportExport->Add( _exportButton, 0, wxALL|wxEXPAND, 5 );


	bSizerMassPanel->Add( bSizerImportExport, 0, wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticBoxSizer* sbSizerStagingArea;
	sbSizerStagingArea = new wxStaticBoxSizer( new wxStaticBox( _massPanel, wxID_ANY, wxT("Staging area") ), wxVERTICAL );

	_stagingList = new wxListBox( sbSizerStagingArea->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB|wxLB_SINGLE );
	sbSizerStagingArea->Add( _stagingList, 1, wxALL|wxEXPAND, 5 );

	_deleteStagedButton = new wxButton( sbSizerStagingArea->GetStaticBox(), wxID_ANY, wxT("Delete staged M.A.S.S."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerStagingArea->Add( _deleteStagedButton, 0, wxALL|wxEXPAND, 5 );

	_stagingAreaButton = new wxButton( sbSizerStagingArea->GetStaticBox(), wxID_ANY, wxT("Open staging area directory"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerStagingArea->Add( _stagingAreaButton, 0, wxALL|wxEXPAND, 5 );


	bSizerMassPanel->Add( sbSizerStagingArea, 1, wxEXPAND|wxALL, 5 );


	_massPanel->SetSizer( bSizerMassPanel );
	_massPanel->Layout();
	bSizerMassPanel->Fit( _massPanel );
	_managerNotebook->AddPage( _massPanel, wxT("M.A.S.S.es"), false );
	_screenshotsPanel = new wxPanel( _managerNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerScreenshotsPanel;
	bSizerScreenshotsPanel = new wxBoxSizer( wxHORIZONTAL );

	_screenshotsList = new wxListCtrl( _screenshotsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ALIGN_TOP|wxLC_AUTOARRANGE|wxLC_ICON|wxLC_SINGLE_SEL );
	bSizerScreenshotsPanel->Add( _screenshotsList, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerScreenshotCommands;
	bSizerScreenshotCommands = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerSorting;
	sbSizerSorting = new wxStaticBoxSizer( new wxStaticBox( _screenshotsPanel, wxID_ANY, wxT("Sorting") ), wxVERTICAL );

	wxBoxSizer* bSizerSortType;
	bSizerSortType = new wxBoxSizer( wxHORIZONTAL );

	_nameRadio = new wxRadioButton( sbSizerSorting->GetStaticBox(), wxID_ANY, wxT("Filename"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	_nameRadio->SetValue( true );
	bSizerSortType->Add( _nameRadio, 1, wxALL, 5 );

	_creationDateRadio = new wxRadioButton( sbSizerSorting->GetStaticBox(), wxID_ANY, wxT("Creation date"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerSortType->Add( _creationDateRadio, 1, wxALL, 5 );


	sbSizerSorting->Add( bSizerSortType, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerSortOrder;
	bSizerSortOrder = new wxBoxSizer( wxHORIZONTAL );

	_ascendingRadio = new wxRadioButton( sbSizerSorting->GetStaticBox(), wxID_ANY, wxT("Ascending"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	_ascendingRadio->SetValue( true );
	bSizerSortOrder->Add( _ascendingRadio, 1, wxALL, 5 );

	_descendingRadio = new wxRadioButton( sbSizerSorting->GetStaticBox(), wxID_ANY, wxT("Descending"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerSortOrder->Add( _descendingRadio, 1, wxALL, 5 );


	sbSizerSorting->Add( bSizerSortOrder, 1, wxEXPAND, 5 );


	bSizerScreenshotCommands->Add( sbSizerSorting, 0, wxEXPAND|wxALL, 5 );

	_viewScreenshotButton = new wxButton( _screenshotsPanel, wxID_ANY, wxT("View"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScreenshotCommands->Add( _viewScreenshotButton, 0, wxALL|wxEXPAND, 5 );

	_deleteScreenshotButton = new wxButton( _screenshotsPanel, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScreenshotCommands->Add( _deleteScreenshotButton, 0, wxALL|wxEXPAND, 5 );

	_screenshotDirButton = new wxButton( _screenshotsPanel, wxID_ANY, wxT("Open directory"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScreenshotCommands->Add( _screenshotDirButton, 0, wxALL|wxEXPAND, 5 );


	bSizerScreenshotsPanel->Add( bSizerScreenshotCommands, 0, wxEXPAND, 5 );


	_screenshotsPanel->SetSizer( bSizerScreenshotsPanel );
	_screenshotsPanel->Layout();
	bSizerScreenshotsPanel->Fit( _screenshotsPanel );
	_managerNotebook->AddPage( _screenshotsPanel, wxT("Photo mode shots"), false );

	bSizerMainPanel->Add( _managerNotebook, 1, wxEXPAND, 5 );

	_riskLabel = new wxStaticText( _mainPanel, wxID_ANY, wxT("USE THIS TOOL AT YOUR OWN RISK!"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	_riskLabel->Wrap( -1 );
	_riskLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	_riskLabel->SetForegroundColour( wxColour( 255, 0, 0 ) );

	bSizerMainPanel->Add( _riskLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerGameStatus;
	bSizerGameStatus = new wxBoxSizer( wxHORIZONTAL );

	_gameStatusLabel = new wxStaticText( _mainPanel, wxID_ANY, wxT("Game status:"), wxDefaultPosition, wxDefaultSize, 0 );
	_gameStatusLabel->Wrap( -1 );
	bSizerGameStatus->Add( _gameStatusLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );

	_gameStatus = new wxStaticText( _mainPanel, wxID_ANY, wxT("not running"), wxDefaultPosition, wxDefaultSize, 0 );
	_gameStatus->Wrap( -1 );
	_gameStatus->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	_gameStatus->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_CAPTIONTEXT ) );

	bSizerGameStatus->Add( _gameStatus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerMainPanel->Add( bSizerGameStatus, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	_aboutText = new wxStaticText( _mainPanel, wxID_ANY, wxT("This version of the application was partially tested on M.A.S.S. Builder early access version 0.5.4.\nIt may or may not work with other versions of the game.\nMade for the M.A.S.S. Builder community by Guillaume Jacquemin."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	_aboutText->Wrap( -1 );
	bSizerMainPanel->Add( _aboutText, 0, wxEXPAND|wxRIGHT|wxLEFT|wxALIGN_CENTER_HORIZONTAL, 5 );

	_githubLink = new wxHyperlinkCtrl( _mainPanel, wxID_ANY, wxT("https://github.com/williamjcm/wxMASSManager"), wxT("https://github.com/williamjcm/wxMASSManager"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizerMainPanel->Add( _githubLink, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	_mainPanel->SetSizer( bSizerMainPanel );
	_mainPanel->Layout();
	bSizerMainPanel->Fit( _mainPanel );
	bSizerMain->Add( _mainPanel, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	_gameCheckTimer.SetOwner( this, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	_profileChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::profileSelectionEvent ), NULL, this );
	_backupSelectedButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::backupSelectedProfileEvent ), NULL, this );
	_unsafeCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::unsafeCheckboxEvent ), NULL, this );
	_managerNotebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MainFrame::tabChangeEvent ), NULL, this );
	_companyName->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MainFrame::companyRenameEvent ), NULL, this );
	_moveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::moveMassEvent ), NULL, this );
	_deleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteMassEvent ), NULL, this );
	_renameButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::renameMassEvent ), NULL, this );
	_openSaveDirButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openSaveDirEvent ), NULL, this );
	_importButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::importMassEvent ), NULL, this );
	_exportButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::exportMassEvent ), NULL, this );
	_stagingList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( MainFrame::stagingSelectionEvent ), NULL, this );
	_deleteStagedButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteStagedEvent ), NULL, this );
	_stagingAreaButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openStagingDirEvent ), NULL, this );
	_screenshotsList->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::viewScreenshotEvent ), NULL, this );
	_screenshotsList->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( MainFrame::screenshotListSelectionEvent ), NULL, this );
	_screenshotsList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::screenshotListSelectionEvent ), NULL, this );
	_nameRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotFilenameSortingEvent ), NULL, this );
	_creationDateRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotCreationDateSortingEvent ), NULL, this );
	_ascendingRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotAscendingSortingEvent ), NULL, this );
	_descendingRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotDescendingSortingEvent ), NULL, this );
	_viewScreenshotButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::viewScreenshotEvent ), NULL, this );
	_deleteScreenshotButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteScreenshotEvent ), NULL, this );
	_screenshotDirButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openScreenshotDirEvent ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::gameCheckTimerEvent ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	_profileChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::profileSelectionEvent ), NULL, this );
	_backupSelectedButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::backupSelectedProfileEvent ), NULL, this );
	_unsafeCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::unsafeCheckboxEvent ), NULL, this );
	_managerNotebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MainFrame::tabChangeEvent ), NULL, this );
	_companyName->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MainFrame::companyRenameEvent ), NULL, this );
	_moveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::moveMassEvent ), NULL, this );
	_deleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteMassEvent ), NULL, this );
	_renameButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::renameMassEvent ), NULL, this );
	_openSaveDirButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openSaveDirEvent ), NULL, this );
	_importButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::importMassEvent ), NULL, this );
	_exportButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::exportMassEvent ), NULL, this );
	_stagingList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( MainFrame::stagingSelectionEvent ), NULL, this );
	_deleteStagedButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteStagedEvent ), NULL, this );
	_stagingAreaButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openStagingDirEvent ), NULL, this );
	_screenshotsList->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::viewScreenshotEvent ), NULL, this );
	_screenshotsList->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( MainFrame::screenshotListSelectionEvent ), NULL, this );
	_screenshotsList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::screenshotListSelectionEvent ), NULL, this );
	_nameRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotFilenameSortingEvent ), NULL, this );
	_creationDateRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotCreationDateSortingEvent ), NULL, this );
	_ascendingRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotAscendingSortingEvent ), NULL, this );
	_descendingRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::screenshotDescendingSortingEvent ), NULL, this );
	_viewScreenshotButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::viewScreenshotEvent ), NULL, this );
	_deleteScreenshotButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::deleteScreenshotEvent ), NULL, this );
	_screenshotDirButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::openScreenshotDirEvent ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::gameCheckTimerEvent ) );

}
