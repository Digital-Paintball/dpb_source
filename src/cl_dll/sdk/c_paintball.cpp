//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "c_paintball.h"
#include "model_types.h"
#include "c_sprite.h"
#include "enginesprite.h"
#include "view.h"

// Should be last include
#include "tier0/memdbgon.h"

const model_t* C_Paintball::s_pModel;

void C_Paintball::Init( )
{
	s_pModel = modelinfo->GetModel( modelinfo->GetModelIndex( PAINTBALL_MODEL ) );
}

void C_Paintball::Init( int i )
{
	IPaintball::Init(i);
	m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;
}

void C_Paintball::Spawn( void )
{
	ClientLeafSystem()->AddRenderable( this, RENDER_GROUP_TRANSLUCENT_ENTITY );
	m_flBrightness = 0;
}

void C_Paintball::Destroy( void )
{
	ClientLeafSystem()->RemoveRenderable( m_hRenderHandle );
}

void C_Paintball::PaintballTouch( C_BaseEntity *pOther, CGameTrace* pTrace )
{
}

//-----------------------------------------------------------------------------
// Data accessors
//-----------------------------------------------------------------------------
const Vector& C_Paintball::GetRenderOrigin( void )
{
	return m_vecPosition;
}

const QAngle& C_Paintball::GetRenderAngles( void )
{
	return vec3_angle;
}

void C_Paintball::GetRenderBounds( Vector& mins, Vector& maxs )
{
	float flSpriteLength = m_vecVelocity.Length() * gpGlobals->frametime;

	Vector vecVelNormal = m_vecVelocity;
	VectorNormalize(vecVelNormal);

	Vector vecEffectEnd = vecVelNormal * flSpriteLength;

	mins.Init(0, 0, 0);
	maxs.Init(0, 0, 0);

	if (vecEffectEnd.x > 0)
		maxs.x = vecEffectEnd.x;
	else
		mins.x = vecEffectEnd.x;

	if (vecEffectEnd.y > 0)
		maxs.y = vecEffectEnd.y;
	else
		mins.y = vecEffectEnd.y;

	if (vecEffectEnd.z > 0)
		maxs.z = vecEffectEnd.z;
	else
		mins.z = vecEffectEnd.z;
}

void C_Paintball::GetRenderBoundsWorldspace( Vector& mins, Vector& maxs )
{
	DefaultRenderBoundsWorldspace( this, mins, maxs );
}

void C_Paintball::GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType )
{
	GetRenderBounds( mins, maxs );
}

ClientShadowHandle_t C_Paintball::GetShadowHandle() const
{
	return CLIENTSHADOW_INVALID_HANDLE;
}

ClientRenderHandle_t& C_Paintball::RenderHandle()
{
	return m_hRenderHandle;
}	


//-----------------------------------------------------------------------------
// Render setup
//-----------------------------------------------------------------------------
bool C_Paintball::SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
{
	if (!s_pModel)
		return false;

	// Setup our transform.
	matrix3x4_t parentTransform;
	const QAngle &vRenderAngles = GetRenderAngles();
	const Vector &vRenderOrigin = GetRenderOrigin();
	AngleMatrix( vRenderAngles, parentTransform );
	parentTransform[0][3] = vRenderOrigin.x;
	parentTransform[1][3] = vRenderOrigin.y;
	parentTransform[2][3] = vRenderOrigin.z;

	// Just copy it on down baby
	studiohdr_t *pStudioHdr = modelinfo->GetStudiomodel( s_pModel );
	for (int i = 0; i < pStudioHdr->numbones; i++) 
	{
		MatrixCopy( parentTransform, pBoneToWorldOut[i] );
	}

	return true;
}

//-----------------------------------------------------------------------------
// Render baby!
//-----------------------------------------------------------------------------
const model_t* C_Paintball::GetModel( ) const
{
	return s_pModel;
}

ConVar r_visualizeballphysics( "r_visualizeballphysics", "0", FCVAR_CHEAT );

