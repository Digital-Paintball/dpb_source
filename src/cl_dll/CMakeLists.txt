SET( CLIENT_SRCS
	alphamaterialproxy.cpp
	animatedentitytextureproxy.cpp
	animatedoffsettextureproxy.cpp
	animatedtextureproxy.cpp
	baseanimatedtextureproxy.cpp
	beamdraw.cpp
	bone_merge_cache.cpp
	c_ai_basehumanoid.cpp
	c_ai_basenpc.cpp
	c_baseanimating.cpp
	c_baseanimatingoverlay.cpp
	c_basecombatcharacter.cpp
	c_basecombatweapon.cpp
	c_basedoor.cpp
	c_baseentity.cpp
	c_baseflex.cpp
	c_baseplayer.cpp
	c_basetempentity.cpp
	c_baseviewmodel.cpp
	c_breakableprop.cpp
	c_dynamiclight.cpp
	c_effects.cpp
	c_entitydissolve.cpp
	c_entityparticletrail.cpp
	c_env_particlescript.cpp
	c_env_screenoverlay.cpp
	c_fire_smoke.cpp
	c_func_areaportalwindow.cpp
	c_func_breakablesurf.cpp
	c_func_conveyor.cpp
	c_func_dust.cpp
	c_func_lod.cpp
	c_func_occluder.cpp
	c_func_smokevolume.cpp
	c_func_tracktrain.cpp
	c_gib.cpp
	c_hairball.cpp
	c_impact_effects.cpp
	c_lightglow.cpp
	c_materialmodifycontrol.cpp
	c_movie_explosion.cpp
	c_particle_fire.cpp
	c_particle_smokegrenade.cpp
	c_physbox.cpp
	c_physicsprop.cpp
	c_physmagnet.cpp
	c_pixel_visibility.cpp
	c_plasma.cpp
	c_playerresource.cpp
	c_point_camera.cpp
	c_prop_vehicle.cpp
	c_props.cpp
	c_ragdoll_manager.cpp
	c_recipientfilter.cpp
	c_rope.cpp
	c_shadowcontrol.cpp
	c_smoke_trail.cpp
	c_smokestack.cpp
	c_soundscape.cpp
	c_spotlight_end.cpp
	c_sprite.cpp
	c_steamjet.cpp
	c_stickybolt.cpp
	c_sun.cpp
	c_te.cpp
	c_te_armorricochet.cpp
	c_te_basebeam.cpp
	c_te_beamentpoint.cpp
	c_te_beaments.cpp
	c_te_beamfollow.cpp
	c_te_beamlaser.cpp
	c_te_beampoints.cpp
	c_te_beamring.cpp
	c_te_beamringpoint.cpp
	c_te_beamspline.cpp
	c_te_bloodsprite.cpp
	c_te_bloodstream.cpp
	c_te_breakmodel.cpp
	c_te_bspdecal.cpp
	c_te_bubbles.cpp
	c_te_bubbletrail.cpp
	c_te_decal.cpp
	c_te_dynamiclight.cpp
	c_te_effect_dispatch.cpp
	c_te_energysplash.cpp
	c_te_explosion.cpp
	c_te_fizz.cpp
	c_te_footprint.cpp
	c_te_glassshatter.cpp
	c_te_glowsprite.cpp
	c_te_impact.cpp
	c_te_killplayerattachments.cpp
	c_te_largefunnel.cpp
	c_te_legacytempents.cpp
	c_te_muzzleflash.cpp
	c_te_particlesystem.cpp
	c_te_physicsprop.cpp
	c_te_playerdecal.cpp
	c_te_projecteddecal.cpp
	c_te_showline.cpp
	c_te_smoke.cpp
	c_te_sparks.cpp
	c_te_sprite.cpp
	c_te_spritespray.cpp
	c_te_worlddecal.cpp
	c_team.cpp
	c_tesla.cpp
	c_test_proxytoggle.cpp
	c_testtraceline.cpp
	c_tracer.cpp
	c_user_message_register.cpp
	c_vehicle_jeep.cpp
	c_vguiscreen.cpp
	c_world.cpp
	camomaterialproxy.cpp
	cdll_client_int.cpp
	cdll_util.cpp
	cl_mat_stub.cpp
	classmap.cpp
	client_thinklist.cpp
	clienteffectprecachesystem.cpp
	cliententitylist.cpp
	clientleafsystem.cpp
	clientmode_shared.cpp
	clientshadowmgr.cpp
	clientsideeffects.cpp
	clientsideeffects_test.cpp
	detailobjectsystem.cpp
	dummyproxy.cpp
	effectsclient.cpp
	flashlighteffect.cpp
	functionproxy.cpp
	fx.cpp
	fx_blood.cpp
	fx_cube.cpp
	fx_discreetline.cpp
	fx_envelope.cpp
	fx_explosion.cpp
	fx_fleck.cpp
	fx_impact.cpp
	fx_line.cpp
	fx_quad.cpp
	fx_shelleject.cpp
	fx_sparks.cpp
	fx_staticline.cpp
	fx_tracer.cpp
	fx_trail.cpp
	fx_water.cpp
	gametrace_client.cpp
	geiger.cpp
	glow_overlay.cpp
	history_resource.cpp
	hltvcamera.cpp
	hud.cpp
	hud_animationinfo.cpp
	hud_basechat.cpp
	hud_basetimer.cpp
	hud_bitmapnumericdisplay.cpp
	hud_closecaption.cpp
	hud_crosshair.cpp
	hud_element_helper.cpp
	hud_hintdisplay.cpp
	hud_msg.cpp
	hud_numericdisplay.cpp
	hud_pdump.cpp
	hud_redraw.cpp
	hud_vehicle.cpp
	hud_weapon.cpp
	in_camera.cpp
	in_joystick.cpp
	in_main.cpp
	in_mouse.cpp
	initializer.cpp
	interpolatedvar.cpp
	lampbeamproxy.cpp
	lamphaloproxy.cpp
	mathproxy.cpp
	matrixproxy.cpp
	menu.cpp
	message.cpp
	movehelper_client.cpp
	panelmetaclassmgr.cpp
	particle_collision.cpp
	particle_litsmokeemitter.cpp
	particle_proxies.cpp
	particle_simple3d.cpp
	particlemgr.cpp
	particles_attractor.cpp
	particles_ez.cpp
	particles_localspace.cpp
	particles_simple.cpp
	particlesphererenderer.cpp
	perfvisualbenchmark.cpp
	physics.cpp
	physics_main_client.cpp
	physpropclientside.cpp
	playerandobjectenumerator.cpp
	prediction.cpp
	proxyentity.cpp
	proxyhealth.cpp
	proxyplayer.cpp
	proxypupil.cpp
	ragdoll.cpp
	recvproxy.cpp
	rendertexture.cpp
	simple_keys.cpp
	smoke_fog_overlay.cpp
	splinepatch.cpp
	spritemodel.cpp
	stdafx.cpp
	text_message.cpp
	texturescrollmaterialproxy.cpp
	timematerialproxy.cpp
	toggletextureproxy.cpp
	train.cpp
	vgui_basepanel.cpp
	vgui_bitmapbutton.cpp
	vgui_bitmapimage.cpp
	vgui_bitmappanel.cpp
	vgui_centerstringpanel.cpp
	vgui_consolepanel.cpp
	vgui_debugoverlaypanel.cpp
	vgui_fpspanel.cpp
	vgui_game_viewport.cpp
	vgui_grid.cpp
	vgui_int.cpp
	vgui_loadingdiscpanel.cpp
	vgui_messagechars.cpp
	vgui_netgraphpanel.cpp
	vgui_textmessagepanel.cpp
	view.cpp
	view_beams.cpp
	view_effects.cpp
	view_scene.cpp
	viewconeimage.cpp
	warp_overlay.cpp
	waterlodmaterialproxy.cpp
	weapon_selection.cpp
	weapons_resource.cpp
	worlddimsproxy.cpp
)

