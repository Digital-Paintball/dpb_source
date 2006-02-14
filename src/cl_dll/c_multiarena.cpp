//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#include "c_multiarena.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT( C_Arena, DT_Arena, CArena )
	RecvPropInt( RECVINFO( m_State ) ),
	RecvPropInt( RECVINFO( m_iID ) ),
END_RECV_TABLE()

CUtlVector<CHandle<C_Arena> > C_Arena::s_hArenas;

C_Arena::C_Arena( )
{
	BaseClass();
	s_hArenas.AddToTail( this );
}

C_Arena::~C_Arena( )
{
	s_hArenas.FindAndRemove( this );
}

void C_Arena::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
	switch ( updateType )
	{
	case DATA_UPDATE_CREATED:
		break;
	case DATA_UPDATE_DATATABLE_CHANGED:
		break;
	}
}

int C_Arena::GetArenaNumber( )
{
	return s_hArenas.Count();
}

C_Arena* C_Arena::GetArena(int i)
{
	if (i < 0 || i >= s_hArenas.Count())
		return NULL;

	return s_hArenas[i];
}