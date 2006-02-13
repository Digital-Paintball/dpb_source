//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Team management class. Contains all the details for a specific team
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "team.h"
#include "player.h"
#include "team_spawnpoint.h"
#include "multiarena.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: SendProxy that converts the Team's player UtlVector to entindexes
//-----------------------------------------------------------------------------
void SendProxy_PlayerList( const SendProp *pProp, const void *pStruct, const void *pData, DVariant *pOut, int iElement, int objectID )
{
	CTeam *pTeam = (CTeam*)pData;

	// If this assertion fails, then SendProxyArrayLength_PlayerArray must have failed.
	Assert( iElement < pTeam->m_aPlayers.Size() );

	CBasePlayer *pPlayer = pTeam->m_aPlayers[iElement];
	pOut->m_Int = pPlayer->entindex();
}


int SendProxyArrayLength_PlayerArray( const void *pStruct, int objectID )
{
	CTeam *pTeam = (CTeam*)pStruct;
	return pTeam->m_aPlayers.Count();
}


// Datatable
IMPLEMENT_SERVERCLASS_ST_NOBASE(CTeam, DT_Team)
	SendPropInt( SENDINFO(m_iTeamNum), 5 ),
	SendPropInt( SENDINFO(m_iScore), 8 ),
	SendPropString( SENDINFO( m_szTeamname ) ),

	SendPropArray2( 
		SendProxyArrayLength_PlayerArray,
		SendPropInt("player_array_element", 0, 4, 10, SPROP_UNSIGNED, SendProxy_PlayerList), 
		MAX_PLAYERS, 
		0, 
		"player_array"
		)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( team_manager, CTeam );