IF(BUILD_HL2)
	SET( CLIENT_SRCS
		${CLIENT_SRCS}
		hud_chat.cpp
		death.cpp
		hud_squadstatus.cpp
	)
ENDIF(BUILD_HL2)

IF(BUILD_HL2MP)
	SET( CLIENT_SRCS
		${CLIENT_SRCS}
		hud_squadstatus.cpp
		hud_voicestatus.cpp
	)
ENDIF(BUILD_HL2MP)

IF(BUILD_SDK)
	SET( CLIENT_SRCS
		${CLIENT_SRCS}
		hud_voicestatus.cpp
	)
ENDIF(BUILD_SDK)

SET( CLIENT_HL2_HUD_SRCS
	hl2_hud/c_waterbullet.cpp
	hl2_hud/c_waterlodcontrol.cpp
)

IF(BUILD_HL2 OR BUILD_HL2MP)
	SET( CLIENT_HL2_HUD_SRCS
		${CLIENT_HL2_HUD_SRCS}
		hl2_hud/c_antlion_dust.cpp
		hl2_hud/c_ar2_explosion.cpp
		hl2_hud/c_barnacle.cpp
		hl2_hud/c_barney.cpp
		hl2_hud/c_basehelicopter.cpp
		hl2_hud/c_basehlcombatweapon.cpp
		hl2_hud/c_basehlplayer.cpp
		hl2_hud/c_citadel_effects.cpp
		hl2_hud/c_corpse.cpp
		hl2_hud/c_env_alyxtemp.cpp
		hl2_hud/c_env_headcrabcanister.cpp
		hl2_hud/c_env_starfield.cpp
		hl2_hud/c_func_monitor.cpp
		hl2_hud/c_func_tankmortar.cpp
		hl2_hud/c_hl2_playerlocaldata.cpp
		hl2_hud/c_info_teleporter_countdown.cpp
		hl2_hud/c_npc_combinegunship.cpp
		hl2_hud/c_npc_manhack.cpp
		hl2_hud/c_npc_rollermine.cpp
		hl2_hud/c_plasma_beam_node.cpp
		hl2_hud/c_prop_combine_ball.cpp
		hl2_hud/c_rotorwash.cpp
		hl2_hud/c_script_intro.cpp
		hl2_hud/c_strider.cpp
		hl2_hud/c_te_concussiveexplosion.cpp
		hl2_hud/c_te_flare.cpp
		hl2_hud/c_thumper_dust.cpp
		hl2_hud/c_vehicle_airboat.cpp
		hl2_hud/c_vehicle_cannon.cpp
		hl2_hud/c_vehicle_crane.cpp
		hl2_hud/c_vehicle_prisoner_pod.cpp
		hl2_hud/c_weapon__stubs_hl2.cpp
		hl2_hud/c_weapon_crossbow.cpp
		hl2_hud/fx_antlion.cpp
		hl2_hud/fx_bugbait.cpp
		hl2_hud/fx_hl2_impacts.cpp
		hl2_hud/fx_hl2_tracers.cpp
		hl2_hud/hl2_clientmode.cpp
		hl2_hud/hl_in_main.cpp
		hl2_hud/hl_prediction.cpp
		hl2_hud/hud_ammo.cpp
		hl2_hud/hud_battery.cpp
		hl2_hud/hud_blood.cpp
		hl2_hud/hud_credits.cpp
		hl2_hud/hud_damageindicator.cpp
		hl2_hud/hud_flashlight.cpp
		hl2_hud/hud_health.cpp
		hl2_hud/hud_poisondamageindicator.cpp
		hl2_hud/hud_quickinfo.cpp
		hl2_hud/hud_suitpower.cpp
		hl2_hud/hud_weaponselection.cpp
		hl2_hud/hud_zoom.cpp
		hl2_hud/shieldproxy.cpp
		hl2_hud/vgui_rootpanel_hl2.cpp
	)
