//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#ifndef MULTIARENA_H
#define MULTIARENA_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "triggers.h"
#include "player.h"
#include "team_spawnpoint.h"

class CArena : public CBaseTrigger
{
public:
	DECLARE_CLASS( CArena, CBaseTrigger );

	void WaitingThink();
	void SetupThink();

	void StartTouch(CBaseEntity *pOther);
	void EndTouch(CBaseEntity *pOther);
	void Spawn();

	void JoinPlayer(CBasePlayer *pPlayer);
	void StartRound();

	CTeam* GetTeam(int i);
	CTeam* GetTeamByNumber(int i);
	int GetTeamNumber();
	bool HasTeam(int i);

	static void AssembleArenas();
	static void ClearArenas();
	static int GetArenaNumber();
	static CArena* GetArena(int i);

	DECLARE_DATADESC();
	
	typedef enum {
		GS_WAITING,		//Waiting for players
		GS_COUNTDOWN,	//Counting down for the round to begin
		GS_INPROGRESS,	//People are killing each other right now
		GS_VICTORY,		//One team has achieved victory
	} gamestate_e;

	gamestate_e m_State;

private:
	static CUtlVector<CHandle<CArena> >	s_hArenas;

	CUtlVector<EHANDLE>					m_hObjects;
	CUtlVector<EHANDLE>					m_hObjectives;
	CUtlVector<CHandle<CBasePlayer> >	m_hPlayers;
	CUtlVector<CHandle<CTeam> >			m_hTeams;

};

#endif // MULTIARENA_H