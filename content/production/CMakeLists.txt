# This file contains a file manifest of all the files that the HL2SDK should
# install. They will be installed when you run the appropriate install
# command, which depends entirely on which compiler and operating system you
# are using. With nmake makefiles, you run "nmake install". With GNUMake you
# run "make install". With Visual Studio you right click the INSTALL target
# and choose "Build". With anything else, you're on your own.

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/GameInfo.txt.in ${CMAKE_CURRENT_BINARY_DIR}/GameInfo.txt)

INSTALL_FILES(/ FILES
	GameInfo.txt
	maplist.txt
	motd.txt
	dpb.fgd
	dpb.tga
	serverconfig.vdf
)

INSTALL_FILES(/DownloadLists FILES
	DownloadLists/dpb_begin.lst
)

INSTALL_FILES(/maps FILES
	maps/dpb_begin.bsp
	maps/dpb_rec.bsp
)

INSTALL_FILES(/materials/console FILES
	materials/console/background01.vmt
	materials/console/background01.vtf
)

INSTALL_FILES(/materials/models FILES
	materials/models/testblock.vmt
)

INSTALL_FILES(/materials/models/inflatables FILES
	materials/models/inflatables/map1.vtf
	materials/models/inflatables/map2.vtf
	materials/models/inflatables/map3.vtf
	materials/models/inflatables/dorito01.vtf
	materials/models/inflatables/checker.vtf
	materials/models/inflatables/map1.vmt
	materials/models/inflatables/map2.vmt
	materials/models/inflatables/map3.vmt
	materials/models/inflatables/ryginflatable_pyramid.vtf
	materials/models/inflatables/dorito01.vmt
	materials/models/inflatables/dorito01_normal.vtf
	materials/models/inflatables/pyramid.vmt
	materials/models/inflatables/pyramid.vtf
	materials/models/inflatables/pyramid_normal.vtf
	materials/models/inflatables/ryginflatable_pyramid.vmt
	materials/models/inflatables/rygInflatable_pyramid_normal.vtf
)

INSTALL_FILES(/materials/models/player/ FILES
	materials/models/player/proto_blue.vmt
	materials/models/player/glass_spec.vtf
	materials/models/player/body1_blue.VTF
	materials/models/player/vforce_red.vmt
	materials/models/player/body2_blue.vtf
	materials/models/player/body4_blue.vtf
	materials/models/player/vforce_blue.vtf
	materials/models/player/body1_blue.vmt
	materials/models/player/proto_red.vtf
	materials/models/player/body3_blue.vmt
	materials/models/player/invision_red.vmt
	materials/models/player/body.vmt
	materials/models/player/invision_blue.vtf
	materials/models/player/face.vmt
	materials/models/player/Chrome_reflect-b.vmt
	materials/models/player/proto_blue.vtf
	materials/models/player/face_blue.vmt
	materials/models/player/vforce_red.vtf
	materials/models/player/body2_red.vmt
	materials/models/player/body3_red.vmt
	materials/models/player/body4_red.vmt
	materials/models/player/body3_blue.vtf
	materials/models/player/invision_red.vtf
	materials/models/player/Chrome_reflect.vmt
	materials/models/player/body.vtf
	materials/models/player/body2_blue.vmt
	materials/models/player/face.vtf
	materials/models/player/Chrome_reflect-y.vmt
	materials/models/player/body4_blue.vmt
	materials/models/player/vforce_blue.vmt
	materials/models/player/proto_red.vmt
	materials/models/player/face_blue.vtf
	materials/models/player/glass.vtf
	materials/models/player/body2_red.vtf
	materials/models/player/body3_red.vtf
	materials/models/player/body4_red.vtf
	materials/models/player/invision_blue.vmt
)

INSTALL_FILES(/materials/models/props_cars/jeep_truck FILES
	materials/models/props_cars/jeep_truck/truck_final.vtf
	materials/models/props_cars/jeep_truck/truck_glass.vtf
	materials/models/props_cars/jeep_truck/truck_final.vmt
	materials/models/props_cars/jeep_truck/truck_glass.vmt
	materials/models/props_cars/jeep_truck/truck_final_normal.vtf
)

INSTALL_FILES(/materials/models/props_foliage/shrub FILES
	materials/models/props_foliage/shrub/shrub1.vtf
	materials/models/props_foliage/shrub/shrub1.vmt
)

