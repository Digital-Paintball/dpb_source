//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Player for HL1.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "sdk_player.h"
#include "sdk_gamerules.h"
#include "weapon_sdkbase.h"
#include "predicted_viewmodel.h"
#include "iservervehicle.h"
#include "viewport_panel_names.h"
#include "multiarena.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern int gEvilImpulse101;

#define SDK_PLAYER_MODEL "models/player/player.mdl"


// -------------------------------------------------------------------------------- //
// Player animation event. Sent to the client when a player fires, jumps, reloads, etc..
// -------------------------------------------------------------------------------- //

class CTEPlayerAnimEvent : public CBaseTempEntity
{
public:
	DECLARE_CLASS( CTEPlayerAnimEvent, CBaseTempEntity );
	DECLARE_SERVERCLASS();

					CTEPlayerAnimEvent( const char *name ) : CBaseTempEntity( name )
					{
					}

	CNetworkHandle( CBasePlayer, m_hPlayer );
	CNetworkVar( int, m_iEvent );
};

#define THROWGRENADE_COUNTER_BITS 3

IMPLEMENT_SERVERCLASS_ST_NOBASE( CTEPlayerAnimEvent, DT_TEPlayerAnimEvent )
	SendPropEHandle( SENDINFO( m_hPlayer ) ),
	SendPropInt( SENDINFO( m_iEvent ), Q_log2( PLAYERANIMEVENT_COUNT ) + 1, SPROP_UNSIGNED ),
END_SEND_TABLE()

static CTEPlayerAnimEvent g_TEPlayerAnimEvent( "PlayerAnimEvent" );

void TE_PlayerAnimEvent( CBasePlayer *pPlayer, PlayerAnimEvent_t event )
{
	CPVSFilter filter( (const Vector&)pPlayer->EyePosition() );
	
	g_TEPlayerAnimEvent.m_hPlayer = pPlayer;
	g_TEPlayerAnimEvent.m_iEvent = event;
	g_TEPlayerAnimEvent.Create( filter, 0 );
}

// -------------------------------------------------------------------------------- //
// Tables.
// -------------------------------------------------------------------------------- //

LINK_ENTITY_TO_CLASS( player, CSDKPlayer );
PRECACHE_REGISTER(player);

BEGIN_SEND_TABLE_NOBASE( CSDKPlayer, DT_SDKLocalPlayerExclusive )
	SendPropInt( SENDINFO( m_iShotsFired ), 8, SPROP_UNSIGNED ),
END_SEND_TABLE()

IMPLEMENT_SERVERCLASS_ST( CSDKPlayer, DT_SDKPlayer )
	SendPropExclude( "DT_BaseAnimating", "m_flPoseParameter" ),
	SendPropExclude( "DT_BaseAnimating", "m_flPlaybackRate" ),	
	SendPropExclude( "DT_BaseAnimating", "m_nSequence" ),
	SendPropExclude( "DT_BaseEntity", "m_angRotation" ),
	SendPropExclude( "DT_BaseAnimatingOverlay", "overlay_vars" ),
	
	// playeranimstate and clientside animation takes care of these on the client
	SendPropExclude( "DT_ServerAnimationData" , "m_flCycle" ),	
	SendPropExclude( "DT_AnimTimeMustBeFirst" , "m_flAnimTime" ),

	// Data that only gets sent to the local player.
	SendPropDataTable( "sdklocaldata", 0, &REFERENCE_SEND_TABLE(DT_SDKLocalPlayerExclusive), SendProxy_SendLocalDataTable ),

	SendPropAngle( SENDINFO_VECTORELEM(m_angEyeAngles, 0), 11 ),
	SendPropAngle( SENDINFO_VECTORELEM(m_angEyeAngles, 1), 11 ),
	SendPropAngle( SENDINFO_VECTORELEM(m_angEyeAngles, 2), 11 ),
	SendPropEHandle( SENDINFO( m_hRagdoll ) ),

	SendPropInt( SENDINFO( m_iThrowGrenadeCounter ), THROWGRENADE_COUNTER_BITS, SPROP_UNSIGNED ),
