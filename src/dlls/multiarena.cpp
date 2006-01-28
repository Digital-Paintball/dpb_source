//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#include "multiarena.h"
#include "team.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_DATADESC( CArena )

	// Function Pointers
	DEFINE_FUNCTION( WaitingThink ),
	DEFINE_FUNCTION( SetupThink ),

END_DATADESC()

LINK_ENTITY_TO_CLASS(game_arena, CArena);

CUtlVector<CHandle<CArena> > CArena::s_hArenas;

void CArena::Spawn( )
{
	BaseClass::Spawn();
	InitTrigger();

	AddSpawnFlags(SF_TRIGGER_ALLOW_ALL);

	m_State = GS_WAITING;

	SetNextThink( gpGlobals->curtime + 5.0 );
	SetThink( WaitingThink );

	s_hArenas.AddToTail( this );
}

void CArena::AssembleArenas( )
{
	CBaseEntity *pList[1024];
	CArena *pArena;
	for (int i = 0; i < s_hArenas.Count(); i++)
	{
		pArena = s_hArenas[i];
		int iResults = UTIL_EntitiesInBox( pList, 1024,
			pArena->CollisionProp()->OBBMins() + pArena->GetAbsOrigin(),
			pArena->CollisionProp()->OBBMaxs() + pArena->GetAbsOrigin(), 0 );
		for (int j = 0; j < iResults; j++)
		{
			if (pList[j] == pArena)
				continue;
			pArena->m_hObjects.AddToTail( pList[j] );
			pList[j]->SetArena( pArena );

			if (pList[j]->GetStartTeamNumber() == 0)
				continue;

			if (!pArena->HasTeam(pList[j]->GetStartTeamNumber()))
			{
				CTeam *pTeam = static_cast<CTeam*>(CreateEntityByName( "sdk_team_manager" ));
				pTeam->Init( "Team", pList[j]->GetStartTeamNumber() );
				pArena->m_hTeams.AddToTail(pTeam);
			}
		}
	}
}

bool CArena::HasTeam(int iTeam)
{
	for (int i = 0; i < m_hTeams.Count(); i++)
		if (m_hTeams[i]->m_iTeamNum == iTeam)
			return true;

	return false;
}

void CArena::WaitingThink( )
{
	if (m_hPlayers.Count() > 0)
	{
		StartRound();
	}
	else
	{
		// Try again in five seconds.
		SetNextThink( gpGlobals->curtime + 5.0 );
	}
}

void CArena::SetupThink( )
{
	for (int i = 0; i < m_hPlayers.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);

		pPlayer->DeployArmaments();

		// Pick a team!
		CTeam *pTeam = NULL;
		for (int j = 0; j < m_hTeams.Count(); j++)
		{
			if (!pTeam)
			{
				pTeam = m_hTeams[j];
				continue;
			}

			if (m_hTeams[j]->m_aPlayers.Count() < pTeam->m_aPlayers.Count())
				pTeam = m_hTeams[j];
		}

		if (!pTeam)
		{
			DevMsg("ERROR: Arena has no teams! Put some info_player_teamspawns in there.\n");
			continue;
		}

		pPlayer->ChangeTeam(pTeam->GetTeamNumber());

		CBaseEntity *pSpawnSpot = pPlayer->GetTeam()->SpawnPlayer(pPlayer);
		pPlayer->SetLocalOrigin( pSpawnSpot->GetLocalOrigin() + Vector(0,0,1) );
		pPlayer->SetLocalAngles( pSpawnSpot->GetLocalAngles() );
		pPlayer->SnapEyeAngles( pSpawnSpot->GetLocalAngles() );
	}
}

void CArena::StartRound( )
{
	m_State = GS_COUNTDOWN;

	SetThink( SetupThink );
	SetNextThink( gpGlobals->curtime + 5.0 );
}

void CArena::JoinPlayer( CBasePlayer *pPlayer )
{
	m_hPlayers.AddToTail( pPlayer );
}

void CArena::StartTouch( CBaseEntity *pOther )
{
	BaseClass::StartTouch(pOther);

	if (!pOther->IsPlayer())
		return;

	CBasePlayer* pPlayer = ToBasePlayer( pOther );

	pPlayer->SetArena( this );
}

void CArena::EndTouch( CBaseEntity *pOther )
{
	BaseClass::EndTouch(pOther);

	CBasePlayer* pPlayer = ToBasePlayer( pOther );

	pPlayer->SetArena( NULL );

	m_hPlayers.FindAndRemove(pPlayer);
}

void CBasePlayer::JoinGame()
{
	if (GetArena() == NULL)
		return;

	GetArena()->JoinPlayer( this );
}

void CArena::ClearArenas( )
{
	s_hArenas.RemoveAll();
}

int CArena::GetArenaNumber( )
{
	return s_hArenas.Count();
}

CArena* CArena::GetArena(int i)
{
	return s_hArenas[i];
}

int CArena::GetTeamNumber( )
{
	return m_hTeams.Count();
}

CTeam* CArena::GetTeam(int i)
{
	if (i >= m_hTeams.Count() || i < 0)
		return NULL;

	return m_hTeams[i];
}

CTeam* CArena::GetTeamByNumber(int iTeam)
{
	for (int i = 0; i < m_hTeams.Count(); i++)
		if (m_hTeams[i]->m_iTeamNum == iTeam)
			return m_hTeams[i];

	return NULL;
}