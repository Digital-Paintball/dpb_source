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
	m_iLastBall = -1;
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

	float flSpread = pOwner->GetPredictedSpread();
	const float flSpinFactor = 120;	// A popular show on Fox News.

	RandomSeed( pOwner->GetPredictionRandomSeed() );

	float flRadius = RandomFloat( 0.0, 1.0 );
	float flTheta = RandomFloat( 0.0, 2*M_PI );

	float flX = flRadius*cos(flTheta);
	float flY = flRadius*sin(flTheta);

	float flSpinTheta = RandomFloat( 0.0, 2*M_PI );

	float flVelocity = PAINTBALL_AIR_VELOCITY + RandomFloat(-120, 120);

	Vector vecForward, vecUp, vecRight;
	AngleVectors( pOwner->EyeAngles() + pOwner->m_Local.m_vecPunchAngle, &vecForward, &vecRight, &vecUp );

	CPaintball *pPB = &m_aBalls[i];

	Assert(pPB->m_bAvailable);
	pPB->m_bAvailable = false;
	pPB->m_pOwner = pOwner;
	pPB->m_vecPosition = pOwner->Weapon_ShootPosition();
	pPB->m_vecVelocity = vecForward * flVelocity + flSpread * flX * vecRight + flSpread * flY * vecUp;
	pPB->m_flXSpin = cos(flSpinTheta) * flSpinFactor;
	pPB->m_flYSpin = sin(flSpinTheta) * flSpinFactor;

	m_iBalls++;
	if (i > m_iLastBall)
		m_iLastBall = i;
	if (i == m_iFirstAvailable)
		m_iFirstAvailable = FindEmptySpace();

	SanityCheck();

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
		bool bLastBallFound = false;
		for (int j = m_iLastBall-1; j >= 0; j--)
		{
			if (m_aBalls[j].IsUsed())
			{
				m_iLastBall = j;
				bLastBallFound = true;
				break;
			}
		}
		if (!bLastBallFound)
			m_iLastBall = -1;
	}
	if (i < m_iFirstAvailable)
		m_iFirstAvailable = i;

	SanityCheck();

	pPB->Destroy();
}

void CPaintballMgr::SanityCheck()
{
#ifdef _DEBUG
	int iBalls = 0;
	int iFirstAvailable = 0;
	int iLastBall = -1;

	//Do some sanity checking.
	for (int i = 0; i < MAX_PAINTBALLS; i++)
	{
		// If we're before the first available ball, this ball had better be used.
		Assert(i >= m_iFirstAvailable || m_aBalls[i].IsUsed());

		// If we're beyond the last ball, this ball had better be unused.
		Assert((m_iLastBall != -1 && i <= m_iLastBall) || m_aBalls[i].IsAvailable());

		if (m_aBalls[i].IsUsed() && iFirstAvailable == i)
			iFirstAvailable++;

		if (m_aBalls[i].IsUsed() && i > iLastBall)
			iLastBall = i;

		if (m_aBalls[i].IsUsed())
			iBalls++;
	}

	Assert(iBalls == m_iBalls);
	Assert(iLastBall == iLastBall);
	Assert(iFirstAvailable == m_iFirstAvailable);
#endif
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
	Vector vecVelNormal = m_vecVelocity;
	VectorNormalize(vecVelNormal);

	float flVelocity = m_vecVelocity.Length();

	Vector vecForward, vecUp, vecRight;
	QAngle angForward;
	VectorAngles( vecVelNormal, angForward );
	AngleVectors( angForward, &vecForward, &vecRight, &vecUp );

	Vector vecGravity = Vector(0, 0, -1) * sv_gravity.GetFloat();
	Vector vecSpin = vecRight * m_flXSpin + vecUp * m_flYSpin;
	Vector vecDrag = flVelocity * flVelocity * PAINTBALL_DRAG_COEFF * -vecVelNormal;

	// Apply all velocity changes at once.
	m_vecVelocity += (vecGravity + vecSpin + vecDrag) * flFrametime;

	// New velocity
	flVelocity = m_vecVelocity.Length();

	// Degrade spin
	m_flXSpin *= 1 - (0.1 * flFrametime);
	m_flYSpin *= 1 - (0.1 * flFrametime);

	//TODO: Use vphysics instead.
	CGameTrace tr;
	UTIL_TraceLine(
		m_vecPosition,
		m_vecPosition + m_vecVelocity * flFrametime,
		MASK_SHOT,
		m_pOwner, COLLISION_GROUP_NONE, &tr );

	if (tr.DidHit())
	{
		bool bBroke = false;
		if (m_pOwner && flVelocity < 2640 && flVelocity > 1680)	//Between 220 and 140 fps
		{
			RandomSeed( m_pOwner->GetPredictionRandomSeed() );
			bBroke = RandomFloat( 0.0, 1.0 ) > (2640 - flVelocity)/(2640 - 1680);
		}
		else if (flVelocity >= 2640)
			bBroke = true;

		if (bBroke)
			PaintballTouch(tr.m_pEnt, &tr);
		
		//Todo: bounce the ball if it did not break.
		CPaintballMgr::GetManager()->RemoveBall(m_iIndex);
		return;
	}

	m_vecPosition = tr.endpos;
}