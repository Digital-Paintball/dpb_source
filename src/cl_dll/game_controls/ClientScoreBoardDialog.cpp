//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include <stdio.h>

#include <cdll_client_int.h>
#include <cdll_util.h>
#include <globalvars_base.h>
#include <igameresources.h>

#include "clientscoreboarddialog.h"
#include "sdk_gamerules.h"
#include "voice_status.h"
#include "c_multiarena.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vstdlib/IKeyValuesSystem.h>

#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/SectionedListPanel.h>

#include <cl_dll/iviewport.h>
#include <igameresources.h>

//#include "voice_status.h"
//#include "Friends/IFriendsUser.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// extern vars
//extern IFriendsUser *g_pFriendsUser;

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CClientScoreBoardDialog::CClientScoreBoardDialog(IViewPort *pViewPort) : Frame( NULL, PANEL_SCOREBOARD )
{
	m_iPlayerIndexSymbol = KeyValuesSystem()->GetSymbolForString("playerIndex");

	memset(s_VoiceImage, 0x0, sizeof( s_VoiceImage ));
	TrackerImage = 0;
	m_pViewPort = pViewPort;

	// initialize dialog
	SetProportional(true);
	SetKeyBoardInputEnabled(false);
	SetMouseInputEnabled(false);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );

	// set the scheme before any child control is created
	SetScheme("ClientScheme");

	m_pPlayerList = new SectionedListPanel(this, "PlayerList");
	m_pPlayerList->SetVerticalScrollbar(false);

	LoadControlSettings("Resource/UI/ScoreBoard.res");
	m_iDesiredHeight = GetTall();
	m_pPlayerList->SetVisible( false ); // hide this until we load the images in applyschemesettings

	m_HLTVSpectators = 0;
	
	// update scoreboard instantly if on of these events occure
	gameeventmanager->AddListener(this, "hltv_status", false );
	gameeventmanager->AddListener(this, "server_spawn", false );

	// Four default buttons to work with.
	m_hButtons.AddToTail(new vgui::Button(this, "Arena Button", "Arena", this));
	m_hButtons.AddToTail(new vgui::Button(this, "Arena Button", "Arena", this));
	m_hButtons.AddToTail(new vgui::Button(this, "Arena Button", "Arena", this));
	m_hButtons.AddToTail(new vgui::Button(this, "Arena Button", "Arena", this));
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CClientScoreBoardDialog::~CClientScoreBoardDialog()
{
	gameeventmanager->RemoveListener(this);

	for (int i = 0; i < m_hButtons.Count(); i++)
		delete m_hButtons[i];
}

//-----------------------------------------------------------------------------
// Purpose: clears everything in the scoreboard and all it's state
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::Reset()
{
	// clear
	m_pPlayerList->DeleteAllItems();
	m_pPlayerList->RemoveAllSections();

	m_iSectionId = 0;
	m_fNextUpdateTime = 0;
	// add all the sections
	InitScoreboardSections();
}

//-----------------------------------------------------------------------------
// Purpose: adds all the team sections to the scoreboard
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::InitScoreboardSections()
{
	AddHeader();
}

//-----------------------------------------------------------------------------
// Purpose: sets up screen
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	ImageList *imageList = new ImageList(false);

	s_VoiceImage[0] = 0;	// index 0 is always blank
	s_VoiceImage[CVoiceStatus::VOICE_NEVERSPOKEN] = imageList->AddImage(scheme()->GetImage("gui/vdisabled", true));
	s_VoiceImage[CVoiceStatus::VOICE_NOTTALKING] = imageList->AddImage(scheme()->GetImage("gui/vneutral", true));
	s_VoiceImage[CVoiceStatus::VOICE_TALKING] = imageList->AddImage(scheme()->GetImage( "gui/vactive", true));
	s_VoiceImage[CVoiceStatus::VOICE_BANNED] = imageList->AddImage(scheme()->GetImage("gui/vbanned", true));

	// resize the images to our resolution
	for (int i = 0; i < imageList->GetImageCount(); i++ )
	{
		int wide, tall;
		imageList->GetImage(i)->GetSize(wide, tall);
		imageList->GetImage(i)->SetSize(scheme()->GetProportionalScaledValue(wide), scheme()->GetProportionalScaledValue(tall));
	}

	m_pPlayerList->SetImageList(imageList, false);
	m_pPlayerList->SetVisible( true );

	// light up scoreboard a bit
	SetBgColor( Color( 0,0,0,0) );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		Reset();
		Update();

		SetMouseInputEnabled( true );
		Activate();
	}
	else
	{
		BaseClass::SetVisible( false );
		SetMouseInputEnabled( false );
		SetKeyBoardInputEnabled( false );
	}
}