ENDIF(BUILD_HL2 OR BUILD_HL2MP)

IF(BUILD_HL2)
	SET( CLIENT_HL2_HUD_SRCS
		${CLIENT_HL2_HUD_SRCS}
		hl2_hud/c_weapon_physcannon.cpp
		hl2_hud/c_weapon_stunstick.cpp
		hl2_hud/clientmode_hlnormal.cpp
	)
ENDIF(BUILD_HL2)

SET( CLIENT_HL2MP_SRCS
	hl2mp/c_hl2mp_player.cpp
	hl2mp/c_te_hl2mp_shotgun_shot.cpp
	hl2mp/clientmode_hl2mpnormal.cpp
	hl2mp/hl2mp_hud_chat.cpp
	hl2mp/hl2mp_hud_target_id.cpp
	hl2mp/hl2mp_hud_team.cpp
	hl2mp/hud_deathnotice.cpp
	hl2mp/ui/backgroundpanel.cpp
	hl2mp/ui/hl2mpclientscoreboard.cpp
	hl2mp/ui/hl2mptextwindow.cpp
)

SET( CLIENT_SDK_SRCS
	sdk/c_sdk_env_sparkler.cpp
	sdk/c_sdk_player.cpp
	sdk/c_sdk_team.cpp
	sdk/c_te_firebullets.cpp
	sdk/clientmode_sdk.cpp
	sdk/sdk_fx_impacts.cpp
	sdk/sdk_fx_weaponfx.cpp
	sdk/sdk_hud_ammo.cpp
	sdk/sdk_hud_chat.cpp
	sdk/sdk_hud_health.cpp
	sdk/sdk_hud_message.cpp
	sdk/sdk_hud_weaponselection.cpp
	sdk/sdk_in_main.cpp
	sdk/sdk_prediction.cpp
)