//-----------------------------------------------------------------------------
// Purpose: Needed because this is an entity, but should never be used
//-----------------------------------------------------------------------------
CTeam::CTeam( void )
{
	memset( m_szTeamname.GetForModify(), 0, sizeof(m_szTeamname) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTeam::~CTeam( void )
{
	m_aSpawnPoints.Purge();
	m_aPlayers.Purge();
}

//-----------------------------------------------------------------------------
// Purpose: Called every frame
//-----------------------------------------------------------------------------
void CTeam::Think( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Teams are always transmitted to clients
//-----------------------------------------------------------------------------
int CTeam::UpdateTransmitState()
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}

//-----------------------------------------------------------------------------
// Visibility/scanners
//-----------------------------------------------------------------------------
bool CTeam::ShouldTransmitToPlayer( CBasePlayer* pRecipient, CBaseEntity* pEntity )
{
	return false;
}

//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------
void CTeam::Init( const char *pName, int iNumber, CArena *pArena )
{
	InitializeSpawnpoints();
	InitializePlayers();

	m_iScore = 0;

	Q_strncpy( m_szTeamname.GetForModify(), pName, MAX_TEAM_NAME_LENGTH );
	m_iTeamNum = iNumber;

	AssertMsg(pArena, "Team initialized with NULL arena.");
	SetArena(pArena);
}

//-----------------------------------------------------------------------------
// DATA HANDLING
//-----------------------------------------------------------------------------
int CTeam::GetTeamNumber( void )
{
	return m_iTeamNum;
}

//-----------------------------------------------------------------------------
// Purpose: Get the team's name
//-----------------------------------------------------------------------------
const char *CTeam::GetName( void )
{
	return m_szTeamname;
}


//-----------------------------------------------------------------------------
// Purpose: Update the player's client data
//-----------------------------------------------------------------------------
void CTeam::UpdateClientData( CBasePlayer *pPlayer )
{
}

//------------------------------------------------------------------------------------------------------------------
// SPAWNPOINTS
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTeam::InitializeSpawnpoints( void )
{
	m_iLastSpawn = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTeam::AddSpawnpoint( CTeamSpawnPoint *pSpawnpoint )
{
	m_aSpawnPoints.AddToTail( pSpawnpoint );
	GetArena()->CalculateSpawnAvg();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTeam::RemoveSpawnpoint( CTeamSpawnPoint *pSpawnpoint )
{
	for (int i = 0; i < m_aSpawnPoints.Size(); i++ )
	{
		if ( m_aSpawnPoints[i] == pSpawnpoint )
		{
			m_aSpawnPoints.Remove( i );
			return;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Spawn the player at one of this team's spawnpoints. Return true if successful.
//-----------------------------------------------------------------------------
CBaseEntity *CTeam::SpawnPlayer( CBasePlayer *pPlayer )
{
	if ( m_aSpawnPoints.Size() == 0 )
		return NULL;

	// Randomize the start spot
	int iSpawn = m_iLastSpawn + random->RandomInt( 1,3 );
	if ( iSpawn >= m_aSpawnPoints.Size() )
		iSpawn -= m_aSpawnPoints.Size();
	int iStartingSpawn = iSpawn;

	// Now loop through the spawnpoints and pick one
	int loopCount = 0;
	do 
	{
		if ( iSpawn >= m_aSpawnPoints.Size() )
		{
			++loopCount;
			iSpawn = 0;
		}

		// check if pSpot is valid, and that the player is on the right team
		if ( (loopCount > 3) || m_aSpawnPoints[iSpawn]->IsValid( pPlayer ) )
		{
			// DevMsg( 1, "player: spawning at (%s)\n", STRING(m_aSpawnPoints[iSpawn]->m_iName) );
			m_aSpawnPoints[iSpawn]->m_OnPlayerSpawn.FireOutput( pPlayer, m_aSpawnPoints[iSpawn] );

			m_iLastSpawn = iSpawn;
			return m_aSpawnPoints[iSpawn];
		}

		iSpawn++;
	} while ( iSpawn != iStartingSpawn ); // loop if we're not back to the start

	return NULL;
}

void CTeam::AverageSpawns( void )
{
	m_vecSpawnAvg = Vector(0, 0, 0);

	if (!m_aSpawnPoints.Count())
		return;

	for (int i = 0; i < m_aSpawnPoints.Count(); i++)
	{
		m_vecSpawnAvg += m_aSpawnPoints[i]->GetAbsOrigin();
	}
	m_vecSpawnAvg /= m_aSpawnPoints.Count();
}

//------------------------------------------------------------------------------------------------------------------
// PLAYERS
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTeam::InitializePlayers( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Add the specified player to this team. Remove them from their current team, if any.
//-----------------------------------------------------------------------------
void CTeam::AddPlayer( CBasePlayer *pPlayer )
{
	m_aPlayers.AddToTail( pPlayer );
	NetworkStateChanged();
}

//-----------------------------------------------------------------------------
// Purpose: Remove this player from the team
//-----------------------------------------------------------------------------
void CTeam::RemovePlayer( CBasePlayer *pPlayer )
{
	if (!m_aPlayers.HasElement(pPlayer))
		return;

	if (pPlayer->IsAlive())
		m_iPlayersAlive--;

	m_aPlayers.FindAndRemove( pPlayer );
	pPlayer->ResetTeam();
	NetworkStateChanged();
}

//-----------------------------------------------------------------------------
// Purpose: Return the number of players in this team.
//-----------------------------------------------------------------------------
int CTeam::GetNumPlayers( void )
{
	return m_aPlayers.Size();
}

//-----------------------------------------------------------------------------
// Purpose: Get a specific player
//-----------------------------------------------------------------------------
CBasePlayer *CTeam::GetPlayer( int iIndex )
{
	Assert( iIndex >= 0 && iIndex < m_aPlayers.Size() );
	return m_aPlayers[ iIndex ];
}

//-----------------------------------------------------------------------------
// Purpose: Player on this team was killed
//-----------------------------------------------------------------------------
void CTeam::ResetPlayersAlive( )
{
	m_iPlayersAlive = m_aPlayers.Count();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTeam::PlayerKilled( CBasePlayer *pPlayer )
{
	if (m_aPlayers.HasElement(pPlayer))
		m_iPlayersAlive--;

	GetArena()->CheckForRoundEnd();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTeam::PlayerSpawn( CBasePlayer *pPlayer )
{
	if (m_aPlayers.HasElement(pPlayer))
		m_iPlayersAlive++;
}

//------------------------------------------------------------------------------------------------------------------
// SCORING
//-----------------------------------------------------------------------------
// Purpose: Add / Remove score for this team
//-----------------------------------------------------------------------------
void CTeam::AddScore( int iScore )
{
	m_iScore += iScore;
}

void CTeam::SetScore( int iScore )
{
	m_iScore = iScore;
}

//-----------------------------------------------------------------------------
// Purpose: Get this team's score
//-----------------------------------------------------------------------------
int CTeam::GetScore( void )
{
	return m_iScore;
}