void CClientScoreBoardDialog::FireGameEvent( IGameEvent *event )
{
	const char * type = event->GetName();

	if ( Q_strcmp(type, "hltv_status") == 0 )
	{
		// spectators = clients - proxies
		m_HLTVSpectators = event->GetInt( "clients" );
		m_HLTVSpectators -= event->GetInt( "proxies" );
	}

	else if ( Q_strcmp(type, "server_spawn") == 0 )
	{
		SetControlString("ServerName", event->GetString("hostname") );
		MoveLabelToFront("ServerName");
	}

	if( IsVisible() )
		Update();

}

bool CClientScoreBoardDialog::NeedsUpdate( void )
{
	return (m_fNextUpdateTime < gpGlobals->curtime);
		

}

//-----------------------------------------------------------------------------
// Purpose: Recalculate the internal scoreboard data
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::Update( void )
{
	// Set the title
	
	// Reset();
	m_pPlayerList->DeleteAllItems();
	
	FillScoreBoard();

	// grow the scoreboard to fit all the players
	int wide, tall;
	m_pPlayerList->GetContentSize(wide, tall);
	wide = GetWide();
	if (m_iDesiredHeight < tall)
	{
		SetSize(wide, tall);
		m_pPlayerList->SetSize(wide, tall);
	}
	else
	{
		SetSize(wide, m_iDesiredHeight);
		m_pPlayerList->SetSize(wide, m_iDesiredHeight);
	}

	MoveToCenterOfScreen();

	// update every second
	m_fNextUpdateTime = gpGlobals->curtime + 1.0f; 
}

