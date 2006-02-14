 //========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_sdkbase.h"
#include "sdk_fx_shared.h"
#include "in_buttons.h"

#if defined( CLIENT_DLL )

	#define CWeaponMarker C_WeaponMarker
	#include "c_sdk_player.h"
	#include "c_te_effect_dispatch.h"	// weapon_paintball
#else

	#include "sdk_player.h"
	#include "te_effect_dispatch.h"		// From here down, used for weapon_paintball
	#include "IEffects.h"
	#include "Sprite.h"
	#include "SpriteTrail.h"
	#include "beam_shared.h"

#endif

#define PAINTBALL_MODEL				"models/paintball/paintball.mdl"
#define PAINTBALL_AIR_VELOCITY		3500
#define PAINTBALL_WATER_VELOCITY	1500
#define	PAINTBALL_SKIN_NORMAL		0
#define PAINTBALL_SKIN_GLOW			1

#include "effect_dispatch_data.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Weapon Paintball
//-----------------------------------------------------------------------------
#ifndef CLIENT_DLL

extern ConVar sk_plr_dmg_crossbow;
extern ConVar sk_npc_dmg_crossbow;

void TE_StickyBolt( IRecipientFilter& filter, float delay,	Vector vecDirection, const Vector *origin );

// CPaintball class
class CPaintball : public CBaseCombatCharacter
{
	DECLARE_CLASS( CPaintball, CBaseCombatCharacter );

public:
	CPaintball() { };
	~CPaintball();

	Class_T Classify( void ) { return CLASS_NONE; }

public:
	void Spawn( void );
	void Precache( void );
	void BubbleThink( void );
	void PaintballTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CPaintball *PaintballCreate( const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner = NULL );

protected:

	bool	CreateSprites( void );

	CHandle<CSprite>		m_pGlowSprite;
	//CHandle<CSpriteTrail>	m_pGlowTrail;
	
	int		m_iDamage;

	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
};
LINK_ENTITY_TO_CLASS( weapon_paintball, CPaintball );
PRECACHE_REGISTER( weapon_paintball );

BEGIN_DATADESC( CPaintball )
	// Function Pointers
	DEFINE_FUNCTION( BubbleThink ),
	DEFINE_FUNCTION( PaintballTouch ),

	// These are recreated on reload, they don't need storage
	DEFINE_FIELD( m_pGlowSprite, FIELD_EHANDLE ),
	//DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CPaintball, DT_Paintball )
END_SEND_TABLE()

