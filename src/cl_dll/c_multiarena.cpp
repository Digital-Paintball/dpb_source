//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#include "c_multiarena.h"
#include "clientscoreboarddialog.h"
#include "hud_macros.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT( C_Arena, DT_Arena, CArena )
	RecvPropInt( RECVINFO( m_State ) ),
	RecvPropInt( RECVINFO( m_iID ) ),
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
		break;
	case CArenaShared::AE_RESET:
		C_BasePlayer::GetLocalPlayer()->ResetLeaning();
		break;
	}
}

C_Arena::C_Arena( )
{
	s_hArenas.AddToTail( this );
	gViewPortInterface->UpdatePanel( PANEL_SCOREBOARD );
	m_bHasLocalPlayer = false;
	HOOK_MESSAGE(Arena);
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