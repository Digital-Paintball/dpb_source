//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_MULTIARENA_H
#define C_MULTIARENA_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "multiarena_shared.h"

class C_Arena : public C_BaseEntity
{
public:
	DECLARE_CLIENTCLASS();
	DECLARE_CLASS( C_Arena, C_BaseEntity );

	C_Arena();
	~C_Arena();

	static int GetArenaNumber();
	static C_Arena* GetArena(int i);

	bool	HasPlayer(C_BasePlayer *pPlayer);

	CArenaShared::gamestate_t m_State;

	bool	m_bHasLocalPlayer;

private:
	int									m_iID;
	static CUtlVector<C_Arena*>			s_hArenas;
};

#endif // C_MULTIARENA_H