int C_Paintball::DrawModel( int flags )
{
	if ( !s_pModel || modelinfo->GetModelType( s_pModel ) != mod_sprite )
		return 0;

	CEngineSprite *pSprite;

	// Get extra data
	pSprite = (CEngineSprite *)modelinfo->GetModelExtraData( s_pModel );
	if ( !pSprite )
		return 0;

	// Fade the paintball in so it's not so obvious it comes from VEC_VIEW.
	float flBackBrightness = 1;
	float flFrontBrightness = 1;
	if (m_flBrightness < 1)
	{
		flBackBrightness = m_flBrightness;
		m_flBrightness += 2 * gpGlobals->frametime;
		flFrontBrightness = m_flBrightness;
	}

	float flSpriteLength = m_vecVelocity.Length() * gpGlobals->frametime;

	Vector vecVelNormal = m_vecVelocity;
	VectorNormalize(vecVelNormal);

	Vector vecEffectOrigin = GetRenderOrigin();

	CGameTrace tr;
	UTIL_TraceLine(
		vecEffectOrigin,
		vecEffectOrigin + vecVelNormal * flSpriteLength,
		MASK_SHOT, m_pOwner, COLLISION_GROUP_NONE, &tr );

	// Don't tunnel.
	if (tr.fraction != 1)
		flSpriteLength *= tr.fraction;

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecEffectOrigin + vecVelNormal * flSpriteLength, 255, 0, 0, true, -1.0f );

	Vector vecVelForward, vecVelRight, vecVelUp;
	QAngle angVelForward;
	VectorAngles( vecVelNormal, angVelForward );
	AngleVectors( angVelForward, &vecVelForward, &vecVelRight, &vecVelUp );

	Vector vecRight, vecUp;

	Vector vecViewToSprite = vecEffectOrigin - CurrentViewOrigin();
	VectorNormalize(vecViewToSprite);

	// If the player is looking at the paintball head-on (or tail-on)
	// the cross product is undefined, so use the player's right vector
	// instead.
	float flDot = DotProduct(vecVelNormal, vecViewToSprite);
	if (flDot > 0.999848 || flDot < -0.999848)	// cos(1 degree) = 0.999848
		vecRight = CurrentViewRight();
	else
		vecRight = CrossProduct(vecVelNormal, vecViewToSprite);
	VectorNormalize(vecRight);

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecEffectOrigin + vecRight * 10, 0, 255, 0, true, -1.0f );

	Vector vecBallForward = CrossProduct(vecRight, vecVelNormal);
	VectorNormalize(vecBallForward);

	Vector vecTop, vecBottom;
	if (m_vecVelocity.z >= 0)
	{
		vecTop = GetRenderOrigin() + vecVelNormal * flSpriteLength + vecBallForward * (PAINTBALL_DIAMETER/2);
		vecBottom = GetRenderOrigin() - vecBallForward * (PAINTBALL_DIAMETER/2);
		vecUp = vecBottom - vecTop;
	}
	else
	{
		vecTop = GetRenderOrigin() + vecBallForward * (PAINTBALL_DIAMETER/2);
		vecBottom = GetRenderOrigin() + vecVelNormal * flSpriteLength - vecBallForward * (PAINTBALL_DIAMETER/2);
		vecUp = vecTop - vecBottom;
	}
	VectorNormalize(vecUp);

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecEffectOrigin + vecUp * 10, 0, 255, 0, true, -1.0f );

	if ( render->GetBlend() <= 0.0f )
		return 0;

	const float flScale = 1.0;

	IMaterial	*pMaterial = pSprite->GetMaterial();
	if( !pMaterial )
		return 0;

	materials->Bind( pMaterial, this );

	Vector vecPoint;
	IMesh* pMesh = materials->GetDynamicMesh( );

	CMeshBuilder meshBuilder;
	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

	meshBuilder.Color4ub (255, 255, 255, 255 * flBackBrightness);
	meshBuilder.TexCoord2f (0, 0, 1);
	VectorMA (vecEffectOrigin, PAINTBALL_DIAMETER/2, vecRight, vecPoint);
	meshBuilder.Position3fv (vecPoint.Base());
	meshBuilder.AdvanceVertex();

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecPoint, 256, 256, 256, true, -1.0f );

	meshBuilder.Color4ub (255, 255, 255, 255 * flFrontBrightness);
	meshBuilder.TexCoord2f (0, 1, 1);
	VectorMA (vecEffectOrigin, -flSpriteLength, vecUp, vecPoint);
	VectorMA (vecPoint, PAINTBALL_DIAMETER/2, vecRight, vecPoint);
	meshBuilder.Position3fv (vecPoint.Base());
	meshBuilder.AdvanceVertex();

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecPoint, 256, 0, 0, true, -1.0f );

	meshBuilder.Color4ub (255, 255, 255, 255 * flFrontBrightness);
	meshBuilder.TexCoord2f (0, 1, 0);
	VectorMA (vecEffectOrigin, -flSpriteLength, vecUp, vecPoint);
	VectorMA (vecPoint, -PAINTBALL_DIAMETER/2, vecRight, vecPoint);
	meshBuilder.Position3fv (vecPoint.Base());
	meshBuilder.AdvanceVertex();

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecPoint, 0, 256, 0, true, -1.0f );

	meshBuilder.Color4ub (255, 255, 255, 255 * flBackBrightness);
	meshBuilder.TexCoord2f (0, 0, 0);
	VectorMA (vecEffectOrigin, -PAINTBALL_DIAMETER/2, vecRight, vecPoint);
	meshBuilder.Position3fv (vecPoint.Base());
	meshBuilder.AdvanceVertex();

	if ( r_visualizeballphysics.GetBool() )
		DebugDrawLine( vecEffectOrigin, vecPoint, 0, 0, 256, true, -1.0f );

	meshBuilder.End();
	pMesh->Draw();

	return 1;
}


int C_Paintball::GetFxBlend( )
{
	return 255;
}

void C_Paintball::GetColorModulation( float* color )
{
	color[0] = color[1] = color[2] = 1.0f;
}