CPaintball *CPaintball::PaintballCreate( const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner )
{
	// Create a new entity with CPaintball private data
	CPaintball *pPB = (CPaintball *)CreateEntityByName( "weapon_paintball" );
	UTIL_SetOrigin( pPB, vecOrigin );
	pPB->SetAbsAngles( angAngles );
	pPB->Spawn();
	pPB->SetOwnerEntity( pentOwner );

	pPB->m_iDamage = iDamage;

	return pPB;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CPaintball::~CPaintball( void )
{
	if ( m_pGlowSprite )
	{
		UTIL_Remove( m_pGlowSprite );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CPaintball::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, FSOLID_NOT_STANDABLE, false );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CPaintball::PhysicsSolidMaskForEntity() const
{
	return ( BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX ) & ~CONTENTS_GRATE;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CPaintball::CreateSprites( void )
{
	/*
	// Start up the eye glow
	m_pGlowSprite = CSprite::SpriteCreate( "sprites/light_glow02_noz.vmt", GetLocalOrigin(), false );

	if ( m_pGlowSprite != NULL )
	{
		m_pGlowSprite->FollowEntity( this );
		m_pGlowSprite->SetTransparency( kRenderGlow, 255, 255, 255, 128, kRenderFxNoDissipation );
		m_pGlowSprite->SetScale( 0.2f );
		m_pGlowSprite->TurnOff();
	}
	*/

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPaintball::Spawn( void )
{
	Precache( );

	SetModel( PAINTBALL_MODEL );
	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
	UTIL_SetSize( this, -Vector(1,1,1), Vector(1,1,1) );
	SetSolid( SOLID_BBOX );
	SetGravity( 1.0f );
	
	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch( &CPaintball::PaintballTouch );

	SetThink( &CPaintball::BubbleThink );
	SetNextThink( gpGlobals->curtime + 0.1f );
	
	CreateSprites();

	// Make us glow until we've hit the wall
	//m_nSkin = PAINTBALL_SKIN_GLOW;
}


void CPaintball::Precache( void )
{
	PrecacheModel( PAINTBALL_MODEL );

	// This is used by C_TEStickyBolt, despte being different from above!!!
	//PrecacheModel( "models/weapon_paintball.mdl" );

	//PrecacheModel( "sprites/light_glow02_noz.vmt" );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CPaintball::PaintballTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsSolid() || pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS) )
		return;

	if ( pOther->m_takedamage != DAMAGE_NO )
	{
		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector	vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize( vecNormalizedVel );

		if( GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC() )
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_iDamage, DMG_NEVERGIB );
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}
		else
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_iDamage, DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}

		ApplyMultiDamage();

		//Adrian: keep going through the glass.
		if ( pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS )
			 return;

		SetAbsVelocity( Vector( 0, 0, 0 ) );

		// play body "thwack" sound
		//EmitSound( "Weapon_Crossbow.BoltHitBody" );

		Vector vForward;

		AngleVectors( GetAbsAngles(), &vForward );
		VectorNormalize ( vForward );

		UTIL_TraceLine( GetAbsOrigin(),	GetAbsOrigin() + vForward * 128, MASK_OPAQUE, pOther, COLLISION_GROUP_NONE, &tr2 );

		if ( tr2.fraction != 1.0f )
		{
//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

			if ( tr2.m_pEnt == NULL || ( tr2.m_pEnt && tr2.m_pEnt->GetMoveType() == MOVETYPE_NONE ) )
			{
				CEffectData	data;

				data.m_vOrigin = tr2.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = tr2.fraction != 1.0f;
			
				//DispatchEffect( "BoltImpact", data );
			}
		}
		
		SetTouch( NULL );
		SetThink( NULL );

		UTIL_Remove( this );
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if ( pOther->GetMoveType() == MOVETYPE_NONE && !( tr.surface.flags & SURF_SKY ) )
		{
			//EmitSound( "Weapon_Crossbow.BoltHitWorld" );

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();
			float speed = VectorNormalize( vecDir );

			// See if we should reflect off this surface
			float hitDot = DotProduct( tr.plane.normal, -vecDir );
			
			if ( ( hitDot < 0.1f ) && ( speed > 100 ) )
			{
				Vector vReflection = 2.0f * tr.plane.normal * hitDot + vecDir;
				QAngle reflectAngles;
				VectorAngles( vReflection, reflectAngles );
				SetLocalAngles( reflectAngles );
				SetAbsVelocity( vReflection * speed * 0.75f );

				// Start to sink faster
				SetGravity( 1.0f );
			}
			else
			{
				SetThink( &CPaintball::SUB_Remove );
				SetNextThink( gpGlobals->curtime + 2.0f );
				
				//FIXME: We actually want to stick (with hierarchy) to what we've hit
				SetMoveType( MOVETYPE_NONE );
			
				Vector vForward;

				AngleVectors( GetAbsAngles(), &vForward );
				VectorNormalize ( vForward );

				CEffectData	data;

				data.m_vOrigin = tr.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = 0;
			
				//DispatchEffect( "BoltImpact", data );
				
				UTIL_ImpactTrace( &tr, DMG_BULLET );

				AddEffects( EF_NODRAW );
				SetTouch( NULL );
				SetThink( &CPaintball::SUB_Remove );
				SetNextThink( gpGlobals->curtime + 2.0f );

				if ( m_pGlowSprite != NULL )
				{
					m_pGlowSprite->TurnOn();
					m_pGlowSprite->FadeAndDie( 3.0f );
				}
			}
		}
		else
		{
			// Put a mark unless we've hit the sky
			if ( ( tr.surface.flags & SURF_SKY ) == false )
			{
				UTIL_ImpactTrace( &tr, DMG_BULLET );
			}

			UTIL_Remove( this );
		}
	}

	if ( g_pGameRules->IsMultiplayer() )
	{
//		SetThink( &CPaintball::ExplodeThink );
//		SetNextThink( gpGlobals->curtime + 0.1f );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPaintball::BubbleThink( void )
{
	QAngle angNewAngles;

	VectorAngles( GetAbsVelocity(), angNewAngles );
	SetAbsAngles( angNewAngles );

	SetNextThink( gpGlobals->curtime + 0.1f );

	if ( GetWaterLevel()  == 0 )
		return;

	UTIL_BubbleTrail( GetAbsOrigin() - GetAbsVelocity() * 0.1f, GetAbsOrigin(), 5 );
}

#endif

//-----------------------------------------------------------------------------
// Weapon Marker
//-----------------------------------------------------------------------------
class CWeaponMarker : public CWeaponSDKBase
{
public:
	DECLARE_CLASS( CWeaponMarker, CWeaponSDKBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CWeaponMarker();

	virtual void PrimaryAttack();
	virtual bool Deploy();
	virtual void WeaponIdle();

	virtual bool Reload();
	virtual	void CheckReload( void );

	virtual SDKWeaponID GetWeaponID( void ) const		{ return WEAPON_MARKER; }


private:

	CWeaponMarker( const CWeaponMarker & );

	void Fire( float flSpread );

	int	m_iReloadStage;
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponMarker, DT_WeaponMarker )

BEGIN_NETWORK_TABLE( CWeaponMarker, DT_WeaponMarker )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponMarker )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_marker, CWeaponMarker );
PRECACHE_WEAPON_REGISTER( weapon_marker );



CWeaponMarker::CWeaponMarker()
{
}

bool CWeaponMarker::Deploy( )
{
	CSDKPlayer *pPlayer = GetPlayerOwner();
	pPlayer->m_iShotsFired = 0;

	return BaseClass::Deploy();
}

bool CWeaponMarker::Reload( )
{
	CSDKPlayer *pPlayer = GetPlayerOwner();

	if (pPlayer->GetAmmoCount( GetPrimaryAmmoType() ) <= 0)
		return false;

	int iResult = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD1A );
	if ( !iResult )
		return false;

	m_iReloadStage = ACT_VM_RELOAD1A;

	pPlayer->SetAnimation( PLAYER_RELOAD );

#ifdef GAME_DLL
	SendReloadEvents();
#endif

#ifndef CLIENT_DLL
	if ((iResult) && (pPlayer->GetFOV() != pPlayer->GetDefaultFOV()))
	{
		pPlayer->SetFOV( pPlayer, pPlayer->GetDefaultFOV() );
	}
#endif

	pPlayer->m_iShotsFired = 0;

	return true;
}

void CWeaponMarker::CheckReload( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	AssertMsg(pOwner, "Weapon has no owner.\n");
	if (!pOwner)
		return;

	if ( (m_bInReload) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		switch (m_iReloadStage)
		{
		case ACT_VM_RELOAD1A:
			{
				DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD1B );
				m_iReloadStage = ACT_VM_RELOAD1B;
				break;
			}
		case ACT_VM_RELOAD1B:
			{
				DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD2 );
				m_iReloadStage = ACT_VM_RELOAD2;
				break;
			}
		case ACT_VM_RELOAD2:
			{
				if (pOwner->m_nButtons & IN_RELOAD)
				{
					DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD2 );
				}
				else
				{
					DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD3 );
					m_iReloadStage = ACT_VM_RELOAD3;
				}
				break;
			}
		case ACT_VM_RELOAD3:
			{
				FinishReload();
				m_flNextPrimaryAttack	= gpGlobals->curtime;
				m_flNextSecondaryAttack = gpGlobals->curtime;
				m_bInReload = false;
			}
		}
	}
}

