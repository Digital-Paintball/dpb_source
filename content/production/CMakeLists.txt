# This file contains a file manifest of all the files that the HL2SDK should
# install. They will be installed when you run the appropriate install
# command, which depends entirely on which compiler and operating system you
# are using. With nmake makefiles, you run "nmake install". With GNUMake you
# run "make install". With Visual Studio you right click the INSTALL target
# and choose "Build". With anything else, you're on your own.

INSTALL_FILES(/ FILES
	GameInfo.txt
	maplist.txt
	motd.txt
	dpb.fgd
)

INSTALL_FILES(/maps FILES
	maps/dpb_begin.bsp
)

INSTALL_FILES(/materials/models/inflatables FILES
	materials/models/inflatables/checker.vtf
	materials/models/inflatables/pyramid.vmt
	materials/models/inflatables/pyramid.vtf
	materials/models/inflatables/pyramid_normal.vtf
)

INSTALL_FILES(/materials/models/player/terror FILES
	materials/models/player/terror/TerrorTemp.vmt
	materials/models/player/terror/TerrorTemp.vtf
)

INSTALL_FILES(/materials/models/shells/9mm FILES
	materials/models/shells/9mm/shell_9mm.vmt
	materials/models/shells/9mm/shell_9mm.vtf
)

INSTALL_FILES(/materials/models/weapons/p_models/eq_fraggrenade FILES
	materials/models/weapons/p_models/eq_fraggrenade/fraggrenade.vmt
	materials/models/weapons/p_models/eq_fraggrenade/fraggrenade.vtf
)

INSTALL_FILES(/materials/models/weapons/v_models/eq_fraggrenade FILES
	materials/models/weapons/v_models/eq_fraggrenade/fraggrenade.vmt
	materials/models/weapons/v_models/eq_fraggrenade/fraggrenade.vtf
	materials/models/weapons/v_models/eq_fraggrenade/fraggrenade_height.vtf
	materials/models/weapons/v_models/eq_fraggrenade/fraggrenade_normal.vmt
	materials/models/weapons/v_models/eq_fraggrenade/fraggrenade_normal.vtf
	materials/models/weapons/v_models/eq_fraggrenade/fraggrenade_ref.vtf
)

INSTALL_FILES(/materials/models/weapons FILES
	materials/models/weapons/v_smg.vmt
	materials/models/weapons/v_smg.vtf
	materials/models/weapons/v_smg_hopper.vmt
	materials/models/weapons/v_smg_hopper.vtf
	materials/models/weapons/v_smg_tank.vmt
	materials/models/weapons/v_smg_tank.vtf
)

INSTALL_FILES(/materials/models/weapons/w_models/w_eq_fraggrenade FILES
	materials/models/weapons/w_models/w_eq_fraggrenade/w_eq_fraggrenade.vmt
	materials/models/weapons/w_models/w_eq_fraggrenade/w_eq_fraggrenade.vtf
)

INSTALL_FILES(/materials/models/weapons/w_models/w_smg_mp5 FILES
	materials/models/weapons/w_models/w_smg_mp5/w_smg_mp5.vmt
	materials/models/weapons/w_models/w_smg_mp5/w_smg_mp5.vtf
)

INSTALL_FILES(/models FILES
	models/pyramid_inflatable.dx80.vtx
	models/pyramid_inflatable.dx90.vtx
	models/pyramid_inflatable.mdl
	models/pyramid_inflatable.phy
	models/pyramid_inflatable.sw.vtx
	models/pyramid_inflatable.vvd
)

INSTALL_FILES(/models/player FILES
	models/player/ak_anims_t.mdl
	models/player/cs_player_shared.mdl
	models/player/player_animations.ani
	models/player/player_animations.mdl
	models/player/Police_SDK.dx80.vtx
	models/player/Police_SDK.dx90.vtx
	models/player/Police_SDK.mdl
	models/player/Police_SDK.phy
	models/player/Police_SDK.sw.vtx
	models/player/Police_SDK.vvd
	models/player/terror.dx80.vtx
	models/player/terror.dx90.vtx
	models/player/terror.mdl
	models/player/terror.phy
	models/player/terror.sw.vtx
	models/player/terror.vvd
)

INSTALL_FILES(/models/shells FILES
	models/shells/shell_9mm.dx80.vtx
	models/shells/shell_9mm.dx90.vtx
	models/shells/shell_9mm.mdl
	models/shells/shell_9mm.sw.vtx
	models/shells/shell_9mm.vvd
)

INSTALL_FILES(/models/weapons FILES
	models/weapons/v_eq_fraggrenade.dx80.vtx
	models/weapons/v_eq_fraggrenade.dx90.vtx
	models/weapons/v_eq_fraggrenade.mdl
	models/weapons/v_eq_fraggrenade.sw.vtx
	models/weapons/v_eq_fraggrenade.vvd
	models/weapons/v_smg1.dx80.vtx
	models/weapons/v_smg1.dx90.vtx
	models/weapons/v_smg1.mdl
	models/weapons/v_smg1.sw.vtx
	models/weapons/v_smg1.vvd
	models/weapons/w_eq_fraggrenade.dx80.vtx
	models/weapons/w_eq_fraggrenade.dx90.vtx
	models/weapons/w_eq_fraggrenade.mdl
	models/weapons/w_eq_fraggrenade.phy
	models/weapons/w_eq_fraggrenade.sw.vtx
	models/weapons/w_eq_fraggrenade.vvd
	models/weapons/w_eq_fraggrenade_thrown.dx80.vtx
	models/weapons/w_eq_fraggrenade_thrown.dx90.vtx
	models/weapons/w_eq_fraggrenade_thrown.mdl
	models/weapons/w_eq_fraggrenade_thrown.phy
	models/weapons/w_eq_fraggrenade_thrown.sw.vtx
	models/weapons/w_eq_fraggrenade_thrown.vvd
	models/weapons/w_smg_mp5.dx80.vtx
	models/weapons/w_smg_mp5.dx90.vtx
	models/weapons/w_smg_mp5.mdl
	models/weapons/w_smg_mp5.phy
	models/weapons/w_smg_mp5.sw.vtx
	models/weapons/w_smg_mp5.vvd
)

INSTALL_FILES(/resource FILES
	resource/ClientScheme.res
	resource/game_menu.tga
	resource/game_menu_mouseover.tga
	resource/GameMenu.res
	resource/ModEvents.res
	resource/sampleapp_english.txt
	resource/sampleapp_german.txt
	resource/spectatormenu.res
	resource/spectatormodes.res
	resource/TrackerScheme.res
)

INSTALL_FILES(/resource/ui FILES
	resource/ui/MOTD.res
	resource/ui/ScoreBoard.res
	resource/ui/TextWindow.res
)

INSTALL_FILES(/scripts FILES
	scripts/game.txt
	scripts/game_sounds_manifest.txt
	scripts/game_sounds_manifest.txt
	scripts/kb_act.lst
	scripts/kb_def.lst
	scripts/kb_keys.lst
	scripts/liblist.gam
	scripts/weapon_grenade.txt
	scripts/weapon_marker.txt
	scripts/HudLayout.res
)

INSTALL_FILES(/scripts/vehicles FILES
	scripts/vehicles/jeep_test.txt
)

INSTALL_FILES(/sound/weapons FILES
	sound/weapons/pinpull.wav
)

INSTALL_FILES(/sound/weapons/hegrenade FILES
	sound/weapons/hegrenade/explode3.wav
	sound/weapons/hegrenade/explode4.wav
	sound/weapons/hegrenade/explode5.wav
	sound/weapons/hegrenade/he_bounce-1.wav
)