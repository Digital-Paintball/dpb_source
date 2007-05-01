//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Entities dealing with the multi-arena functions.
//
// $NoKeywords: $
//=============================================================================//

#ifndef MULTIARENA_SHARED_H
#define MULTIARENA_SHARED_H
#ifdef _WIN32
#pragma once
#endif

class CArenaShared
{
public:
	typedef enum {
		GS_WAITING,		//Waiting for players
		GS_COUNTDOWN,	//Counting down for the round to begin
		GS_INPROGRESS,	//People are killing each other right now
		GS_VICTORY,		//One team has achieved victory
		// If you add to here, also add the number of bits in SendPropInt
	} gamestate_t;

	typedef enum {
		AE_JOIN,	//Player has joined the arena
		AE_QUIT,	//Player has quit the arena
		AE_RESET,	//Round is being reset
		AE_VICTORY,	//Specified team won the round
		AE_TIMEUPDATE, // I have a message for you - it has round time in it. -Jeff
		AE_TVICTORY, // This team won - Jeff
	} arenaevent_t;
};

#endif // MULTIARENA_SHARED_H