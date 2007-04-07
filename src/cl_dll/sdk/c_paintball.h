#ifndef C_PAINTBALL_H
#define C_PAINTBALL_H

#include "cbase.h"
#include "paintball_shared.h"

#define CPaintball C_Paintball

class C_Paintball : public IClientUnknown, public IClientRenderable, public IPaintball
{
public:
	
	static void		Init();
	void			Init(int i);

	virtual void	Spawn();
	virtual void	Destroy();

	void	PaintballTouch( C_BaseEntity *pOther, CGameTrace* pTrace );

// IClientRenderable overrides.
public:

	virtual int					GetBody() { return 0; }
	virtual const Vector&		GetRenderOrigin( );
	virtual const QAngle&		GetRenderAngles( );
	virtual bool				ShouldDraw() { return true; };
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
	
	//Tony; missing virtuals from newer sdk changes.
	virtual bool IsShadowDirty( )			     { return false; }
	virtual void MarkShadowDirty( bool bDirty )  {}
	virtual IClientRenderable *GetShadowParent() { return NULL; }
	virtual IClientRenderable *FirstShadowChild(){ return NULL; }
	virtual IClientRenderable *NextShadowPeer()  { return NULL; }
	virtual ShadowType_t		ShadowCastType() { return SHADOWS_NONE; }
	virtual void CreateModelInstance()			 {}
	virtual ModelInstanceHandle_t GetModelInstance() { return MODEL_INSTANCE_INVALID; }
	virtual int					LookupAttachment( const char *pAttachmentName ) { return -1; }
	virtual bool				GetAttachment( int number, matrix3x4_t &matrix );
	virtual	bool				GetAttachment( int number, Vector &origin, QAngle &angles );
	virtual float *				GetRenderClipPlane( void ) { return NULL; }
	virtual int					GetSkin( void ) { return 0; }
	virtual const matrix3x4_t &	RenderableToWorldTransform();
	// IHandleEntity stubs.
public:
	virtual void SetRefEHandle( const CBaseHandle &handle )	{ Assert( false ); }
	virtual const CBaseHandle& GetRefEHandle() const		{ Assert( false ); return *((CBaseHandle*)0); }

	//---------------------------------
	struct LightStyleInfo_t
	{
		unsigned int	m_LightStyle:24;
		unsigned int	m_LightStyleCount:8;
	};
	// IClientUnknown overrides.
public:

	virtual IClientUnknown*		GetIClientUnknown()		{ return this; }
	virtual ICollideable*		GetCollideable()		{ return 0; }		// Static props DO implement this.
	virtual IClientNetworkable*	GetClientNetworkable()	{ return 0; }
	virtual IClientRenderable*	GetClientRenderable()	{ return this; }
	virtual IClientEntity*		GetIClientEntity()		{ return 0; }
	virtual C_BaseEntity*		GetBaseEntity()			{ return 0; }
	virtual IClientThinkable*	GetClientThinkable()	{ return 0; }

protected:
	static const model_t*			s_pModel;
	ClientRenderHandle_t			m_hRenderHandle;
	float							m_flBrightness;
};

#endif // C_PAINTBALL_H