void CWeaponMarker::PrimaryAttack( void )
{
	const CSDKWeaponInfo &pWeaponInfo = GetSDKWpnData();
	CSDKPlayer *pPlayer = GetPlayerOwner();

	float flCycleTime = pWeaponInfo.m_flCycleTime;

	float flSpread = 0.01f;

	// more spread when jumping
	if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		flSpread = 0.05f;
	
	pPlayer->m_iShotsFired++;

	// Out of ammo?
	if ( m_iClip1 <= 0 )
	{
		if (m_bFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2;
		}
	}

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	m_iClip1--;

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	/*
	FX_FireBullets(
		pPlayer->entindex(),
		pPlayer->Weapon_ShootPosition(),
		pPlayer->EyeAngles() + pPlayer->GetPunchAngle(),
		GetWeaponID(),
		bPrimaryMode?Primary_Mode:Secondary_Mode,
		CBaseEntity::GetPredictionRandomSeed() & 255,
		flSpread );
	
	pPlayer->DoMuzzleFlash();
	*/

#ifndef CLIENT_DLL

	// Fire paintball!
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	Vector vecAiming	= pOwner->GetAutoaimVector( 0 );	
	Vector vecSrc		= pOwner->Weapon_ShootPosition();
	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );
	CPaintball *pPB = CPaintball::PaintballCreate( vecSrc, angAiming, 1000, pOwner );

	if ( pOwner->GetWaterLevel() == 3 )
	{
		pPB->SetAbsVelocity( vecAiming * PAINTBALL_WATER_VELOCITY );
	}
	else
	{
		pPB->SetAbsVelocity( vecAiming * PAINTBALL_AIR_VELOCITY );
	}

#endif

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + flCycleTime;

	if (!m_iClip1 && pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", false, 0);
	}

	// start idle animation in 5 seconds
	SetWeaponIdleTime( gpGlobals->curtime + 5.0 );
}

void CWeaponMarker::WeaponIdle()
{
	if (m_flTimeWeaponIdle > gpGlobals->curtime)
		return;

	// only idle if the slid isn't back
	if ( m_iClip1 != 0 )
	{
		SetWeaponIdleTime( gpGlobals->curtime + 5.0f );
		SendWeaponAnim( ACT_VM_IDLE );
	}
}


