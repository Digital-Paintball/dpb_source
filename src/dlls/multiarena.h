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
	void BeginThink();

	void StartTouch(CBaseEntity *pOther);
	void EndTouch(CBaseEntity *pOther);
	void Spawn();

	void AddToArena(CBasePlayer *pPlayer);
	void RemoveFromArena(CBasePlayer *pPlayer);
	void JoinPlayer(CBasePlayer *pPlayer);
	void QuitPlayer(CBasePlayer *pPlayer);

	void SetupRound();
	void CheckForRoundEnd();
	void RoundEnd( int iWinningTeam );
	void CalculateSpawnAvg();

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
	} gamestate_t;

	gamestate_t m_State;

private:
	int									m_iID;
	static CUtlVector<CHandle<CArena> >	s_hArenas;

	CUtlVector<EHANDLE>					m_hObjects;
	CUtlVector<EHANDLE>					m_hObjectives;
	CUtlVector<CHandle<CBasePlayer> >	m_hPlayers;
	CUtlVector<CHandle<CBasePlayer> >	m_hSpectators;
	CUtlVector<CHandle<CTeam> >			m_hTeams;

	CUtlVector<CHandle<CBasePlayer> >	m_hJoiners;		//Those who are joining the arena.
	CUtlVector<CHandle<CBasePlayer> >	m_hQuitters;	//Those who are leaving the arena.

	Vector m_vecSpawnAvg;
};

#endif // MULTIARENA_H