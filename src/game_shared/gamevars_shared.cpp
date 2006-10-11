//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "gamevars_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// some shared cvars used by game rules
ConVar mp_forcecamera( 
	"mp_forcecamera", 
	"0", 
	FCVAR_REPLICATED,
	"Restricts spectator modes for dead players" );
	
ConVar mp_allowspectators(
	"mp_allowspectators", 
	"1.0", 
	FCVAR_REPLICATED,
	"toggles whether the server allows spectator mode or not" );

ConVar friendlyfire(
	"mp_friendlyfire",
	"0",
	FCVAR_REPLICATED | FCVAR_NOTIFY );

ConVar mp_balanceteams( // jeff - unimplemented
	"mp_balanceteams",
	"0",
	FCVAR_REPLICATED | FCVAR_NOTIFY,
	"Bitmask for arenas that will have auto-team-balance enabled");

ConVar mp_balanceteams_threshold( // jeff - unimplemented
	"mp_balanceteams_threshold",
	"2",
	FCVAR_REPLICATED | FCVAR_NOTIFY,
	"How much of an advantage makes a team unbalanced");