SET( CLIENT_SDK_VGUI_SRCS
	sdk/vgui/sdkviewport.cpp
	sdk/vgui/vgui_rootpanel_sdk.cpp
)

SET( CLIENT_GAME_CONTROLS_SRCS
	game_controls/clientscoreboarddialog.cpp
	game_controls/commandmenu.cpp
	game_controls/mapoverview.cpp
	game_controls/navprogress.cpp
	game_controls/spectatorgui.cpp
	game_controls/teammenu.cpp
	game_controls/vgui_teamfortressviewport.cpp
	game_controls/vguitextwindow.cpp
)

IF(BUILD_SDK)
	SET( CLIENT_GAME_CONTROLS_SRCS
		${CLIENT_GAME_CONTROLS_SRCS}
		game_controls/buymenu.cpp
		game_controls/buysubmenu.cpp
		game_controls/classmenu.cpp
	)
ENDIF(BUILD_SDK)

SET( COMMON_SRCS
	../common/randoverride.cpp
)

SET( GAME_SHARED_SRCS
	../game_shared/activitylist.cpp
	../game_shared/ammodef.cpp
	../game_shared/animation.cpp
	../game_shared/base_playeranimstate.cpp
	../game_shared/basecombatcharacter_shared.cpp
	../game_shared/basecombatweapon_shared.cpp
	../game_shared/baseentity_shared.cpp
	../game_shared/basegrenade_shared.cpp
	../game_shared/baseparticleentity.cpp
	../game_shared/baseplayer_shared.cpp
	../game_shared/baseviewmodel_shared.cpp
	../game_shared/beam_shared.cpp
	../game_shared/collisionproperty.cpp
	../game_shared/death_pose.cpp
	../game_shared/decals.cpp
	../game_shared/effect_dispatch_data.cpp
	../game_shared/ehandle.cpp
	../game_shared/entitylist_base.cpp
	../game_shared/entityparticletrail_shared.cpp
	../game_shared/env_detail_controller.cpp
	../game_shared/env_wind_shared.cpp
	../game_shared/eventlist.cpp
	../game_shared/func_ladder.cpp
	../game_shared/gamemovement.cpp
	../game_shared/gamerules.cpp
	../game_shared/gamerules_register.cpp
	../game_shared/gamestringpool.cpp
	../game_shared/gamevars_shared.cpp
	../game_shared/igamesystem.cpp
	../game_shared/interval.cpp
	../game_shared/mapentities_shared.cpp
	../game_shared/movevars_shared.cpp
	../game_shared/multiplay_gamerules.cpp
	../game_shared/physics_main_shared.cpp
	../game_shared/physics_saverestore.cpp
	../game_shared/physics_shared.cpp
	../game_shared/precache_register.cpp
	../game_shared/predictableid.cpp
	../game_shared/predictioncopy.cpp
	../game_shared/props_shared.cpp
	../game_shared/ragdoll_shared.cpp
	../game_shared/rope_helpers.cpp
	../game_shared/saverestore.cpp
	../game_shared/sheetsimulator.cpp
	../game_shared/singleplay_gamerules.cpp
	../game_shared/soundemittersystem.cpp
	../game_shared/soundenvelope.cpp
	../game_shared/sprite.cpp
	../game_shared/spritetrail.cpp
	../game_shared/studio_shared.cpp
	../game_shared/takedamageinfo.cpp
	../game_shared/test_ehandle.cpp
	../game_shared/usercmd.cpp
	../game_shared/usermessages.cpp
	../game_shared/util_shared.cpp
	../game_shared/voice_banmgr.cpp
	../game_shared/voice_status.cpp
	../game_shared/weapon_parse.cpp
)