INSTALL_FILES(/materials/models/props_foliage/pine FILES
	materials/models/props_foliage/pine/pinebark_light.vtf
	materials/models/props_foliage/pine/rygPineBillboard.vtf
	materials/models/props_foliage/pine/pinebark_light.vmt
	materials/models/props_foliage/pine/rygPineBillboard_illum.vtf
	materials/models/props_foliage/pine/rygPineBranch.vtf
	materials/models/props_foliage/pine/pinebark.vtf
	materials/models/props_foliage/pine/rygpinebillboard.vmt
	materials/models/props_foliage/pine/rygPineBranch.vmt
	materials/models/props_foliage/pine/pinebark.vmt
	materials/models/props_foliage/pine/rygPineBranch_normal.vtf
	materials/models/props_foliage/pine/pinebark_normal.vtf
	materials/models/props_foliage/pine/bry_pinebark.vtf
	materials/models/props_foliage/pine/pinebranch.vtf
	materials/models/props_foliage/pine/bry_pinebark.vmt
	materials/models/props_foliage/pine/bry_pinebark_normal.vtf
	materials/models/props_foliage/pine/pinebranch.vmt
)

INSTALL_FILES(/materials/models/props_proshop FILES
	materials/models/props_proshop/co2tank_green.vtf
	materials/models/props_proshop/co2tank_red.vmt
	materials/models/props_proshop/airtank_yellow.vtf
	materials/models/props_proshop/co2tank_blue_nofs.vmt
	materials/models/props_proshop/tank_normal.vtf
	materials/models/props_proshop/co2tank_red_nofs.vmt
	materials/models/props_proshop/co2tank_blue.vtf
	materials/models/props_proshop/co2tank_green.vmt
	materials/models/props_proshop/airtank_yellow.vmt
	materials/models/props_proshop/co2tank_red.vtf
	materials/models/props_proshop/co2tank_green_nofs.vmt
	materials/models/props_proshop/co2tank_blue.vmt
	materials/models/props_proshop/airtank_yellow_nofs.vmt
)

INSTALL_FILES(/materials/models/props_spool FILES
	materials/models/props_spool/spool_06.vtf
	materials/models/props_spool/spool_07.vtf
	materials/models/props_spool/spool_01.vmt
	materials/models/props_spool/spool_02.vmt
	materials/models/props_spool/spool_03.vmt
	materials/models/props_spool/spool_04.vmt
	materials/models/props_spool/spool_05.vmt
	materials/models/props_spool/spool_06.vmt
	materials/models/props_spool/spool_07.vmt
	materials/models/props_spool/spool_01.vtf
	materials/models/props_spool/spool_02.vtf
	materials/models/props_spool/spool_03.vtf
	materials/models/props_spool/spool_04.vtf
	materials/models/props_spool/spool_05.vtf
)

INSTALL_FILES(/materials/models/shadertest FILES
	materials/models/shadertest/envball_1.vtf
	materials/models/shadertest/envball_2.vtf
	materials/models/shadertest/envball_3.vtf
	materials/models/shadertest/envball_4.vtf
	materials/models/shadertest/envball_5.vtf
	materials/models/shadertest/envball_6_normal.vmt
	materials/models/shadertest/envball_6.vtf
	materials/models/shadertest/envball_1.vmt
	materials/models/shadertest/envball_2.vmt
	materials/models/shadertest/envball_3.vmt
	materials/models/shadertest/envball_4.vmt
	materials/models/shadertest/envball_5.vmt
	materials/models/shadertest/shieldnoise0_normal.vtf
	materials/models/shadertest/envball_6.vmt
	materials/models/shadertest/envball_6_normal.vtf
)

INSTALL_FILES(/materials/models/supair FILES
	materials/models/supair/rygPyramid2_normal.vtf
	materials/models/supair/rygpyramid3.vmt
	materials/models/supair/rygPyramid3_normal.vtf
	materials/models/supair/rygpyramid1.vtf
	materials/models/supair/rygpyramid2.vtf
	materials/models/supair/rygpyramid3.vtf
	materials/models/supair/rygpyramid1.vmt
	materials/models/supair/rygPyramid1_normal.vtf
	materials/models/supair/rygpyramid2.vmt
)