END_SEND_TABLE()

class CSDKRagdoll : public CBaseAnimatingOverlay
{
public:
	DECLARE_CLASS( CSDKRagdoll, CBaseAnimatingOverlay );
	DECLARE_SERVERCLASS();

	// Transmit ragdolls to everyone.
	virtual int UpdateTransmitState()
	{
		return SetTransmitState( FL_EDICT_ALWAYS );
	}

public:
	// In case the client has the player entity, we transmit the player index.
	// In case the client doesn't have it, we transmit the player's model index, origin, and angles
	// so they can create a ragdoll in the right place.
	CNetworkHandle( CBaseEntity, m_hPlayer );	// networked entity handle 
	CNetworkVector( m_vecRagdollVelocity );
	CNetworkVector( m_vecRagdollOrigin );
};

LINK_ENTITY_TO_CLASS( sdk_ragdoll, CSDKRagdoll );

IMPLEMENT_SERVERCLASS_ST_NOBASE( CSDKRagdoll, DT_SDKRagdoll )
	SendPropVector( SENDINFO(m_vecRagdollOrigin), -1,  SPROP_COORD ),
	SendPropEHandle( SENDINFO( m_hPlayer ) ),
	SendPropModelIndex( SENDINFO( m_nModelIndex ) ),
	SendPropInt		( SENDINFO(m_nForceBone), 8, 0 ),
	SendPropVector	( SENDINFO(m_vecForce), -1, SPROP_NOSCALE ),
	SendPropVector( SENDINFO( m_vecRagdollVelocity ) )
END_SEND_TABLE()


// -------------------------------------------------------------------------------- //

void cc_CreatePredictionError_f()
{
	CBaseEntity *pEnt = CBaseEntity::Instance( 1 );
	pEnt->SetAbsOrigin( pEnt->GetAbsOrigin() + Vector( 63, 0, 0 ) );
}

ConCommand cc_CreatePredictionError( "CreatePredictionError", cc_CreatePredictionError_f, "Create a prediction error", FCVAR_CHEAT );

void CC_Buy_f()
{
	CBasePlayer *pPlayer = ToBasePlayer( UTIL_GetCommandClient() ); 
	if ( !pPlayer )
		return;

	if (engine->Cmd_Argc() <= 2)
        pPlayer->ShowViewPortPanel( PANEL_BUY, true );
	else
		ToSDKPlayer(pPlayer)->OrderWeapon(engine->Cmd_Argv(1), atoi(engine->Cmd_Argv(2)));
}

ConCommand CC_Buy( "buy", CC_Buy_f, "Open the buy menu" );

CSDKPlayer::CSDKPlayer()
{
	m_PlayerAnimState = CreatePlayerAnimState( this, this, LEGANIM_9WAY, true );

	UseClientSideAnimation();
	m_angEyeAngles.Init();

	SetViewOffset( SDK_PLAYER_VIEW_OFFSET );

	m_iThrowGrenadeCounter = 0;
}


CSDKPlayer::~CSDKPlayer()
{
	m_PlayerAnimState->Release();
}


CSDKPlayer *CSDKPlayer::CreatePlayer( const char *className, edict_t *ed )
{
	CSDKPlayer::s_PlayerEdict = ed;
	return (CSDKPlayer*)CreateEntityByName( className );
}

void CSDKPlayer::LeaveVehicle( const Vector &vecExitPoint, const QAngle &vecExitAngles )
{
	BaseClass::LeaveVehicle( vecExitPoint, vecExitAngles );

	//teleport physics shadow too
	// Vector newPos = GetAbsOrigin();
	// QAngle newAng = GetAbsAngles();

	// Teleport( &newPos, &newAng, &vec3_origin );
}

void CSDKPlayer::PreThink(void)
{
	// Riding a vehicle?
	if ( IsInAVehicle() )	
	{
		// make sure we update the client, check for timed damage and update suit even if we are in a vehicle
		UpdateClientData();		
		CheckTimeBasedDamage();

		// Allow the suit to recharge when in the vehicle.
		CheckSuitUpdate();
		
		WaterMove();	
		return;
	}

	BaseClass::PreThink();
}