IF(BUILD_HL2)
	SET( GAME_SHARED_SRCS
		${GAME_SHARED_SRCS}
		../game_shared/weapon_parse_default.cpp
	)
ENDIF(BUILD_HL2)

IF(BUILD_HL2MP)
	SET( GAME_SHARED_SRCS
		${GAME_SHARED_SRCS}
		../game_shared/predicted_viewmodel.cpp
	)
ENDIF(BUILD_HL2MP)

IF(BUILD_SDK)
	SET( GAME_SHARED_SRCS
		${GAME_SHARED_SRCS}
		../game_shared/predicted_viewmodel.cpp
	)
ENDIF(BUILD_SDK)

SET( GAME_SHARED_HL2_SRCS
	../game_shared/hl2/basehlcombatweapon_shared.cpp
	../game_shared/hl2/env_headcrabcanister_shared.cpp
	../game_shared/hl2/hl2_gamerules.cpp
	../game_shared/hl2/hl2_usermessages.cpp
	../game_shared/hl2/hl_gamemovement.cpp
)

SET( GAME_SHARED_HL2MP_SRCS
	../game_shared/hl2mp/hl2mp_gamerules.cpp
	../game_shared/hl2mp/hl2mp_player_shared.cpp
	../game_shared/hl2mp/hl2mp_weapon_parse.cpp
	../game_shared/hl2mp/weapon_357.cpp
	../game_shared/hl2mp/weapon_ar2.cpp
	../game_shared/hl2mp/weapon_crossbow.cpp
	../game_shared/hl2mp/weapon_crowbar.cpp
	../game_shared/hl2mp/weapon_frag.cpp
	../game_shared/hl2mp/weapon_hl2mpbase.cpp
	../game_shared/hl2mp/weapon_hl2mpbase_machinegun.cpp
	../game_shared/hl2mp/weapon_hl2mpbasebasebludgeon.cpp
	../game_shared/hl2mp/weapon_hl2mpbasehlmpcombatweapon.cpp
	../game_shared/hl2mp/weapon_physcannon.cpp
	../game_shared/hl2mp/weapon_pistol.cpp
	../game_shared/hl2mp/weapon_rpg.cpp
	../game_shared/hl2mp/weapon_shotgun.cpp
	../game_shared/hl2mp/weapon_slam.cpp
	../game_shared/hl2mp/weapon_smg1.cpp
	../game_shared/hl2mp/weapon_stunstick.cpp
)

