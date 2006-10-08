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
#include "multiarena_shared.h"

class CArena : public CBaseTrigger
{
public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_CLASS( CArena, CBaseTrigger );

	void WaitingThink();
	void BeginThink();
	void TimesUpThink();
	void KeepTimeThink();

	void StartTouch(CBaseEntity *pOther);
	void EndTouch(CBaseEntity *pOther);
	void Spawn();

	void AddToArena(CBasePlayer *pPlayer);
	void RemoveFromArena(CBasePlayer *pPlayer);
	void RemoveQuitter(CBasePlayer *pPlayer);
	void JoinPlayer(CBasePlayer *pPlayer);
	void QuitPlayer(CBasePlayer *pPlayer);
	bool HasPlayer(CBasePlayer *pPlayer);
	void AssignTeam(CBasePlayer *pPlayer);
	void SwitchTeam(CBasePlayer *pPlayer, int newteam);
	void SwitchQueueAdd(CBasePlayer *pPlayer, int newteam);

	static void SpawnPlayer(CBasePlayer *pPlayer, CArena* pArena = NULL);
	static void MovePlayer(CBasePlayer *pPlayer, const Vector &vecOrigin, const QAngle &angAngles);

	void SetupRound();
	void CheckForRoundEnd();
	void RoundEnd( int iWinningTeam );
	void CalculateSpawnAvg();

	CTeam* GetTeam(int i);
	CTeam* GetTeamByNumber(int i);
	int GetTeamNumber();
	bool HasTeam(int i);

	int GetNumberOfPlayers();

	static void AssembleArenas();
	static void ClearArenas();
	static int GetArenaNumber();
	static CArena* GetArena(int i);

	int UpdateTransmitState() { return SetTransmitState( FL_EDICT_ALWAYS ); }

	CNetworkVar( CArenaShared::gamestate_t, m_State );

	CNetworkVar ( int, m_iRedTeamScore );
	CNetworkVar ( int, m_iBlueTeamScore );
	// jeff hold team scores

private:
	CNetworkVar( int, m_iID );
	static CUtlVector<CHandle<CArena> >	s_hArenas;

	CUtlVector<EHANDLE>					m_hObjects;
	CUtlVector<EHANDLE>					m_hObjectives;
	CUtlVector<CHandle<CBasePlayer> >	m_hPlayers;
	CUtlVector<CHandle<CBasePlayer> >	m_hSpectators;
	CUtlVector<CHandle<CTeam> >			m_hTeams;

	CUtlVector<CHandle<CBasePlayer> >	m_hJoiners;		//Those who are joining the arena.
	CUtlVector<CHandle<CBasePlayer> >	m_hQuitters;	//Those who are leaving the arena.
	CUtlVector<CHandle<CBasePlayer> >	m_hSwitchersRed;	//Those who are about to die... switch teams - Jeff
	CUtlVector<CHandle<CBasePlayer> >	m_hSwitchersBlue;
	Vector		m_vecSpawnAvg;

	// Values which can be set in the FGD
	string_t	m_iszName;
	float		m_flMinutes;
	float		m_startedWhen;
	float		m_willEnd;
};

#endif // MULTIARENA_H