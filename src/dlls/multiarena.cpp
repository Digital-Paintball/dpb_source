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
	DEFINE_FUNCTION( BeginThink ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CArena, DT_Arena )
	SendPropInt( SENDINFO( m_State ), 2, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iID ), 4, SPROP_UNSIGNED ),
END_SEND_TABLE() 

LINK_ENTITY_TO_CLASS(game_arena, CArena);

CUtlVector<CHandle<CArena> > CArena::s_hArenas;

void CArena::Spawn( )
{
	BaseClass::Spawn();
	InitTrigger();

	AddSpawnFlags(SF_TRIGGER_ALLOW_ALL);
	AddEFlags( EFL_FORCE_CHECK_TRANSMIT );

	m_State = CArenaShared::GS_WAITING;

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
		pArena->m_iID = i;
		int iResults = UTIL_EntitiesInBox( pList, 1024,
			pArena->CollisionProp()->OBBMins() + pArena->GetAbsOrigin(),
			pArena->CollisionProp()->OBBMaxs() + pArena->GetAbsOrigin(), 0 );
		int j;
		for (j = 0; j < iResults; j++)
		{
			if (pList[j] == pArena)
				continue;
			pArena->m_hObjects.AddToTail( pList[j] );
			pList[j]->SetArena( pArena );

			if (pList[j]->GetStartTeamNumber() == 0)
				continue;

			if (!pArena->HasTeam(pList[j]->GetStartTeamNumber()))
			{
				CTeam *pTeam = static_cast<CTeam*>(CreateEntityByName( "team_manager" ));
				pTeam->Init( pList[j]->GetStartTeamNumber()==1?"Blue":"Red", pList[j]->GetStartTeamNumber(), pArena );
				pArena->m_hTeams.AddToTail(pTeam);
			}
		}
	}
}

void CArena::CalculateSpawnAvg()
{
	m_vecSpawnAvg = Vector(0, 0, 0);

	if (!GetTeamNumber())
		return;

	for (int i = 0; i < GetTeamNumber(); i++)
	{
		m_hTeams[i]->AverageSpawns();
		m_vecSpawnAvg += m_hTeams[i]->m_vecSpawnAvg;
	}

	m_vecSpawnAvg /= GetTeamNumber();
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
	m_State = CArenaShared::GS_WAITING;

	if (m_hPlayers.Count() + m_hJoiners.Count() > 0)
	{
		SetupRound();
	}
	else
	{
		// Try again in five seconds.
		SetNextThink( gpGlobals->curtime + 5.0 );
	}
}

void CArena::SetupRound( )
{
	int i;
	bool bRoundStarting = true;

	m_State = CArenaShared::GS_COUNTDOWN;

	// Fix things from last round.
	for (i = 0; i < m_hPlayers.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);
		if (!pPlayer->IsAlive())
			pPlayer->Spawn();

		if (pPlayer->GetActiveWeapon())
			pPlayer->GetActiveWeapon()->Holster();
	}

	// Remove people who have quit the game
	for (i = 0; i < m_hQuitters.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hQuitters[i]);
		m_hPlayers.FindAndRemove(pPlayer);
		if (pPlayer->GetTeam())
			pPlayer->GetTeam()->RemovePlayer(pPlayer);

		pPlayer->RemoveAllItems(false);

		m_hSpectators.AddToHead( pPlayer );

		// default to normal spawn
		CBaseEntity *pSpawnSpot = pPlayer->EntSelectStartPoint();
		pPlayer->SetLocalOrigin( pSpawnSpot->GetLocalOrigin() + Vector(0,0,1) );
		pPlayer->SetLocalAngles( pSpawnSpot->GetLocalAngles() );
		pPlayer->SnapEyeAngles( pSpawnSpot->GetLocalAngles() );
	}

	//Then add people who have joined the game
	for (i = 0; i < m_hJoiners.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hJoiners[i]);

		m_hPlayers.AddToHead( pPlayer );

		//Just in case.
		m_hSpectators.FindAndRemove( pPlayer );

		pPlayer->ResetRounds();
		pPlayer->SetArena(this);

		if (!pPlayer->GetTeam())
		{
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
		}
	}

	//Empty the queues.
	m_hJoiners.RemoveAll();
	m_hQuitters.RemoveAll();

	if (m_hPlayers.Count() <= 0)
	{
		SetThink( WaitingThink );
		SetNextThink( gpGlobals->curtime + 3.0 );
		return;
	}

	int iTeamsWithPlayers = 0;
	//Check to see which teams have players at all.
 	for (i = 0; i < m_hTeams.Count(); i++)
	{
		if (m_hTeams[i]->GetNumPlayers() > 0)
		{
			iTeamsWithPlayers += 1;
		}
	}

	if (iTeamsWithPlayers <= 1)
		bRoundStarting = false;

	for (i = 0; i < m_hPlayers.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);

		if (!pPlayer->IsAlive())
			pPlayer->Spawn();

		if (bRoundStarting || pPlayer->Rounds() == -1)
		{
			// Look away from the center of the map
			QAngle angLookAt = QAngle(0, 0, 0);
			CBaseEntity *pSpawnSpot = pPlayer->GetTeam()->SpawnPlayer(pPlayer);
			VectorAngles( pSpawnSpot->GetLocalOrigin() - m_vecSpawnAvg, angLookAt );
			angLookAt.x = 15;	//Look somewhat at the ground.

			pPlayer->SetLocalOrigin( pSpawnSpot->GetLocalOrigin() + Vector(0,0,1) );
			pPlayer->SetAbsVelocity( Vector(0,0,0) );
			pPlayer->SetLocalAngles( angLookAt );
			pPlayer->SnapEyeAngles( angLookAt );
		}

		if (bRoundStarting)
			pPlayer->LockPlayerInPlace();

		//-1 rounds denotes that the player has not been put in the arena yet.
		//The player should be in by now, so set it to zero.
		if (pPlayer->Rounds() == -1)
			pPlayer->AddRound();
	}

	for (i = 0; i < m_hTeams.Count(); i++)
	{
		m_hTeams[i]->ResetPlayersAlive();
	}

	if (!bRoundStarting)
	{
		SetThink( WaitingThink );
		SetNextThink( gpGlobals->curtime + 3.0 );
		return;
	}

	SetThink( BeginThink );
	SetNextThink( gpGlobals->curtime + 5.0 );
}