SET( GAME_SHARED_SDK_SRCS
	../game_shared/sdk/sdk_basegrenade_projectile.cpp
	../game_shared/sdk/sdk_fx_shared.cpp
	../game_shared/sdk/sdk_gamemovement.cpp
	../game_shared/sdk/sdk_gamerules.cpp
	../game_shared/sdk/sdk_player_shared.cpp
	../game_shared/sdk/sdk_playeranimstate.cpp
	../game_shared/sdk/sdk_usermessages.cpp
	../game_shared/sdk/sdk_weapon_parse.cpp
	../game_shared/sdk/weapon_basesdkgrenade.cpp
	../game_shared/sdk/weapon_grenade.cpp
	../game_shared/sdk/weapon_mp5.cpp
	../game_shared/sdk/weapon_sdkbase.cpp
	../game_shared/sdk/weapon_shotgun.cpp
)

SET( PUBLIC_SRCS
	../public/bone_accessor.cpp
	../public/bone_setup.cpp
	../public/client_class.cpp
	../public/collisionutils.cpp
	../public/crtmemdebug.cpp
	../public/dt_recv.cpp
	../public/dt_utlvector_common.cpp
	../public/dt_utlvector_recv.cpp
	../public/filesystem_helpers.cpp
	../public/icekey.cpp
	../public/imageloader.cpp
	../public/mathlib.cpp
	../public/networkvar.cpp
	../public/rope_physics.cpp
	../public/scratchpad3d.cpp
	../public/scratchpadutils.cpp
	../public/sentence.cpp
	../public/simple_physics.cpp
	../public/soundparametersinternal.cpp
	../public/stringregistry.cpp
	../public/studio.cpp
	../public/tgaloader.cpp
	../public/tgawriter.cpp
	../public/vallocator.cpp
	../public/vmatrix.cpp
)

SET( MATERIALS_SRCS
	../public/materialsystem/MaterialSystemUtil.cpp
)

SET( TIER0_SRCS
	../public/tier0/memoverride.cpp
)

SET( TIER1_SRCS
	../tier1/bitbuf.cpp
	../tier1/characterset.cpp
	../tier1/checksum_crc.cpp
	../tier1/checksum_md5.cpp
	../tier1/convar.cpp
	../tier1/interface.cpp
	../tier1/keyvalues.cpp
	../tier1/mempool.cpp
	../tier1/rangecheckedvar.cpp
	../tier1/resourcemanager.cpp
	../tier1/stringpool.cpp
	../tier1/utlbuffer.cpp
	../tier1/utlsymbol.cpp
)

IF(BUILD_HL2)
	SET(CLIENT_BUILD_SRCS
		${GAME_SHARED_HL2_SRCS}
	)	
	SET(CLIENT_BUILD_INC_CL_DLL ${HL2SDK_SOURCE_DIR}/src/cl_dll/hl2_hud )
	SET(CLIENT_BUILD_INC_GAME_SHARED ${HL2SDK_SOURCE_DIR}/src/game_shared/hl2 )
ENDIF(BUILD_HL2)

IF(BUILD_HL2MP)
	SET(CLIENT_BUILD_SRCS
		${GAME_SHARED_HL2_SRCS}
		${GAME_SHARED_HL2MP_SRCS}
		${CLIENT_HL2MP_SRCS}
	)	
	SET(CLIENT_BUILD_INC_CL_DLL
		${HL2SDK_SOURCE_DIR}/src/cl_dll/hl2mp
		${HL2SDK_SOURCE_DIR}/src/cl_dll/hl2mp/ui
	)
	SET(CLIENT_BUILD_INC_GAME_SHARED ${HL2SDK_SOURCE_DIR}/src/game_shared/hl2mp )
ENDIF(BUILD_HL2MP)

