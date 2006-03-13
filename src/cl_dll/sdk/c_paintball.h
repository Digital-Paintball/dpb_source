#ifndef C_PAINTBALL_H
#define C_PAINTBALL_H

#include "cbase.h"
#include "paintball_shared.h"

#define CPaintball C_Paintball

class C_Paintball : public IClientRenderable, public IPaintball
{
public:
	
	static void		Init();
	void			Init(int i);

	virtual void	Spawn();
	virtual void	Destroy();

	void	PaintballTouch( C_BaseEntity *pOther, CGameTrace* pTrace );

// IClientRenderable overrides.
public:

	IClientUnknown*				GetIClientUnknown() { return NULL; };
	virtual int					GetBody() { return 0; }
	virtual const Vector&		GetRenderOrigin( );
	virtual const QAngle&		GetRenderAngles( );
	virtual bool				ShouldDraw() { return IsUsed(); };
	virtual bool				IsTransparent( void ) { return false; };
	virtual const model_t*		GetModel( ) const;
	virtual int					DrawModel( int flags );
	virtual void				ComputeFxBlend( ) {};
	virtual int					GetFxBlend( );
	virtual void				GetColorModulation( float* color );
	virtual bool				LODTest() { return true; };
	virtual bool				SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	virtual void				SetupWeights( void ) {};
	virtual void				DoAnimationEvents( void ) {};
	virtual void				GetRenderBounds( Vector& mins, Vector& maxs );
	virtual IPVSNotify*			GetPVSNotifyInterface() { return NULL; };
	virtual void				GetRenderBoundsWorldspace( Vector& mins, Vector& maxs );
	virtual bool				ShouldReceiveProjectedTextures( int flags ) { return false; };
	virtual bool				GetShadowCastDistance( float *pDist, ShadowType_t shadowType ) const			{ return false; }
	virtual bool				GetShadowCastDirection( Vector *pDirection, ShadowType_t shadowType ) const	{ return false; }
	virtual bool				UsesFrameBufferTexture() { return false; };

	virtual ClientShadowHandle_t	GetShadowHandle() const;
	virtual ClientRenderHandle_t&	RenderHandle();
	virtual void				GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType );

protected:
	static const model_t*			s_pModel;
	ClientRenderHandle_t			m_hRenderHandle;
	float							m_flBrightness;
};

#endif // C_PAINTBALL_H