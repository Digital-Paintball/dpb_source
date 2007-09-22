//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//
#include <cbase.h>
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
	DEFINE_FUNCTION( KeepTimeThink ),

	DEFINE_INPUT( m_iszName, FIELD_STRING, "ArenaName" ),
	DEFINE_INPUT( m_flMinutes, FIELD_FLOAT, "RoundTime" ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CArena, DT_Arena )
	SendPropInt( SENDINFO( m_State ), 2, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iID ), 4, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iRedTeamScore ), 4, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iBlueTeamScore ), 4, SPROP_UNSIGNED ),
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
	SetThink( &CArena::WaitingThink );

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
				pArena->m_iRedTeamScore = 0;
				pArena->m_iBlueTeamScore = 0; // reset team scores
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
		SetThink( &CArena::WaitingThink );
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
	{
		bRoundStarting = false;
		m_iRedTeamScore = 0;
		m_iBlueTeamScore = 0;
	}
	// jeff - put message here to notify clients no round - not enough players?

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
	//Then, add people who have switched teams, the scurvy dogs! - jeff
	for (i = 0; i < m_hSwitchersRed.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hSwitchersRed[i]);

		if (!pPlayer) // person got tired of waiting, apparently. asshat
			continue;

		pPlayer->ResetFragCount(); // all your scores are die when you switch teams. sorry
		SwitchTeam(pPlayer, 1); // red is team #1, blue is team #0. The devel plays on team -1. mwahaha.
	}
	// blue, too
	for (i = 0; i < m_hSwitchersBlue.Count(); i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hSwitchersBlue[i]);

		if (!pPlayer)
			continue;

		pPlayer->ResetFragCount(); // all your scores are die when you switch teams. sorry
		SwitchTeam(pPlayer, 0); 
	}


	//Empty the queues.
	m_hJoiners.RemoveAll();
	m_hQuitters.RemoveAll();
	m_hSwitchersRed.RemoveAll();
	m_hSwitchersBlue.RemoveAll();

	if (m_hPlayers.Count() <= 0)
	{
		SetThink( &CArena::WaitingThink );
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

	int availableSpawns[MAX_TEAMS];
	memset( availableSpawns, 0, sizeof(availableSpawns) );

	for( i = 0; i < m_hTeams.Count(); i++ )
		availableSpawns[i] = m_hTeams[i]->GetNumSpawnpoints();

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

		// find the proper team index
		int iTeamIndex = -1;
		for( int j = 0; j < m_hTeams.Count(); j++ )
		{
			if( pPlayer->GetTeam() != m_hTeams[j] )
				continue;

			iTeamIndex = j;
			break;
		}

		// we couldn't find the team index?
		Assert( iTeamIndex != -1 );

		if (!pPlayer->IsAlive())
			pPlayer->Spawn();

		// limit the number of players that spawn to the number of spawnpoints for that team
		if( availableSpawns[ iTeamIndex ] <= 0 )
		{
			pPlayer->StartObserverMode( 0 );
			continue;
		}
		
		availableSpawns[ iTeamIndex ]--;

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
				WRITE_BYTE( m_iID ); // what arena are we resetting
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
		SetThink( &CArena::WaitingThink );
		SetNextThink( gpGlobals->curtime + 3.0 );
		return;
	}

	SetThink( &CArena::BeginThink );
	SetNextThink( gpGlobals->curtime + 5.0 );
	if (m_flMinutes)
	{
		m_startedWhen = gpGlobals->curtime;
		m_willEnd = ( gpGlobals->curtime + m_flMinutes * 60 );

	}
	for (int i = 0; i < iPlayersCount; i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);
			
		if (!pPlayer)
		{
			m_hPlayers.FastRemove(i);
			continue;
		}
		if (this->HasPlayer(pPlayer)) 
		{
			ClientPrint( pPlayer, HUD_PRINTCENTER, "GET READY!" );
		}
	}
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
//		SetThink( TimesUpThink );
//		SetNextThink( gpGlobals->curtime + m_flMinutes * 60 );
	}
		
	SetThink( &CArena::KeepTimeThink );
	SetNextThink( gpGlobals->curtime + 1.0 );

	CheckForRoundEnd();

}

void CArena::TimesUpThink()
{
	RoundEnd(-1);
}

void CArena::KeepTimeThink() // jeff
{
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

		// jeff - client time updates go here
		CSingleUserRecipientFilter user( pPlayer );
			
		user.MakeReliable();
		UserMessageBegin( user, "Arena" );
			WRITE_BYTE( CArenaShared::AE_TIMEUPDATE );

			WRITE_BYTE( m_iID ); // tell them what arena we're in too
			int temp = (int)(m_willEnd - gpGlobals->curtime);
			WRITE_BYTE( temp );  
		MessageEnd();
	}


	DevMsg("%f seconds left in round! Time is %f Started %f, ends %f\n", m_willEnd - gpGlobals->curtime, gpGlobals->curtime, m_startedWhen, m_willEnd);
	if ((m_willEnd - gpGlobals->curtime) < 0)
		{		
			DevMsg("Ending round!");
			RoundEnd(-2);
			return;
		}

	SetThink( &CArena::KeepTimeThink );
	SetNextThink( gpGlobals->curtime + 1.0 );
}