IF(BUILD_SDK)
	SET(CLIENT_BUILD_SRCS
		${CLIENT_SDK_SRCS}
		${CLIENT_SDK_VGUI_SRCS}
		${GAME_SHARED_SDK_SRCS}
	)	
	SET(CLIENT_BUILD_INC_CL_DLL ${HL2SDK_SOURCE_DIR}/src/cl_dll/sdk )
	SET(CLIENT_BUILD_INC_GAME_SHARED ${HL2SDK_SOURCE_DIR}/src/game_shared/sdk )
ENDIF(BUILD_SDK)

SET( CLIENT_ALL_SRCS
	${CLIENT_SRCS}
	${CLIENT_HL2_HUD_SRCS}
	${CLIENT_GAME_CONTROLS_SRCS}
	${GAME_SHARED_SRCS}
	${PUBLIC_SRCS}
	${TIER0_SRCS}
	${TIER1_SRCS}
	${MATERIALS_SRCS}
	${CLIENT_BUILD_SRCS}
	${COMMON_SRCS}
)

SOURCE_GROUP(client			FILES ${CLIENT_SRCS} ${CLIENT_SDK_SRCS} ${CLIENT_HL2_HUD_SRCS} ${CLIENT_HL2MP_SRCS})
SOURCE_GROUP(common			FILES ${COMMON_SRCS})
SOURCE_GROUP(game_controls	FILES ${CLIENT_GAME_CONTROLS_SRCS})
SOURCE_GROUP(game_shared	FILES ${GAME_SHARED_SRCS} ${GAME_SHARED_SDK_SRCS} ${GAME_SHARED_HL2_SRCS} ${GAME_SHARED_HL2MP_SRCS})
SOURCE_GROUP(materials		FILES ${MATERIALS_SRCS})
SOURCE_GROUP(public			FILES ${PUBLIC_SRCS})
SOURCE_GROUP(tier0			FILES ${TIER0_SRCS})
SOURCE_GROUP(tier1			FILES ${TIER1_SRCS})
SOURCE_GROUP(vgui			FILES ${CLIENT_SDK_VGUI_SRCS})

INCLUDE_DIRECTORIES(
	${HL2SDK_SOURCE_DIR}/src/public/tier1
	${HL2SDK_SOURCE_DIR}/src/cl_dll/sdk/vgui
	${CLIENT_BUILD_INC_CL_DLL}
	${CLIENT_BUILD_INC_GAME_SHARED}
	${HL2SDK_SOURCE_DIR}/src/cl_dll
	${HL2SDK_SOURCE_DIR}/src/public
	${HL2SDK_SOURCE_DIR}/src/vgui2/include
	${HL2SDK_SOURCE_DIR}/src/vgui2/controls
	${HL2SDK_SOURCE_DIR}/src/game_shared
	${HL2SDK_SOURCE_DIR}/src/cl_dll/game_controls
	${HL2SDK_SOURCE_DIR}/src/dlls
	${HL2SDK_SOURCE_DIR}/src/game_shared/hl2
	${HL2SDK_SOURCE_DIR}/src/cl_dll/hl2_hud
)

ADD_DEFINITIONS(
	${SDK_BUILD_FLAGS}
	${SDK_BUILD_CLIENT_FLAGS}
	-Dfopen=dont_use_fopen
	-DNO_STRING_T
	-DCLIENT_DLL
	-D_WINDOWS
	-DVECTOR
	-Dstrncpy=use_Q_strncpy_instead
	-D_snprintf=use_Q_snprintf_instead
	-D_WIN32
	-DPROTECTED_THINGS_ENABLE
)

ADD_LIBRARY(client SHARED ${CLIENT_ALL_SRCS})

IF(WIN32)
	SET_TARGET_PROPERTIES(client PROPERTIES LINK_FLAGS "/FORCE:MULTIPLE")
ENDIF(WIN32)

LINK_DIRECTORIES(
	${HL2SDK_SOURCE_DIR}/src/lib/public
)

TARGET_LINK_LIBRARIES(client tier0 vstdlib nvtc vgui_controls winmm)

INSTALL_TARGETS(/bin client)