//-----------------------------------------------------------------------------
// Purpose: Sort all the teams
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::UpdateTeamInfo()
{
	int sectionId;
	int i;
	char cstring[256];
	char plural[2];
	wchar_t tn[256];
	IGameResources *gr = GameResources();
	if ( !gr )
		return;

	for (i = TEAM_UNASSIGNED; i < TEAM_COUNT; i++)
	{
		sectionId = m_iTeamSections[i];

		if (m_iPlayersOnTeam[i] == 1)
			sprintf(plural,"");
		else
			sprintf(plural,"s");

		sprintf(cstring, "%s - (%i player%s)", gr->GetTeamName(i),  m_iPlayersOnTeam[i], plural);
		localize()->ConvertANSIToUnicode(cstring, tn, sizeof(tn));
		m_pPlayerList->ModifyColumn(sectionId, "name", tn);

		if ( m_iPlayersOnTeam[i] > 0 )
			m_iLatency[i] /= m_iPlayersOnTeam[i];
		else
			m_iLatency[i] = 0;

		wchar_t sz[6];
		swprintf(sz, L"%d", gr->GetTeamScore(i));
		m_pPlayerList->ModifyColumn(sectionId, "score", sz);
		if (m_iLatency[i] < 1)
		{
			m_pPlayerList->ModifyColumn(sectionId, "ping", L"");
		}
		else
		{
			swprintf(sz, L"%i", m_iLatency[i]);
			m_pPlayerList->ModifyColumn(sectionId, "ping", sz);
		}
		m_iLatency[i] = 0;

		m_pPlayerList->SetSectionFgColor(sectionId, gr->GetTeamColor(i));
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::UpdatePlayerInfo()
{
	m_iSectionId = 0; // 0'th row is a header
	int selectedRow = -1;

	// walk all the players and make sure they're in the scoreboard
	for ( int i = 1; i < gpGlobals->maxClients; i++ )
	{
		IGameResources *gr = GameResources();

		if ( gr && gr->IsConnected( i ) )
		{
			// add the player to the list
			KeyValues *playerData = new KeyValues("data");
			GetPlayerScoreInfo( i, playerData );

	
			const char *oldName = playerData->GetString("name","");
			int bufsize = strlen(oldName) * 2;
			char *newName = (char *)_alloca( bufsize );

			UTIL_MakeSafeName( oldName, newName, bufsize );

			playerData->SetString("name", newName);

			int itemID = FindItemIDForPlayerIndex( i );
			int playerTeam = gr->GetTeam(i);
			int sectionID = m_iTeamSections[playerTeam];

			m_iPlayersOnTeam[playerTeam]++;
			m_iLatency[playerTeam]+=playerData->GetInt("ping");

			if ( gr->IsLocalPlayer( i ) )
			{
				selectedRow = itemID;
			}
			if (itemID == -1)
			{
				// add a new row
				itemID = m_pPlayerList->AddItem( sectionID, playerData );
			}
			else
			{
				// modify the current row
				m_pPlayerList->ModifyItem( itemID, sectionID, playerData );
			}

			// set the row color based on the players team
			m_pPlayerList->SetItemFgColor( itemID, gr->GetTeamColor( playerTeam ) );

			playerData->deleteThis();
		}
		else
		{
			// remove the player
			int itemID = FindItemIDForPlayerIndex( i );
			if (itemID != -1)
			{
				m_pPlayerList->RemoveItem(itemID);
			}
		}
	}

	if ( selectedRow != -1 )
	{
		m_pPlayerList->SetSelectedItem(selectedRow);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::UpdateArenaInfo()
{
	for (int i = 0; i < m_hButtons.Count(); i++)
	{
		if (i < C_Arena::GetArenaNumber())
		{
			m_hButtons[i]->SetVisible(true);
			m_hButtons[i]->SetPos(i*70+10, 26);
		}
		else
			m_hButtons[i]->SetVisible(false);
	}
}

//-----------------------------------------------------------------------------
// Purpose: adds the top header of the scoreboars
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::AddHeader()
{
	m_iSectionId = 0; //make a blank one for the server name
	m_pPlayerList->AddSection(m_iSectionId, "");
	m_pPlayerList->SetSectionAlwaysVisible(m_iSectionId);
	m_pPlayerList->AddColumnToSection(m_iSectionId, "name", "", 0, scheme()->GetProportionalScaledValue(NAME_WIDTH) );

	m_iSectionId = 1; //make a blank one for arenas
	m_pPlayerList->AddSection(m_iSectionId, "");
	m_pPlayerList->SetSectionAlwaysVisible(m_iSectionId);
	m_pPlayerList->AddColumnToSection(m_iSectionId, "name", "", 0, scheme()->GetProportionalScaledValue(NAME_WIDTH) );

	m_iSectionId = 2;
	m_pPlayerList->AddSection(m_iSectionId, "");
	m_pPlayerList->SetSectionAlwaysVisible(m_iSectionId);
	m_pPlayerList->AddColumnToSection(m_iSectionId, "name", "", 0, scheme()->GetProportionalScaledValue(NAME_WIDTH) );
	m_pPlayerList->AddColumnToSection(m_iSectionId, "score", "#PlayerScore", 0, scheme()->GetProportionalScaledValue(SCORE_WIDTH) );
	m_pPlayerList->AddColumnToSection(m_iSectionId, "ping", "#PlayerPing", 0, scheme()->GetProportionalScaledValue(PING_WIDTH) );
	m_pPlayerList->AddColumnToSection(m_iSectionId, "voice", "#PlayerVoice", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, scheme()->GetProportionalScaledValue(VOICE_WIDTH) );

	m_iSectionId = 3; //first team;
	m_iTeamSections[TEAM_BLUE]			= AddSection(TYPE_TEAM, TEAM_BLUE);
	m_iTeamSections[TEAM_RED]			= AddSection(TYPE_TEAM, TEAM_RED);
	m_iTeamSections[TEAM_UNASSIGNED]	= AddSection(TYPE_NOTEAM, TEAM_UNASSIGNED);
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new section to the scoreboard (i.e the team header)
//-----------------------------------------------------------------------------
int CClientScoreBoardDialog::AddSection(int teamType, int teamNumber)
{
	if ( teamType == TYPE_TEAM )
	{
		IGameResources *gr = GameResources();

		if ( !gr )
			return -1;

		// setup the team name
		wchar_t *teamName = localize()->Find( gr->GetTeamName(teamNumber) );
		wchar_t name[64];
       
		if (!teamName)
		{
			localize()->ConvertANSIToUnicode(gr->GetTeamName(teamNumber), name, sizeof(name));
			teamName = name;
		}

		m_pPlayerList->AddSection(m_iSectionId, "", StaticPlayerSortFunc);
		m_pPlayerList->SetSectionAlwaysVisible(m_iSectionId);
		m_pPlayerList->SetFgColor(gr->GetTeamColor(teamNumber));

		m_pPlayerList->AddColumnToSection(m_iSectionId, "name", teamName, 0, scheme()->GetProportionalScaledValue(NAME_WIDTH) );
		m_pPlayerList->AddColumnToSection(m_iSectionId, "score", "0", 0, scheme()->GetProportionalScaledValue(SCORE_WIDTH) );
		m_pPlayerList->AddColumnToSection(m_iSectionId, "ping", "", 0, scheme()->GetProportionalScaledValue(PING_WIDTH) );
		m_pPlayerList->AddColumnToSection(m_iSectionId, "voice", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, scheme()->GetProportionalScaledValue(VOICE_WIDTH) );
	}
	else if ( teamType == TYPE_UNASSIGNED )
	{
		m_pPlayerList->AddSection(m_iSectionId, "");
		m_pPlayerList->AddColumnToSection(m_iSectionId, "name", "#Unassigned", 0, scheme()->GetProportionalScaledValue(NAME_WIDTH));
		m_pPlayerList->AddColumnToSection(m_iSectionId, "score", "", 0, scheme()->GetProportionalScaledValue(SCORE_WIDTH) );
		m_pPlayerList->AddColumnToSection(m_iSectionId, "ping", "", 0, scheme()->GetProportionalScaledValue(PING_WIDTH) );
	}
	m_iSectionId++; //increment for next
	return m_iSectionId - 1;
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting players
//-----------------------------------------------------------------------------
bool CClientScoreBoardDialog::StaticPlayerSortFunc(vgui::SectionedListPanel *list, int itemID1, int itemID2)
{
	KeyValues *it1 = list->GetItemData(itemID1);
	KeyValues *it2 = list->GetItemData(itemID2);
	Assert(it1 && it2);

	int v1 = it1->GetInt("score");
	int v2 = it2->GetInt("score");
	if (v1 > v2)
		return true;
	else if (v1 < v2)
		return false;

	// the same, so compare itemID's (as a sentinel value to get deterministic sorts)
	return itemID1 < itemID2;
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new row to the scoreboard, from the playerinfo structure
//-----------------------------------------------------------------------------
bool CClientScoreBoardDialog::GetPlayerScoreInfo(int playerIndex, KeyValues *kv)
{
	IGameResources *gr = GameResources();

	if (!gr )
		return false;

	kv->SetInt("score", gr->GetFrags( playerIndex ) );
	kv->SetInt("ping", gr->GetPing( playerIndex ) ) ;
	kv->SetString("name", gr->GetPlayerName( playerIndex ) );
	kv->SetInt("playerIndex", playerIndex);

//	kv->SetInt("voice",	s_VoiceImage[GetClientVoice()->GetSpeakerStatus( playerIndex - 1) ]);	

/*	// setup the tracker column
	if (g_pFriendsUser)
	{
		unsigned int trackerID = gEngfuncs.GetTrackerIDForPlayer(row);

		if (g_pFriendsUser->IsBuddy(trackerID) && trackerID != g_pFriendsUser->GetFriendsID())
		{
			kv->SetInt("tracker",TrackerImage);
		}
	}
*/
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: reload the player list on the scoreboard
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::FillScoreBoard()
{
	for (int i = TEAM_UNASSIGNED; i < TEAM_COUNT; i++)
	{
		m_iPlayersOnTeam[i] = 0;
	}

	UpdateArenaInfo();
	
	// update totals information
	UpdateTeamInfo();

	// update player info
	UpdatePlayerInfo();
} 

//-----------------------------------------------------------------------------
// Purpose: searches for the player in the scoreboard
//-----------------------------------------------------------------------------
int CClientScoreBoardDialog::FindItemIDForPlayerIndex(int playerIndex)
{
	for (int i = 0; i <= m_pPlayerList->GetHighestItemID(); i++)
	{
		if (m_pPlayerList->IsItemIDValid(i))
		{
			KeyValues *kv = m_pPlayerList->GetItemData(i);
			kv = kv->FindKey(m_iPlayerIndexSymbol);
			if (kv && kv->GetInt() == playerIndex)
				return i;
		}
	}
	return -1;
}




//-----------------------------------------------------------------------------
// Purpose: Sets the text of a control by name
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::MoveLabelToFront(const char *textEntryName)
{
	Label *entry = dynamic_cast<Label *>(FindChildByName(textEntryName));
	if (entry)
	{
		entry->MoveToFront();
	}
}