void CSDKPlayer::PostThink()
{
	BaseClass::PostThink();

	QAngle angles = GetLocalAngles();
	angles[PITCH] = 0;
	SetLocalAngles( angles );
	
	// Store the eye angles pitch so the client can compute its animation state correctly.
	m_angEyeAngles = EyeAngles();

    m_PlayerAnimState->Update( m_angEyeAngles[YAW], m_angEyeAngles[PITCH] );
}


void CSDKPlayer::Precache()
{
	PrecacheModel( SDK_PLAYER_MODEL );

	BaseClass::Precache();
}

void CSDKPlayer::Spawn()
{
	SetModel( SDK_PLAYER_MODEL );
	SetMoveType( MOVETYPE_WALK );
	RemoveSolidFlags( FSOLID_NOT_SOLID );

	m_hRagdoll = NULL;
	
	BaseClass::Spawn();
}

void CSDKPlayer::InitialSpawn( void )
{
	BaseClass::InitialSpawn();

	const ConVar *hostname = cvar->FindVar( "hostname" );
	const char *title = (hostname) ? hostname->GetString() : "MESSAGE OF THE DAY";

	// open info panel on client showing MOTD:
	KeyValues *data = new KeyValues("data");
	data->SetString( "title", title );		// info panel title
	data->SetString( "type", "1" );			// show userdata from stringtable entry
	data->SetString( "msg",	"motd" );		// use this stringtable entry

	ShowViewPortPanel( PANEL_INFO, true, data );

	data->deleteThis();
}

void CSDKPlayer::Event_Killed( const CTakeDamageInfo &info )
{
	// Note: since we're dead, it won't draw us on the client, but we don't set EF_NODRAW
	// because we still want to transmit to the clients in our PVS.

	BaseClass::Event_Killed( info );
}

void CSDKPlayer::CreateRagdollEntity()
{
	// If we already have a ragdoll, don't make another one.
	CSDKRagdoll *pRagdoll = dynamic_cast< CSDKRagdoll* >( m_hRagdoll.Get() );

	if ( !pRagdoll )
	{
		// create a new one
		pRagdoll = dynamic_cast< CSDKRagdoll* >( CreateEntityByName( "sdk_ragdoll" ) );
	}

	if ( pRagdoll )
	{
		pRagdoll->m_hPlayer = this;
		pRagdoll->m_vecRagdollOrigin = GetAbsOrigin();
		pRagdoll->m_vecRagdollVelocity = GetAbsVelocity();
		pRagdoll->m_nModelIndex = m_nModelIndex;
		pRagdoll->m_nForceBone = m_nForceBone;
		pRagdoll->m_vecForce = Vector(0,0,0);
	}

	// ragdolls will be removed on round restart automatically
	m_hRagdoll = pRagdoll;
}

void CSDKPlayer::DoAnimationEvent( PlayerAnimEvent_t event )
{
	if ( event == PLAYERANIMEVENT_THROW_GRENADE )
	{
		// Grenade throwing has to synchronize exactly with the player's grenade weapon going away,
		// and events get delayed a bit, so we let CCSPlayerAnimState pickup the change to this
		// variable.
		m_iThrowGrenadeCounter = (m_iThrowGrenadeCounter+1) % (1<<THROWGRENADE_COUNTER_BITS);
	}
	else
	{
		m_PlayerAnimState->DoAnimationEvent( event );
		TE_PlayerAnimEvent( this, event );	// Send to any clients who can see this guy.
	}
}

CWeaponSDKBase* CSDKPlayer::GetActiveSDKWeapon() const
{
	return dynamic_cast< CWeaponSDKBase* >( GetActiveWeapon() );
}