INSTALL_FILES(/materials/models/weapons FILES
	materials/models/weapons/arm_hand_normal.vtf
	materials/models/weapons/dpb_co2_20oz.vtf
	materials/models/weapons/pod_glass.vmt
	materials/models/weapons/gripsthing.vmt
	materials/models/weapons/gripsthing_normal.vtf
	materials/models/weapons/pod_main.vtf
	materials/models/weapons/blazer_normal.vtf
	materials/models/weapons/dpb_ppsbar_brs.vmt
	materials/models/weapons/dpb_gravhopper_lid.vtf
	materials/models/weapons/dpb_blazerEframe_normal.vtf
	materials/models/weapons/drop_asa.vmt
	materials/models/weapons/drop_asa_normal.vtf
	materials/models/weapons/dpb_ppsbar_blk.vtf
	materials/models/weapons/dpb_gravhopper_main.vtf
	materials/models/weapons/reg_stabilizer_black.vmt
	materials/models/weapons/dpb_brl_lpc_normal.vtf
	materials/models/weapons/pod_glass.vtf
	materials/models/weapons/hosefittings.vmt
	materials/models/weapons/dpb_ppsbar_svr.vmt
	materials/models/weapons/gripsthing.vtf
	materials/models/weapons/arm_hand.vmt
	materials/models/weapons/dpb_ppsbar_brs.vtf
	materials/models/weapons/dpb_blazerEframe.vtf
	materials/models/weapons/blazer.vmt
	materials/models/weapons/drop_asa.vtf
	materials/models/weapons/dpb_co2_20oz.vmt
	materials/models/weapons/dpb_co2_20oz_normal.vtf
	materials/models/weapons/dpb_blazereframe.vmt
	materials/models/weapons/pod_main.vmt
	materials/models/weapons/pod_main_normal.vtf
	materials/models/weapons/dpb_gravhopper_lid.vmt
	materials/models/weapons/reg_stabilizer_black.vtf
	materials/models/weapons/dpb_brl_lpc.vtf
	materials/models/weapons/dpb_ppsbar_svr.vtf
	materials/models/weapons/hosefittings.vtf
	materials/models/weapons/arm_hand.vtf
	materials/models/weapons/dpb_ppsbar_blk.vmt
	materials/models/weapons/dpb_gravhopper_main.vmt
	materials/models/weapons/dpb_gravhopper_main_normal.vtf
	materials/models/weapons/blazer.vtf
	materials/models/weapons/v_smg.vmt
	materials/models/weapons/v_smg.vtf
	materials/models/weapons/v_smg_hopper.vmt
	materials/models/weapons/v_smg_hopper.vtf
	materials/models/weapons/v_smg_tank.vmt
	materials/models/weapons/v_smg_tank.vtf
)

INSTALL_FILES(/materials/norrington FILES
	materials/norrington/rygProshopCarpet.vmt
	materials/norrington/rygProshopCarpet_normal.vtf
	materials/norrington/rygProshopWall.vmt
	materials/norrington/rygProshopWall_normal.vtf
	materials/norrington/rygCielingFoam.vmt
	materials/norrington/rygCielingFoam_normal.vtf
	materials/norrington/rygProshopCarpet.vtf
	materials/norrington/rygProshopWall.vtf
	materials/norrington/rygCielingFoam.vtf
)

INSTALL_FILES(/materials/VGUI/logos/UI FILES
	materials/VGUI/logos/UI/spray_canned.vmt
)

INSTALL_FILES(/models FILES
	models/rygPyramid.sw.vtx
	models/rygpyramid.mdl
	models/rygpyramid.vvd
	models/rygPyramid.phy
	models/rygPyramid.dx80.vtx
	models/rygPyramid.dx90.vtx
)

INSTALL_FILES(/models/inflatables FILES
	models/inflatables/dorito01.vvd
	models/inflatables/dorito01.jpg
	models/inflatables/dorito01.phy
	models/inflatables/dorito01.dx80.vtx
	models/inflatables/dorito01.dx90.vtx
	models/inflatables/dorito01.sw.vtx
	models/inflatables/dorito01.mdl
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
	models/player/player.vvd
	models/player/player.dx80.vtx
	models/player/player.dx90.vtx
	models/player/player.sw.vtx
	models/player/player.mdl
	models/player/player.jpg
)

