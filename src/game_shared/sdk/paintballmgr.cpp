 //========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "usermessages.h"
#include "paintballmgr.h"
#include "paintball_shared.h"
#include "movevars_shared.h"

#ifdef CLIENT_DLL
#include "hud_macros.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CPaintballMgr CPaintballMgr::s_PaintballMgr;

#ifdef CLIENT_DLL
void __MsgFunc_Paintball(bf_read &msg)
{
	C_BaseEntity *pEnt = cl_entitylist->GetEnt(msg.ReadByte());
	Assert(pEnt->IsPlayer());
	CPaintballMgr::GetManager()->AddBall((C_BasePlayer*)pEnt);
}
#endif

void CPaintballMgr::LevelInitPreEntity()
{
	for (int i = 0; i < MAX_PAINTBALLS; i++)
	{
		m_aBalls[i].Init(i);
	}

	m_iBalls = 0;
	m_iLastBall = 0;
	m_iFirstAvailable = 0;

#ifdef CLIENT_DLL
	HOOK_MESSAGE(Paintball);
#endif

	m_bInitialized = true;
}

int CPaintballMgr::FindEmptySpace( )
{
	int i;
	for (i = m_iFirstAvailable; i < MAX_PAINTBALLS; i++)
	{
		if (m_aBalls[i].IsAvailable())
			return i;
	}

	//Not enough space? Sucks to be you.
	return -1;

/*	//Not enough space? Grow the memory.
	int h = m_hBalls.Count();
	m_hBalls.Grow(1);		//This grows by the grow size, not 1.

	//Initialize new balls.
	for (i = h; i < m_hBalls.Count(); i++)
	{
		m_hBalls[i].Init();
	}

	return h;*/
}

//Kujar lieks milk.
void CPaintballMgr::AddBall( CBasePlayer *pOwner )
{
	int i = FindEmptySpace();
	if (i < 0 || i >= MAX_PAINTBALLS)
	{
#ifdef CLIENT_DLL
		AssertMsg(0, VarArgs("Client paintball index %d invalid.\n", i));
#else
		AssertMsg(0, UTIL_VarArgs("Server paintball index %d invalid.\n", i));
#endif
		return;
	}

#ifdef GAME_DLL
	CArenaRecipientFilter filter( pOwner->GetArena(), true );
	filter.RemoveRecipientsByInvPVS( pOwner->GetAbsOrigin() );
	filter.UsePredictionRules();

	UserMessageBegin( filter, "Paintball" );
		WRITE_BYTE( engine->IndexOfEdict(pOwner->edict()) );
	MessageEnd();
#endif

	CPaintball *pPB = &m_aBalls[i];

	Assert(pPB->m_bAvailable);
	pPB->m_bAvailable = false;
	pPB->m_vecPosition = pOwner->Weapon_ShootPosition();
	pPB->m_vecVelocity = pOwner->GetAutoaimVector( 0 ) * PAINTBALL_AIR_VELOCITY;
	pPB->m_pOwner = pOwner;

	m_iBalls++;
	if (i > m_iLastBall)
		m_iLastBall = i;
	if (i == m_iFirstAvailable)
		m_iFirstAvailable = FindEmptySpace();

	pPB->Spawn();
}

void CPaintballMgr::RemoveBall( int i )
{
	if (i < 0 || i >= MAX_PAINTBALLS)
		return;

	CPaintball *pPB = &m_aBalls[i];

	Assert(!pPB->m_bAvailable);
	pPB->m_bAvailable = true;

	m_iBalls--;
	if (i == m_iLastBall)
	{
		for (int j = m_iLastBall-1; j >= 0; j--)
		{
			if (m_aBalls[j].IsUsed())
			{
				m_iLastBall = j;
				break;
			}
		}
		m_iLastBall = 0;
	}
	if (i < m_iFirstAvailable)
		m_iFirstAvailable = i;

	pPB->Destroy();
}

void CPaintballMgr::THINK_PROTOTYPE
{
	//Don't think before we're initialized.
	if (!m_bInitialized)
		return;

#ifdef GAME_DLL
	float flFrametime = gpGlobals->frametime;
#endif

	for (int i = 0; i <= m_iLastBall; i++)
	{
		if (m_aBalls[i].IsUsed())
			m_aBalls[i].Update( flFrametime );
	}
}

void IPaintball::Update( float flFrametime )
{
	//Apply gravity
	m_vecVelocity.z -= sv_gravity.GetFloat() * flFrametime;

	//TODO: Use vphysics instead.
	CGameTrace tr;
	UTIL_TraceLine(
		m_vecPosition,
		m_vecPosition + m_vecVelocity * flFrametime,
		MASK_SHOT,
		m_pOwner, COLLISION_GROUP_NONE, &tr );

	if (tr.DidHit())
	{
		PaintballTouch(tr.m_pEnt, &tr);
		CPaintballMgr::GetManager()->RemoveBall(m_iIndex);
	}
	else
		m_vecPosition = tr.endpos;
}