//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Team spawnpoint handling
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "entitylist.h"
#include "entityoutput.h"
#include "player.h"
#include "eventqueue.h"
#include "gamerules.h"
#include "team_spawnpoint.h"
#include "team.h"
#include "multiarena.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( info_player_teamspawn, CTeamSpawnPoint );

BEGIN_DATADESC( CTeamSpawnPoint )

END_DATADESC()


//-----------------------------------------------------------------------------
// Purpose: Attach this spawnpoint to it's team
//-----------------------------------------------------------------------------
void CTeamSpawnPoint::Activate( void )
{
	BaseClass::Activate();

	if ( !GetArena() )
	{
		Warning( "info_player_teamspawn at (%f, %f, %f) is not in an arena!\n", GetAbsOrigin().x, GetAbsOrigin().y, GetAbsOrigin().z );
		UTIL_Remove( this );
		return;
	}

	if ( GetTeamNumber() > 0 && GetTeamNumber() <= MAX_TEAMS && GetTeam() )
	{
		int iDrop = UTIL_DropToFloor(this, MASK_PLAYERSOLID_BRUSHONLY);

		AssertMsg(iDrop >= 0, UTIL_VarArgs("Spawn point at %f %f %f is in the floor. Move it up some.\n", GetAbsOrigin().x, GetAbsOrigin().y, GetAbsOrigin().z));

		GetTeam()->AddSpawnpoint( this );
	}
	else
	{
		Warning( "info_player_teamspawn with invalid team number: %d\n", GetTeamNumber() );
		UTIL_Remove( this );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Is this spawnpoint ready for a player to spawn in?
//-----------------------------------------------------------------------------
bool CTeamSpawnPoint::IsValid( CBasePlayer *pPlayer )
{
	CBaseEntity *ent = NULL;
	for ( CEntitySphereQuery sphere( GetAbsOrigin(), 40 ); ( ent = sphere.GetCurrentEntity() ) != NULL; sphere.NextEntity() )
	{
		// if ent is a client, don't spawn on 'em
		CBaseEntity *plent = ent;
		if ( plent && plent->IsPlayer() && plent != pPlayer )
			return false;
	}

	return true;
}

//===========================================================================================================
// VEHICLE SPAWNPOINTS
//===========================================================================================================
LINK_ENTITY_TO_CLASS( info_vehicle_groundspawn, CTeamVehicleSpawnPoint );

BEGIN_DATADESC( CTeamVehicleSpawnPoint )

	// outputs
	DEFINE_OUTPUT( m_OnVehicleSpawn, "OnVehicleSpawn" ),

END_DATADESC()


//-----------------------------------------------------------------------------
// Purpose: Is this spawnpoint ready for a vehicle to spawn in?
//-----------------------------------------------------------------------------
bool CTeamVehicleSpawnPoint::IsValid( void )
{
	CBaseEntity *ent = NULL;
	for ( CEntitySphereQuery sphere( GetAbsOrigin(), 128 ); ( ent = sphere.GetCurrentEntity() ) != NULL; sphere.NextEntity() )
	{
		// if ent is a client, don't spawn on 'em
		CBaseEntity *plent = ent;
		if ( plent && plent->IsPlayer() )
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Attach this spawnpoint to it's team
//-----------------------------------------------------------------------------
void CTeamVehicleSpawnPoint::Activate( void )
{
	BaseClass::Activate();
	if ( GetTeamNumber() > 0 && GetTeamNumber() <= MAX_TEAMS )
	{
		// Don't add vehicle spawnpoints to the team for now
		//GetGlobalTeam( GetTeamNumber() )->AddSpawnpoint( this );
	}
	else
	{
		Warning( "info_vehicle_groundspawn with invalid team number: %d\n", GetTeamNumber() );
		UTIL_Remove( this );
	}
}
