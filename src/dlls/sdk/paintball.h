#ifndef PAINTBALL_H
#define PAINTBALL_H

#include "paintball_shared.h"

// CPaintball class
class CPaintball : public IPaintball
{
public:
	void	Init(int i);

	virtual void	Spawn( void );
	virtual void	Destroy( void );

	void	PaintballTouch( CBaseEntity *pOther, CGameTrace* pTrace );
};

#endif // PAINTBALL_H
