//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "obstacle_pushaway.h"
#include "props_shared.h"

//-----------------------------------------------------------------------------------------------------
ConVar sv_pushaway_force( "sv_pushaway_force", "30000", FCVAR_REPLICATED, "How hard physics objects are pushed away from the players on the server." );
ConVar sv_pushaway_min_player_speed( "sv_pushaway_min_player_speed", "75", FCVAR_REPLICATED, "If a player is moving slower than this, don't push away physics objects (enables ducking behind things)." );
ConVar sv_pushaway_max_force( "sv_pushaway_max_force", "1000", FCVAR_REPLICATED, "Maximum amount of force applied to physics objects by players." );
ConVar sv_pushaway_clientside( "sv_pushaway_clientside", "0", FCVAR_REPLICATED, "Clientside physics push away (0=off, 1=only localplayer, 1=all players)" );
ConVar sv_turbophysics( "sv_turbophysics", "0", FCVAR_REPLICATED, "Turns on turbo physics" );

ConVar sv_pushaway_player_force( "sv_pushaway_player_force", "200000", FCVAR_REPLICATED | FCVAR_CHEAT, "How hard the player is pushed away from physics objects (falls off with inverse square of distance)." );
ConVar sv_pushaway_max_player_force( "sv_pushaway_max_player_force", "10000", FCVAR_REPLICATED | FCVAR_CHEAT, "Maximum of how hard the player is pushed away from physics objects." );

//-----------------------------------------------------------------------------------------------------
int GetPushawayEnts( CBaseCombatCharacter *pPushingEntity, CBaseEntity **ents, int nMaxEnts, float flPlayerExpand, int PartitionMask, CPushAwayEnumerator *enumerator )
{
	
	Vector vExpand( flPlayerExpand, flPlayerExpand, flPlayerExpand );

	Ray_t ray;
	ray.Init( pPushingEntity->GetAbsOrigin(), pPushingEntity->GetAbsOrigin(), pPushingEntity->GetCollideable()->OBBMins() - vExpand, pPushingEntity->GetCollideable()->OBBMaxs() + vExpand );

	CPushAwayEnumerator *physPropEnum = NULL;
	if  ( !enumerator )
	{
		physPropEnum = new CPushAwayEnumerator( ents, nMaxEnts );
		enumerator = physPropEnum;
	}

	partition->EnumerateElementsAlongRay( PartitionMask, ray, false, enumerator );

	int numHit = enumerator->m_nAlreadyHit;

	if ( physPropEnum )
		delete physPropEnum;

	return numHit;
}

//-----------------------------------------------------------------------------------------------------
void PerformObstaclePushaway( CBaseCombatCharacter *pPushingEntity )
{
	if (  pPushingEntity->m_lifeState != LIFE_ALIVE )
		return;

	// Give a push to any barrels that we're touching.
	// The client handles adjusting our usercmd to push us away.
	CBaseEntity *props[256];

#ifdef CLIENT_DLL
	// if sv_pushaway_clientside is disabled, clientside phys objects don't bounce away
	if ( sv_pushaway_clientside.GetInt() == 0 )
		return;

	// if sv_pushaway_clientside is 1, only local player can push them
	CBasePlayer *pPlayer = pPushingEntity->IsPlayer() ? (dynamic_cast< CBasePlayer * >(pPushingEntity)) : NULL;
	if ( (sv_pushaway_clientside.GetInt() == 1) && (!pPlayer || !pPlayer->IsLocalPlayer()) )
		return;

	int nEnts = GetPushawayEnts( pPushingEntity, props, ARRAYSIZE( props ), 3.0f, PARTITION_CLIENT_RESPONSIVE_EDICTS );
#else
	int nEnts = GetPushawayEnts( pPushingEntity, props, ARRAYSIZE( props ), 3.0f, PARTITION_ENGINE_SOLID_EDICTS );
#endif
	
	for ( int i=0; i < nEnts; i++ )
	{
		// If this entity uas PHYSICS_MULTIPLAYER_FULL set (ie: it's not just debris), and we're moving too slow, don't push it away.
		// Instead, let the client bounce off it. This allows players to get close to and duck behind things without knocking them over.
		IMultiplayerPhysics *pInterface = dynamic_cast<IMultiplayerPhysics*>( props[i] );

#ifdef GAME_DLL
		if ( pInterface->IsAsleep() && sv_turbophysics.GetBool() )
			continue;
#endif

		if ( pInterface && pInterface->GetMultiplayerPhysicsMode() == PHYSICS_MULTIPLAYER_SOLID )
		{
			if ( pPushingEntity->GetAbsVelocity().Length2D() < sv_pushaway_min_player_speed.GetFloat() )
				continue;
		}

		IPhysicsObject *pObj = props[i]->VPhysicsGetObject();

		if ( pObj )
		{		
			Vector vPushAway = (props[i]->WorldSpaceCenter() - pPushingEntity->WorldSpaceCenter());
			vPushAway.z = 0;
			
			float flDist = VectorNormalize( vPushAway );
			flDist = max( flDist, 1 );
			
			float flForce = sv_pushaway_force.GetFloat() / flDist;
			flForce = min( flForce, sv_pushaway_max_force.GetFloat() );

			pObj->ApplyForceOffset( vPushAway * flForce, pPushingEntity->WorldSpaceCenter() );
		}
	}
}