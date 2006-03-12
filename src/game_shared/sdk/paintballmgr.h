 //========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef PAINTBALLMGR_H
#define PAINTBALLMGR_H

#ifdef CLIENT_DLL
#include "c_paintball.h"
#include "c_multiarena.h"
#include "c_recipientfilter.h"
#else
#include "paintball.h"
#include "multiarena.h"
#include "recipientfilter.h"
#endif

#define MAX_PAINTBALLS 256

#ifdef GAME_DLL
#define THINK_PROTOTYPE FrameUpdatePostEntityThink()
#else
#define THINK_PROTOTYPE Update( float flFrametime )
#endif

// CPaintballMgr class
class CPaintballMgr : public CAutoGameSystem
{
public:
	static CPaintballMgr *GetManager() { return &s_PaintballMgr; }

	CPaintballMgr() { m_bInitialized = false; };

	// IGameSystem functions
	virtual void LevelInitPreEntity();
	virtual void THINK_PROTOTYPE;

	void	AddBall( CBasePlayer* pOwner );
	void	RemoveBall( int i );
	void	RemoveBalls( CArena* pArena );
	int		FindEmptySpace( );

	void	SanityCheck();

	bool		m_bInitialized;

	CPaintball	m_aBalls[MAX_PAINTBALLS];	//Paintball players love to shoot their balls.
	int			m_iBalls;
	int			m_iLastBall;
	int			m_iFirstAvailable;

private:
	static CPaintballMgr s_PaintballMgr;
};

#endif // PAINTBALLMGR_H