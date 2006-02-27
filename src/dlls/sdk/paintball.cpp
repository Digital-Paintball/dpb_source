 //========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "paintball.h"

// Should be last include
#include "tier0/memdbgon.h"

void CPaintball::Init( int i )
{
	IPaintball::Init(i);
}

void CPaintball::Spawn( void )
{
}

void CPaintball::Destroy( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CPaintball::PaintballTouch( CBaseEntity *pOther, CGameTrace* pTrace )
{
	if ( pOther->m_takedamage != DAMAGE_NO )
	{
		ClearMultiDamage();

		Vector	vecNormalizedVel = m_vecVelocity;
		VectorNormalize( vecNormalizedVel );

		if( m_pOwner && m_pOwner->IsPlayer() && pOther->IsNPC() )
		{
			CTakeDamageInfo	dmgInfo( m_pOwner, m_pOwner, 1000, DMG_NEVERGIB );
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, pTrace->endpos, 0.7f );
			dmgInfo.SetDamagePosition( pTrace->endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, pTrace );
		}
		else
		{
			CTakeDamageInfo	dmgInfo( m_pOwner, m_pOwner, 1000, DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, pTrace->endpos, 0.7f );
			dmgInfo.SetDamagePosition( pTrace->endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, pTrace );
		}

		ApplyMultiDamage();
	}
	else
	{
		// See if we struck the world
		if ( pOther->GetMoveType() == MOVETYPE_NONE && !( pTrace->surface.flags & SURF_SKY ) )
		{
			UTIL_ImpactTrace( pTrace, DMG_BULLET, "Splat" );
		}
		else
		{
			// Put a mark unless we've hit the sky
			if ( ( pTrace->surface.flags & SURF_SKY ) == false )
			{
				UTIL_ImpactTrace( pTrace, DMG_BULLET, "Splat" );
			}
		}
	}
}