INSTALL_FILES(/models/props_cars FILES
	models/props_cars/jeep_truck.sw.vtx
	models/props_cars/jeep_truck.mdl
	models/props_cars/jeep_truck.vvd
	models/props_cars/jeep_truck.phy
	models/props_cars/jeep_truck.dx80.vtx
	models/props_cars/jeep_truck.dx90.vtx
)

INSTALL_FILES(/models/props_foliage FILES
	models/props_foliage/rygPine.dx80.vtx
	models/props_foliage/pine_tree.sw.vtx
	models/props_foliage/pine_tree.mdl
	models/props_foliage/rygPine.sw.vtx
	models/props_foliage/rygpine.mdl
	models/props_foliage/bry_pine1.dx80.vtx
	models/props_foliage/bry_pine2.dx90.vtx
	models/props_foliage/bry_pine3.dx80.vtx
	models/props_foliage/bry_pine4.dx90.vtx
	models/props_foliage/bry_pine1.sw.vtx
	models/props_foliage/bry_pine3.sw.vtx
	models/props_foliage/bry_pine1.mdl
	models/props_foliage/bry_pine2.mdl
	models/props_foliage/bry_pine3.mdl
	models/props_foliage/bry_pine4.mdl
	models/props_foliage/shrub1.dx80.vtx
	models/props_foliage/pine_tree.vvd
	models/props_foliage/pine_tree.phy
	models/props_foliage/rygpine.vvd
	models/props_foliage/shrub1.sw.vtx
	models/props_foliage/pine_tree.dx90.vtx
	models/props_foliage/shrub1.mdl
	models/props_foliage/rygPine.dx90.vtx
	models/props_foliage/bry_pine1.vvd
	models/props_foliage/bry_pine1.phy
	models/props_foliage/bry_pine2.vvd
	models/props_foliage/bry_pine3.vvd
	models/props_foliage/bry_pine2.phy
	models/props_foliage/bry_pine4.vvd
	models/props_foliage/bry_pine3.phy
	models/props_foliage/bry_pine4.phy
	models/props_foliage/bry_pine1.dx90.vtx
	models/props_foliage/bry_pine2.dx80.vtx
	models/props_foliage/bry_pine4.dx80.vtx
	models/props_foliage/bry_pine3.dx90.vtx
	models/props_foliage/bry_pine2.sw.vtx
	models/props_foliage/shrub1.vvd
	models/props_foliage/bry_pine4.sw.vtx
	models/props_foliage/shrub1.dx90.vtx
	models/props_foliage/pine_tree.dx80.vtx
	models/props_foliage/rygPine.phy
)

INSTALL_FILES(/models/props_proshop FILES
	models/props_proshop/co2tank.vvd
	models/props_proshop/co2tank.JPG
	models/props_proshop/Airtank.sw.vtx
	models/props_proshop/co2tank.dx80.vtx
	models/props_proshop/airtank.mdl
	models/props_proshop/co2tank.dx90.vtx
	models/props_proshop/airtank.vvd
	models/props_proshop/airtank.jpg
	models/props_proshop/co2tank.sw.vtx
	models/props_proshop/Airtank.dx80.vtx
	models/props_proshop/Airtank.dx90.vtx
	models/props_proshop/co2tank.mdl
)

INSTALL_FILES(/models/props_spool FILES
	models/props_spool/spool.dx90.vtx
	models/props_spool/spool.sw.vtx
	models/props_spool/spool.mdl
	models/props_spool/spool.vvd
	models/props_spool/spool.phy
	models/props_spool/spool.dx80.vtx
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
	models/weapons/v_blazer.mdl
	models/weapons/w_smg1.dx90.vtx
	models/weapons/v_blazer.vvd
	models/weapons/v_blazer.dx90.vtx
	models/weapons/w_smg1.mdl
	models/weapons/w_smg1.jpg
	models/weapons/w_smg1.vvd
	models/weapons/w_smg1.phy
	models/weapons/w_smg1.dx80.vtx
	models/weapons/w_smg1.sw.vtx
	models/weapons/v_blazer.dx80.vtx
	models/weapons/v_smg1.jpg
	models/weapons/v_blazer.sw.vtx
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
	scripts/ChapterBackgrounds.txt
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