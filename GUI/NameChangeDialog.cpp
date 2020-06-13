///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "NameChangeDialog.h"

///////////////////////////////////////////////////////////////////////////

NameChangeDialog::NameChangeDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	_nameLabel = new wxStaticText( this, wxID_ANY, wxT("Please enter a new name:"), wxDefaultPosition, wxDefaultSize, 0 );
	_nameLabel->Wrap( -1 );
	bSizerMain->Add( _nameLabel, 0, wxALL, 5 );

	_nameInput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !_nameInput->HasFlag( wxTE_MULTILINE ) )
	{
	_nameInput->SetMaxLength( 32 );
	}
	#else
	_nameInput->SetMaxLength( 32 );
	#endif
	bSizerMain->Add( _nameInput, 0, wxALL|wxEXPAND, 5 );

	_conditionsLabel = new wxStaticText( this, wxID_ANY, wxT("The name must satisfy the following conditions:"), wxDefaultPosition, wxDefaultSize, 0 );
	_conditionsLabel->Wrap( -1 );
	bSizerMain->Add( _conditionsLabel, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizerConditions;
	fgSizerConditions = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerConditions->SetFlexibleDirection( wxBOTH );
	fgSizerConditions->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	_lengthBitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_CROSS_MARK, wxART_BUTTON ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerConditions->Add( _lengthBitmap, 0, wxALL, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	_lengthLabel = new wxStaticText( this, wxID_ANY, wxT("Between 6 and 32 characters inclusive. Current: "), wxDefaultPosition, wxDefaultSize, 0 );
	_lengthLabel->Wrap( -1 );
	bSizer3->Add( _lengthLabel, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

	_nameLength = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	_nameLength->Wrap( -1 );
	bSizer3->Add( _nameLength, 0, wxTOP|wxBOTTOM, 5 );


	fgSizerConditions->Add( bSizer3, 1, wxEXPAND, 5 );

	_charsBitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_CROSS_MARK, wxART_BUTTON ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerConditions->Add( _charsBitmap, 0, wxALL, 5 );

	_charsLabel = new wxStaticText( this, wxID_ANY, wxT("Uses only the allowed characters: A-Z, a-z, 0-9, -, and spaces."), wxDefaultPosition, wxDefaultSize, 0 );
	_charsLabel->Wrap( -1 );
	fgSizerConditions->Add( _charsLabel, 0, wxALL, 5 );

	_whitespaceAtBeginOrEndBitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_CROSS_MARK, wxART_BUTTON ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerConditions->Add( _whitespaceAtBeginOrEndBitmap, 0, wxALL, 5 );

	_whitespaceAtBeginOrEndLabel = new wxStaticText( this, wxID_ANY, wxT("Has no whitespace at the beginning or end."), wxDefaultPosition, wxDefaultSize, 0 );
	_whitespaceAtBeginOrEndLabel->Wrap( -1 );
	fgSizerConditions->Add( _whitespaceAtBeginOrEndLabel, 0, wxALL, 5 );


	bSizerMain->Add( fgSizerConditions, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );


	bSizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );

	_okButton = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( _okButton, 0, wxRIGHT|wxLEFT, 5 );

	_cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( _cancelButton, 0, wxRIGHT|wxLEFT, 5 );


	bSizerMain->Add( bSizerButtons, 0, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	_nameInput->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( NameChangeDialog::textEditEvent ), NULL, this );
	_okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NameChangeDialog::okButtonEvent ), NULL, this );
	_cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NameChangeDialog::cancelButtonEvent ), NULL, this );
}

NameChangeDialog::~NameChangeDialog()
{
	// Disconnect Events
	_nameInput->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( NameChangeDialog::textEditEvent ), NULL, this );
	_okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NameChangeDialog::okButtonEvent ), NULL, this );
	_cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NameChangeDialog::cancelButtonEvent ), NULL, this );

}