// Check for the necessary conditions to end the round.
void CArena::CheckForRoundEnd( )
{
	//if ((m_State != CArenaShared::GS_INPROGRESS)||(m_State != CArenaShared::GS_VICTORY)) // if we check for the round end, but the round isn't in progress - jeff
	if (m_State == CArenaShared::GS_COUNTDOWN)
	{
		RoundEnd(-1); // just go to the default reset round, which is -1
		return;
	}
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
		RoundEnd(-1);	//Everybody is dead for some reason! Round draw.
}

void CArena::RoundEnd( int iWinningTeam )
{
//	if ( iWinningTeam != -1 ) // if the winning team is -1, we're forcing it because everyone is dead for some reason
	if ((iWinningTeam==-1)||(iWinningTeam==-2)) // jeff - do this whole thing better
	{
	int iPlayersCount = m_hPlayers.Count();
	for (int i = 0; i < iPlayersCount; i++)
	{
		CBasePlayer *pPlayer = ToBasePlayer(m_hPlayers[i]);
			
		if (!pPlayer)
		{
			m_hPlayers.FastRemove(i);
			continue;
		}
		if (this->HasPlayer(pPlayer)) // we only need to send them this message if they're like, in the arena.
		{
			if (iWinningTeam==-1) // warmup
			{
				ClientPrint( pPlayer, HUD_PRINTCENTER, "Warmup Mode\nWaiting for players..." );
			}
			if (iWinningTeam==-2) // timeup (consider replacing with whistle)
			{
				ClientPrint( pPlayer, HUD_PRINTCENTER, "Time's up!\nDraw!" );
			}
		}
	}

	
		
	}
	if 	((m_State != CArenaShared::GS_INPROGRESS))
		return;

	m_State = CArenaShared::GS_VICTORY;

	SetThink( &CArena::WaitingThink );
	SetNextThink( gpGlobals->curtime + 3.0 );

	CArenaRecipientFilter user( this );
	user.MakeReliable();
	UserMessageBegin( user, "Arena" );
		WRITE_BYTE( CArenaShared::AE_VICTORY ); // jeff - why are we sending the team that won as the player that won?
		WRITE_BYTE( iWinningTeam );
	MessageEnd();

	
	DevMsg("Team %i has won!\n", iWinningTeam);

	if (iWinningTeam==0)
		m_iBlueTeamScore++;
	if (iWinningTeam==1)
		m_iRedTeamScore++;

	UserMessageBegin( user, "Arena" );
		WRITE_BYTE( CArenaShared::AE_TVICTORY );
		WRITE_BYTE( m_iID );
		WRITE_BYTE( m_iRedTeamScore );
		WRITE_BYTE( m_iBlueTeamScore );
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
		ClientPrint( pPlayer, HUD_PRINTNOTIFY, "You must leave your current arena before joining a new one.\n");
		return;
	}

	pPlayer->SetArena( this );
	
	engine->ClientCommand(engine->PEntityOfEntIndex(pPlayer->entindex()), "ShowJoinArena" ); // jeff - show the join arena dialog
	m_hSpectators.AddToHead(pPlayer);
	
	ClientPrint( pPlayer, HUD_PRINTCENTER, UTIL_VarArgs("You are now able to join Arena #%d\n", m_iID+1) );
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

	pPlayer->ResetFragCount(); // jeff - set their score to zerrrooo
	pPlayer->SetArena( NULL );

	m_hSpectators.FindAndRemove(pPlayer);

	//ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs("Now leaving arena #%d.\n", m_iID+1) );
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
		ClientPrint( pPlayer, HUD_PRINTCENTER, "You must leave your current arena before you join another one.\n");
		return;
	}

	if (m_hJoiners.HasElement( pPlayer ) || m_hPlayers.HasElement( pPlayer ))
		return;

	for (int j = 0; j < s_hArenas.Count(); j++)
		if (s_hArenas[j]->HasPlayer( pPlayer ) || s_hArenas[j]->m_hJoiners.HasElement( pPlayer ))
			return;

	m_hJoiners.AddToTail( pPlayer );

	ClientPrint( pPlayer, HUD_PRINTCENTER, UTIL_VarArgs("Added to join queue for Arena #%d\n", m_iID+1) );
}

void CArena::SwitchQueueAdd( CBasePlayer *pPlayer, int newteam )
{
	// TODO - see todo in switchteam
	CTeam *pTeam = NULL;
	pTeam = m_hTeams[newteam];
	if (newteam==1) // red
	{
		m_hSwitchersRed.AddToTail (pPlayer);
		ClientPrint( pPlayer, HUD_PRINTCENTER, "You will switch to the red team after the end of the round." );
	}
	if (newteam==0) // blue
	{
		m_hSwitchersBlue.AddToTail (pPlayer);
		ClientPrint( pPlayer, HUD_PRINTCENTER, "You will switch to the blue team after the end of the round." );
	}
}

void CArena::SwitchTeam( CBasePlayer *pPlayer, int newteam )
{
	// at some point, this should check spawns and so forth, and probably account for team balancing. consider it a TODO - jeff
	CTeam *pTeam = NULL;
	pTeam = m_hTeams[newteam];
	pPlayer->ChangeTeam(pTeam->GetTeamNumber());
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

	ClientPrint( pPlayer, HUD_PRINTCENTER, UTIL_VarArgs("Added to queue to leave Arena #%d\n", m_iID+1) );
}

void CArena::RemoveQuitter( CBasePlayer *pPlayer )
{
	if (!m_hPlayers.HasElement(pPlayer) && !m_hQuitters.HasElement(pPlayer))
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
