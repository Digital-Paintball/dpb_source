//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//
#include <cbase.h>
#include "c_multiarena.h"
#include "clientscoreboarddialog.h"
#include "hud_macros.h"

#include "c_playerresource.h"
#include "c_team.h" // jeff 4/8 - revamped team code

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void RecvProxyArrayLength_Arena_TeamArray( void *pStruct, int objectID, int currentArrayLength ) // jeff 4/8 - revamped team code
{
}

void SetTeamRef( C_Arena *pArena, int iTeamID, int iTeamRef ) // jeff 4/8 - revamped team code
{
	if( !pArena )
		return;

	EHANDLE Parent;
	IntToHandle( &Parent, iTeamRef );

	if( !Parent.IsValid() )
		return;

	C_Team *pTeam = dynamic_cast< C_Team* >( Parent.Get( ) );

	if( !pTeam )
		return;

	pArena->SetTeam( iTeamID, pTeam );
}

void RecvProxy_Arena_TeamList( const CRecvProxyData *pData, void *pStruct, void *pOut ) // jeff 4/8 - revamped team code
{
	SetTeamRef( ( C_Arena* )pStruct, pData->m_iElement, pData->m_Value.m_Int );
}


IMPLEMENT_CLIENTCLASS_DT( C_Arena, DT_Arena, CArena )
	RecvPropInt( RECVINFO( m_State ) ),
	RecvPropInt( RECVINFO( m_iID ) ),
	RecvPropInt( RECVINFO( m_iRedTeamScore ) ),
	RecvPropInt( RECVINFO( m_iBlueTeamScore ) ),

	RecvPropArray2( 
		RecvProxyArrayLength_Arena_TeamArray,
		RecvPropInt("team_element", 0, SIZEOF_IGNORE, 0, RecvProxy_Arena_TeamList), 
		ARENATEAM_COUNT, 
		0, 
		"team_array"),
	
END_RECV_TABLE()

CUtlVector<C_Arena*> C_Arena::s_hArenas;

void __MsgFunc_Arena(bf_read &msg)
{
	C_Arena *pArena;

	switch (msg.ReadByte())
	{
	case CArenaShared::AE_JOIN:
		pArena = C_Arena::GetArena(msg.ReadByte());
		if (pArena)
			pArena->m_bHasLocalPlayer = true;
		break;
	case CArenaShared::AE_QUIT:
		for (int i = 0; i < C_Arena::GetArenaNumber(); i++)
			C_Arena::GetArena(i)->m_bHasLocalPlayer = false;
		break;
	case CArenaShared::AE_VICTORY:
		msg.ReadByte();	//This person won. Yipee... wait, why do we care?
		// jeff team won dialog / end of round dialog goes HURRR
		break;
	case CArenaShared::AE_TVICTORY:
		pArena = C_Arena::GetArena(msg.ReadByte()); // Jeff - in this arena the
		pArena->m_iRedTeamScore = msg.ReadByte(); // Jeff - red team score is now
		pArena->m_iBlueTeamScore = msg.ReadByte(); // Jeff - blue team score is now
		break;
	case CArenaShared::AE_TIMEUPDATE: 
		pArena = C_Arena::GetArena(msg.ReadByte());
		pArena->m_iRoundTime = msg.ReadByte(); // Jeff - Clients should know how long they have left.
		break;
	case CArenaShared::AE_RESET:
		C_BasePlayer::GetLocalPlayer()->ResetLeaning();
		pArena = C_Arena::GetArena(msg.ReadByte());

		//Including paintballmgr.h causes crashes for some reason, so I won't bother with this for now.
//		CPaintballMgr::GetManager()->RemoveBalls( C_BasePlayer::GetLocalPlayer()->GetArena() );
		break;
	}
}

C_Arena::C_Arena( )
{
	s_hArenas.AddToTail( this );
	gViewPortInterface->UpdatePanel( PANEL_SCOREBOARD );
	m_bHasLocalPlayer = false;
	m_iRedTeamScore=0;
	m_iBlueTeamScore=0;
	HOOK_MESSAGE(Arena);

	memset(m_pTeams, NULL, sizeof( m_pTeams ) ); // jeff revamped team code 4/8 
}

C_Arena::~C_Arena( )
{
	s_hArenas.FindAndRemove( this );
	gViewPortInterface->UpdatePanel( PANEL_SCOREBOARD );
}

int C_Arena::GetArenaNumber( )
{
	return s_hArenas.Count();
}

C_Arena* C_Arena::GetArena(int i)
{
	if (i < 0 || i >= s_hArenas.Count())
		return NULL;

	return s_hArenas[i];
}

bool C_Arena::HasPlayer(C_BasePlayer *pPlayer)
{
	if (pPlayer == C_BasePlayer::GetLocalPlayer())
		return m_bHasLocalPlayer;
	else
		return false;
}

Color g_ArenaTeamColors[ ARENATEAM_COUNT ] = {
	COLOR_RED,
	COLOR_BLUE
};

Color C_Arena::GetTeamColor( int iPlayerID )
{
	if( g_PR->GetTeam( iPlayerID ) == TEAM_INGAME )
	{
		return g_ArenaTeamColors[ g_PR->GetArenaTeamID( iPlayerID ) ];
	}
	else
	{
		return g_PR->GetTeamColor( iPlayerID );
	}
}

void C_Arena::SetTeam( int iTeamID, C_Team *pTeam )
{
	m_pTeams[ iTeamID ] = pTeam;
}
