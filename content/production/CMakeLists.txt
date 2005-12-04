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
)

INSTALL_FILES(/maps FILES
	maps/sdk_vehicles.bsp
)

INSTALL_FILES(/materials/models/player/terror FILES
	materials/models/player/terror/TerrorTemp.vmt
	materials/models/player/terror/TerrorTemp.vtf
)

INSTALL_FILES(/materials/models/shells/9mm FILES
	materials/models/shells/9mm/shell_9mm.vmt
	materials/models/shells/9mm/shell_9mm.vtf
)

INSTALL_FILES(/materials/models/shells/12gauge FILES
	materials/models/shells/12gauge/shell_12gauge.vmt
	materials/models/shells/12gauge/shell_12gauge.vtf
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

INSTALL_FILES(/materials/models/weapons/v_models/hands FILES
	materials/models/weapons/v_models/hands/HandTemp.vmt
	materials/models/weapons/v_models/hands/HandTemp.vtf
	materials/models/weapons/v_models/hands/smokegrenade_ref.vtf
	materials/models/weapons/v_models/hands/v_hands.vmt
	materials/models/weapons/v_models/hands/v_hands.vtf
	materials/models/weapons/v_models/hands/v_hands_normal.vmt
	materials/models/weapons/v_models/hands/v_hands_normal.vtf
	materials/models/weapons/v_models/hands/v_hands_ref.vtf
	materials/models/weapons/v_models/hands/view_finger.vmt
	materials/models/weapons/v_models/hands/view_finger.vtf
	materials/models/weapons/v_models/hands/view_glove.vmt
	materials/models/weapons/v_models/hands/view_glove.vtf
	materials/models/weapons/v_models/hands/view_skin.vmt
	materials/models/weapons/v_models/hands/view_skin.vtf
)

INSTALL_FILES(/materials/models/weapons/v_models/shot_m3super90 FILES
	materials/models/weapons/v_models/shot_m3super90/shot_m3super90.vmt
	materials/models/weapons/v_models/shot_m3super90/shot_m3super90.vtf
	materials/models/weapons/v_models/shot_m3super90/shot_m3super90_height.vtf
	materials/models/weapons/v_models/shot_m3super90/shot_m3super90_normal.vmt
	materials/models/weapons/v_models/shot_m3super90/shot_m3super90_normal.vtf
	materials/models/weapons/v_models/shot_m3super90/shot_m3super90_ref.vtf
)

INSTALL_FILES(/materials/models/weapons/v_models/smg_mp5 FILES
	materials/models/weapons/v_models/smg_mp5/MP5_1.vmt
	materials/models/weapons/v_models/smg_mp5/MP5_1.vtf
	materials/models/weapons/v_models/smg_mp5/MP5_1_normal.vmt
	materials/models/weapons/v_models/smg_mp5/MP5_1_normal.vtf
	materials/models/weapons/v_models/smg_mp5/MP5_1_ref.vtf
)

INSTALL_FILES(/materials/models/weapons/w_models/w_eq_fraggrenade FILES
	materials/models/weapons/w_models/w_eq_fraggrenade/w_eq_fraggrenade.vmt
	materials/models/weapons/w_models/w_eq_fraggrenade/w_eq_fraggrenade.vtf
)

INSTALL_FILES(/materials/models/weapons/w_models/w_shot_m3super90 FILES
	materials/models/weapons/w_models/w_shot_m3super90/w_shot_m3super90.vmt
	materials/models/weapons/w_models/w_shot_m3super90/w_shot_m3super90.vtf
)

INSTALL_FILES(/materials/models/weapons/w_models/w_smg_mp5 FILES
	materials/models/weapons/w_models/w_smg_mp5/w_smg_mp5.vmt
	materials/models/weapons/w_models/w_smg_mp5/w_smg_mp5.vtf
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
	models/shells/shell_12gauge.dx80.vtx
	models/shells/shell_12gauge.dx90.vtx
	models/shells/shell_12gauge.mdl
	models/shells/shell_12gauge.sw.vtx
	models/shells/shell_12gauge.vvd
)

INSTALL_FILES(/models/weapons FILES
	models/weapons/v_eq_fraggrenade.dx80.vtx
	models/weapons/v_eq_fraggrenade.dx90.vtx
	models/weapons/v_eq_fraggrenade.mdl
	models/weapons/v_eq_fraggrenade.sw.vtx
	models/weapons/v_eq_fraggrenade.vvd
	models/weapons/v_shot_m3super90.dx80.vtx
	models/weapons/v_shot_m3super90.dx90.vtx
	models/weapons/v_shot_m3super90.mdl
	models/weapons/v_shot_m3super90.sw.vtx
	models/weapons/v_shot_m3super90.vvd
	models/weapons/v_smg_mp5.dx80.vtx
	models/weapons/v_smg_mp5.dx90.vtx
	models/weapons/v_smg_mp5.mdl
	models/weapons/v_smg_mp5.sw.vtx
	models/weapons/v_smg_mp5.vvd
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
	models/weapons/w_shot_m3super90.dx80.vtx
	models/weapons/w_shot_m3super90.dx90.vtx
	models/weapons/w_shot_m3super90.mdl
	models/weapons/w_shot_m3super90.phy
	models/weapons/w_shot_m3super90.sw.vtx
	models/weapons/w_shot_m3super90.vvd
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
	scripts/weapon_mp5.txt
	scripts/weapon_shotgun.txt
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

INSTALL_FILES(/sound/weapons/m3 FILES
	sound/weapons/m3/m3-1.wav
	sound/weapons/m3/m3_insertshell.wav
	sound/weapons/m3/m3_pump.wav
)

INSTALL_FILES(/sound/weapons/mp5navy FILES
	sound/weapons/mp5navy/mp5-1.wav
	sound/weapons/mp5navy/mp5_clipin.wav
	sound/weapons/mp5navy/mp5_clipout.wav
	sound/weapons/mp5navy/mp5_slideback.wav
)