#ifndef PAINTBALL_SHARED_H
#define PAINTBALL_SHARED_H

#define PAINTBALL_MODEL				"sprites/pb_blur.vmt"
#define PAINTBALL_AIR_VELOCITY		3500
#define PAINTBALL_DRAG_COEFF		(0.0002f)
#define PAINTBALL_DIAMETER			(0.68f)

#ifdef CLIENT_DLL
#define CArena C_Arena
#endif

class CArena;

class IPaintball
{
	friend class CPaintball;

public:
	virtual void Init( int iIndex )
	{
		m_bAvailable = true;
		m_iIndex = iIndex;
	};

	virtual void	PaintballTouch( CBaseEntity *pOther, CGameTrace* pTrace ) = 0;

	virtual void	Spawn() = 0;
	virtual void	Destroy() = 0;

	void		Update( float flFrametime );

	inline bool IsAvailable() { return m_bAvailable; };
	inline bool IsUsed() { return !m_bAvailable; };

	bool			m_bAvailable;
	int				m_iIndex;
	CBasePlayer*	m_pOwner;
	CArena*			m_pArena;

	Vector			m_vecPosition;
	Vector			m_vecVelocity;
	float			m_flXSpin;
	float			m_flYSpin;
};

#endif // PAINTBALL_SHARED_H