void CSDKPlayer::CreateViewModel( int index /*=0*/ )
{
	Assert( index >= 0 && index < MAX_VIEWMODELS );

	if ( GetViewModel( index ) )
		return;

	CPredictedViewModel *vm = ( CPredictedViewModel * )CreateEntityByName( "predicted_viewmodel" );
	if ( vm )
	{
		vm->SetAbsOrigin( GetAbsOrigin() );
		vm->SetOwner( this );
		vm->SetIndex( index );
		DispatchSpawn( vm );
		vm->FollowEntity( this, false );
		m_hViewModel.Set( index, vm );
	}
}

void CSDKPlayer::CheatImpulseCommands( int iImpulse )
{
	if ( iImpulse != 101 )
	{
		BaseClass::CheatImpulseCommands( iImpulse );
		return ;
	}
	gEvilImpulse101 = true;

	EquipSuit();

	GiveNamedItem( "weapon_blazer" );

	// Give the player everything!
	GiveAmmo( 300, AMMO_PAINT );
	GiveAmmo( 5, AMMO_GRENADE );
	
	if ( GetHealth() < 100 )
	{
		TakeHealth( 25, DMG_GENERIC );
	}

	gEvilImpulse101		= false;
}

void CSDKPlayer::ResetOrder()
{
	m_szDesiredWeapon[0] = '\0';
	m_iDesiredAttachments = 0;
}

void CSDKPlayer::OrderWeapon(const char* pszWeapon, int iAttachments)
{
	int iWeaponID = AliasToWeaponID(pszWeapon);
	if (iWeaponID == WEAPON_NONE)
	{
		ResetOrder();
		return;
	}

	// Insert some better rules about which weapons are allowed here.
	if (iWeaponID != WEAPON_BLAZER)
	{
		ResetOrder();
		return;
	}

	Q_snprintf(m_szDesiredWeapon, DESIRED_WPN_LENGTH, "weapon_%s", pszWeapon);
	m_iDesiredAttachments = iAttachments;
}

bool CSDKPlayer::ArenaSpawnOK()
{
	if (m_eDesiredTeam == TEAM_UNASSIGNED)
		return false;

	if (m_szDesiredWeapon[0] == '\0')
		return false;

	return true;
}

void CSDKPlayer::SetDesiredTeam( enum eteams_list eTeam, bool bAuto )
{
	if (!GetArena())
		return;

	// Unassigned means auto-assign.
	if (eTeam == TEAM_UNASSIGNED && bAuto)
	{
		// Pick a team!
		CTeam *pTeam = NULL;
		for (int j = 0; j < GetArena()->GetTeamNumber(); j++)
		{
			if (!pTeam)
			{
				pTeam = GetArena()->GetTeam(j);
				continue;
			}

			if (GetArena()->GetTeam(j)->m_aPlayers.Count() < pTeam->m_aPlayers.Count())
				pTeam = GetArena()->GetTeam(j);
		}

		if (!pTeam)
		{
			DevMsg("ERROR: Arena has no teams! Put some info_player_teamspawns in there.\n");
			Assert(0);
			return;
		}
		m_eDesiredTeam = (enum eteams_list) pTeam->GetTeamNumber();
	}
	else
		m_eDesiredTeam = eTeam;
}

void CSDKPlayer::DeployArmaments()
{
	GiveAmmo( 300, AMMO_PAINT );
	if (GetActiveWeapon())
	{
		GetActiveWeapon()->Deploy();
	}
	else
	{
		GiveNamedItem( GetOrderedWeapon() );
	}
}

enum eteams_list CSDKPlayer::GetDesiredTeam( void )
{
	return m_eDesiredTeam;
}

const char*	CSDKPlayer::GetOrderedWeapon( void )
{
	return m_szDesiredWeapon;
}

void CSDKPlayer::FlashlightTurnOn( void )
{
	AddEffects( EF_DIMLIGHT );
}

void CSDKPlayer::FlashlightTurnOff( void )
{
	RemoveEffects( EF_DIMLIGHT );
}

int CSDKPlayer::FlashlightIsOn( void )
{
	return IsEffectActive( EF_DIMLIGHT );
}
