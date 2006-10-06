//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#include "multiarena.h"
#include "team.h"
#include "paintballmgr.h"
#include "viewport_panel_names.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_DATADESC( CArena )

	// Function Pointers
	DEFINE_FUNCTION( WaitingThink ),
	DEFINE_FUNCTION( BeginThink ),
	DEFINE_FUNCTION( TimesUpThink ),

	DEFINE_INPUT( m_iszName, FIELD_STRING, "ArenaName" ),
	DEFINE_INPUT( m_flMinutes, FIELD_FLOAT, "RoundTime" ),

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

	// Rounds should not be less then 30 seconds long.
	if (m_flMinutes < 0)
		m_flMinutes = 0;
	if (m_flMinutes > 0 && m_flMinutes < .5)
		m_flMinutes = .5;
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

	CPaintballMgr::GetManager()->RemoveBalls( this );

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

	// Fix things from last round.
	int iPlayersCount = m_hPlayers.Count();
	for (i = 0; i < iPlayersCount; i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);

		//Maybe the client has disconnected or something?
		//Get him out of here!
		if (!pPlayer)
		{
			m_hPlayers.FastRemove(i);
			continue;
		}

		if (!pPlayer->IsAlive())
			pPlayer->Spawn();

		if (pPlayer->GetActiveWeapon() && bRoundStarting)
		{
			pPlayer->GetActiveWeapon()->Holster();
			pPlayer->GetActiveWeapon()->FinishReload();
		}
	}

	// Remove people who have quit the game
	for (i = 0; i < m_hQuitters.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hQuitters[i]);

		if (!pPlayer)
			continue;

		RemoveQuitter(pPlayer);
	}

	//Then add people who have joined the game
	for (i = 0; i < m_hJoiners.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hJoiners[i]);

		if (!pPlayer)
			continue;

		if (m_hPlayers.HasElement( pPlayer ))
			continue;

		m_hPlayers.AddToHead( pPlayer );

		pPlayer->ResetRounds();
		pPlayer->SetArena(this);

		if (!pPlayer->GetTeam())
			AssignTeam(pPlayer);

		CSingleUserRecipientFilter user( pPlayer );
		user.MakeReliable();
		UserMessageBegin( user, "Arena" );
			WRITE_BYTE( CArenaShared::AE_JOIN );
			WRITE_BYTE( m_iID );
		MessageEnd();
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

	iTeamsWithPlayers = 0;
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

		if (!pPlayer)
		{
			m_hPlayers.FastRemove(i);
			continue;
		}

		pPlayer->SetArena(this);

		if (!pPlayer->GetTeam())
			AssignTeam(pPlayer);

		Assert(pPlayer->GetTeam());

		if (!pPlayer->IsAlive())
			pPlayer->Spawn();

		if (bRoundStarting || pPlayer->Rounds() == -1)
		{
			// Look away from the center of the map
			QAngle angLookAt = QAngle(0, 0, 0);
			CBaseEntity *pSpawnSpot = pPlayer->GetTeam()->SpawnPlayer(pPlayer);
			if (pSpawnSpot)
			{
				VectorAngles( pSpawnSpot->GetLocalOrigin() - m_vecSpawnAvg, angLookAt );
				angLookAt.x = 15;	//Look somewhat at the ground.

				MovePlayer(pPlayer, pSpawnSpot->GetLocalOrigin(), angLookAt);
				pPlayer->SnapEyeAngles( angLookAt );
			}
			else
				AssertMsg(0, UTIL_VarArgs("No spawn spot found for player %s on team %d in arena %d.\n", pPlayer->GetPlayerName(), pPlayer->GetTeamNumber(), m_iID));

			CSingleUserRecipientFilter user( pPlayer );
			user.MakeReliable();
			UserMessageBegin( user, "Arena" );
				WRITE_BYTE( CArenaShared::AE_RESET );
			MessageEnd();
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

	int iPlayersCount = m_hPlayers.Count();
	for (int i = 0; i < iPlayersCount; i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);

		//Maybe the client has disconnected or something?
		//Get him out of here!
		if (!pPlayer)
		{
			m_hPlayers.FastRemove(i);
			continue;
		}

		pPlayer->UnlockPlayer();
		pPlayer->DeployArmaments();
		pPlayer->AddRound();
	}

	if (m_flMinutes)
	{
		SetThink( TimesUpThink );
		SetNextThink( gpGlobals->curtime + m_flMinutes * 60 );
	}

	CheckForRoundEnd();
}

void CArena::TimesUpThink()
{
	RoundEnd(0);
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

	CArenaRecipientFilter user( this );
	user.MakeReliable();
	UserMessageBegin( user, "Arena" );
		WRITE_BYTE( CArenaShared::AE_VICTORY );
		WRITE_BYTE( iWinningTeam );
	MessageEnd();
}

