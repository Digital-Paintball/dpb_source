 //========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_sdkbase.h"
#include "sdk_fx_shared.h"
#include "in_buttons.h"
#include "paintballmgr.h"

#if defined( CLIENT_DLL )
	#define CWeaponMarker C_WeaponMarker
	#include "c_sdk_player.h"
#else
	#include "sdk_player.h"
#endif

// Should be last include
#include "tier0/memdbgon.h"

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

#ifndef _DEBUG
	if (!pPlayer->GetArena())
		return;

	if (!pPlayer->GetArena()->HasPlayer(pPlayer))
		return;
#endif

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

	// Fire paintball!
	Vector vecAiming	= pPlayer->GetAutoaimVector( 0 );
	Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	CPaintballMgr::GetManager()->AddBall( pPlayer );

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


