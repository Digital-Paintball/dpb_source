//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "c_paintball.h"
#include "model_types.h"

// Should be last include
#include "tier0/memdbgon.h"

void C_Paintball::Init( int i )
{
	IPaintball::Init(i);
	m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;
	m_pModel = engine->LoadModel( PAINTBALL_MODEL );
}

void C_Paintball::Spawn( void )
{
	ClientLeafSystem()->AddRenderable( this, RENDER_GROUP_OPAQUE_ENTITY );
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
	int nModelType = modelinfo->GetModelType( m_pModel );
	if (nModelType == mod_studio || nModelType == mod_brush)
	{
		modelinfo->GetModelRenderBounds( GetModel(), mins, maxs );
	}
	else
	{
		mins.Init( 0,0,0 );
		maxs.Init( 0,0,0 );
	}
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
	if (!m_pModel)
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
	studiohdr_t *pStudioHdr = modelinfo->GetStudiomodel( m_pModel );
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
	return m_pModel;
}

int C_Paintball::DrawModel( int flags )
{
	if (!m_pModel)
		return 0;

	int drawn = modelrender->DrawModel( 
		flags,
		this,
		m_ModelInstance,
		-1,		// no entity index
		m_pModel,
		m_vecPosition,
		vec3_angle,
		0,	// skin
		0,	// body
		0  // hitboxset
		);
	return drawn;
}


int C_Paintball::GetFxBlend( )
{
	return 255;
}

void C_Paintball::GetColorModulation( float* color )
{
	color[0] = color[1] = color[2] = 1.0f;
}