void CArena::StartTouch( CBaseEntity *pOther )
{
	BaseClass::StartTouch(pOther);

	if (!pOther->IsPlayer())
		return;

	CBasePlayer* pPlayer = ToBasePlayer( pOther );

	AddToArena(pPlayer);
}

void CArena::AddToArena( CBasePlayer *pPlayer )
{
	if (m_hPlayers.HasElement(pPlayer) || m_hJoiners.HasElement(pPlayer))
	{
		pPlayer->FishOutOfWater(false);
		return;
	}

	if (pPlayer->GetArena() && pPlayer->GetArena() != this)
	{
		ClientPrint( pPlayer, HUD_PRINTCONSOLE, "Leave your current arena first.\n");
		return;
	}

	pPlayer->SetArena( this );
	
	// open arena panel on client showing JoinArena:
	//pPlayer->ShowViewPortPanel( PANEL_ARENAJOIN, true, NULL );
	//pPlayer->ClientCommnd( "ShowJoinArena" );
	engine->ClientCommand(engine->PEntityOfEntIndex(pPlayer->entindex()), "ShowJoinArena" );
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
	{
		pPlayer->FishOutOfWater(true);
		return;
	}

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

	if (m_hJoiners.HasElement( pPlayer ) || m_hPlayers.HasElement( pPlayer ))
		return;

	for (int j = 0; j < s_hArenas.Count(); j++)
		if (s_hArenas[j]->HasPlayer( pPlayer ) || s_hArenas[j]->m_hJoiners.HasElement( pPlayer ))
			return;

	m_hJoiners.AddToTail( pPlayer );

	ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("Joining game in arena #%d.\n", m_iID+1) );
}

void CArena::AssignTeam( CBasePlayer *pPlayer )
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
		Assert(0);
		return;
	}

	pPlayer->ChangeTeam(pTeam->GetTeamNumber());
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

	// If the player has made his way outside the arena for some reason, remove him immediately.
	if (pPlayer->FishOutOfWater() || !pPlayer->IsAlive())
	{
		RemoveQuitter(pPlayer);
		CheckForRoundEnd();
	}

	ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("Quitting game in arena #%d.\n", m_iID+1) );
}

void CArena::RemoveQuitter( CBasePlayer *pPlayer )
{
	if (!m_hPlayers.HasElement(pPlayer) || !m_hQuitters.HasElement(pPlayer))
		return;

	m_hPlayers.FindAndRemove(pPlayer);
	if (pPlayer->GetTeam())
		pPlayer->GetTeam()->RemovePlayer(pPlayer);

	pPlayer->RemoveAllItems(false);
	pPlayer->UnlockPlayer();

	SpawnPlayer(pPlayer, this);

	pPlayer->FishOutOfWater(false);

	if (!pPlayer->IsAlive())
		pPlayer->Spawn();

	CSingleUserRecipientFilter user( pPlayer );
	user.MakeReliable();
	UserMessageBegin( user, "Arena" );
		WRITE_BYTE( CArenaShared::AE_QUIT );
	MessageEnd();
}

void CArena::SpawnPlayer( CBasePlayer *pPlayer, CArena* pArena )
{
	CBaseEntity *pSpawnSpot = NULL;

	if (pArena)
		pSpawnSpot = pPlayer->EntSelectStartPoint( pArena );

	if (!pSpawnSpot)
		pSpawnSpot = pPlayer->EntSelectStartPoint();

	MovePlayer(pPlayer, pSpawnSpot->GetLocalOrigin(), pSpawnSpot->GetLocalAngles());
}

void CArena::MovePlayer(CBasePlayer *pPlayer, const Vector &vecOrigin, const QAngle &angAngles)
{
	Vector vecDrop = vecOrigin + Vector(0, 0, 64);

	trace_t trace;
	UTIL_TraceEntity( pPlayer, vecDrop, vecOrigin, MASK_PLAYERSOLID, pPlayer, COLLISION_GROUP_PLAYER_MOVEMENT, &trace );
			
	AssertMsg( !trace.allsolid && !trace.startsolid, "CArena::MovePlayer: allsolid or startsolid" );

	pPlayer->SetAbsOrigin( trace.endpos );
	pPlayer->SetAbsAngles( angAngles );
	pPlayer->SetAbsVelocity( vec3_origin );
	pPlayer->SnapEyeAngles( angAngles );
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

int CArena::GetNumberOfPlayers()
{
	int iPlayers = 0;
	for (int i = 0; i < m_hTeams.Count(); i++)
		iPlayers += m_hTeams[i]->m_aPlayers.Count();

	return iPlayers;
}