void CArena::BeginThink( )
{
	m_State = CArenaShared::GS_INPROGRESS;

	for (int i = 0; i < m_hPlayers.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);
		pPlayer->UnlockPlayer();
		pPlayer->DeployArmaments();
		pPlayer->AddRound();
	}

	CheckForRoundEnd();
}

// Check for the necessary conditions to end the round.
void CArena::CheckForRoundEnd( )
{
	int iTeamsBitmask = 0;	//Bitmask of teams with players alive
	int	iTeamsAlive = 0;	//Number of teams with players alive

 	for (int i = 0; i < m_hTeams.Count(); i++)
	{
		if (m_hTeams[i]->GetPlayersAlive() > 0)
		{
			iTeamsBitmask |= (1<<i);
			iTeamsAlive += 1;
		}
	}

	if (iTeamsAlive == 1)
		//The following log arithmetic is equal to log2(x) which gives us the team number from the bitmask.
		RoundEnd(log10((float)iTeamsBitmask)/log10((float)2));
	else if (iTeamsAlive == 0)
		RoundEnd(0);	//Everybody is dead for some reason! Round draw.
}

void CArena::RoundEnd( int iWinningTeam )
{
	m_State = CArenaShared::GS_VICTORY;

	SetThink( WaitingThink );
	SetNextThink( gpGlobals->curtime + 3.0 );
}

void CArena::StartTouch( CBaseEntity *pOther )
{
	BaseClass::StartTouch(pOther);

	if (!pOther->IsPlayer())
		return;

	CBasePlayer* pPlayer = ToBasePlayer( pOther );

	if (!pOther->IsPlayer())
		return;

	AddToArena(pPlayer);
}

void CArena::AddToArena( CBasePlayer *pPlayer )
{
	if (m_hPlayers.HasElement(pPlayer) || m_hSpectators.HasElement(pPlayer) || m_hJoiners.HasElement(pPlayer))
		return;

	if (pPlayer->GetArena() && pPlayer->GetArena() != this)
	{
		ClientPrint( pPlayer, HUD_PRINTCONSOLE, "Leave your current arena first.\n");
		return;
	}

	pPlayer->SetArena( this );

	m_hSpectators.AddToHead(pPlayer);

	ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("You are now in arena #%d.\n", m_iID+1) );
}

void CArena::EndTouch( CBaseEntity *pOther )
{
	BaseClass::EndTouch(pOther);

	if (!pOther->IsPlayer())
		return;

	CBasePlayer* pPlayer = ToBasePlayer( pOther );

	//Players who have died stop touching the arena, but will soon respawn.
	if (!pPlayer->IsAlive())
		return;

	RemoveFromArena(pPlayer);
}

void CArena::RemoveFromArena( CBasePlayer *pPlayer )
{
	if (m_hPlayers.HasElement(pPlayer))
		return;

	pPlayer->SetArena( NULL );

	m_hSpectators.FindAndRemove(pPlayer);

	ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("Now leaving arena #%d.\n", m_iID+1) );
}

void CBasePlayer::JoinGame()
{
	if (GetArena() == NULL)
		return;

	GetArena()->JoinPlayer( this );
}

void CArena::JoinPlayer( CBasePlayer *pPlayer )
{
	if (pPlayer->GetArena() && pPlayer->GetArena() != this)
	{
		ClientPrint( pPlayer, HUD_PRINTCONSOLE, "Leave your current arena first.\n");
		return;
	}

	m_hJoiners.AddToTail( pPlayer );

	ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("Joining game in arena #%d.\n", m_iID+1) );
}

void CBasePlayer::QuitGame()
{
	if (GetArena() == NULL)
		return;

	GetArena()->QuitPlayer( this );
}

void CArena::QuitPlayer( CBasePlayer *pPlayer )
{
	if (m_hPlayers.HasElement( pPlayer ) && !m_hQuitters.HasElement( pPlayer ))
		m_hQuitters.AddToTail( pPlayer );

	if (m_hJoiners.HasElement( pPlayer ))
		m_hJoiners.FindAndRemove( pPlayer );

	ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("Quitting game in arena #%d.\n", m_iID+1) );
}

bool CArena::HasPlayer( CBasePlayer *pPlayer )
{
	return m_hPlayers.HasElement(pPlayer);
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
	if (i < 0 || i >= s_hArenas.Count())
		return NULL;

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