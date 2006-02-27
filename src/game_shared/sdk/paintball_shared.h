#ifndef PAINTBALL_SHARED_H
#define PAINTBALL_SHARED_H

#define PAINTBALL_MODEL				"models/paintball/paintball.mdl"
#define PAINTBALL_AIR_VELOCITY		3500

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

	void		Update( float flFrametime );

	inline bool IsAvailable() { return m_bAvailable; };
	inline bool IsUsed() { return !m_bAvailable; };


	bool			m_bAvailable;
	int				m_iIndex;
	Vector			m_vecPosition;
	Vector			m_vecVelocity;
	CBasePlayer*	m_pOwner;
};

#endif // PAINTBALL_SHARED_H