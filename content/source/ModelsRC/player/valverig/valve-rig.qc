/*
==============================================================================

Empire exo player model

==============================================================================
*/

$modelname player/player.mdl
$cdmaterials models/player


$surfaceprop "metal"

//start eye/face data
$eyeposition 0 0 73
$bbox -13 -13 0 13 13 72

// head controllers
$attachment "chest" "ValveBiped.Bip01_Spine2" 5.00 4.00 0.00 rotate 0 90 90

$model "valve-rig" "valve-rig.smd"

$include "ruleshierarchy.qci"
$include "ct_attachments.qci"

$include "bones1.qci"

$includemodel player/cs_player_shared.mdl
$includemodel player/ak_anims_ct.mdl


$sequence dead "surrender" fps 30 activity ACT_DIE 1

// lean delta anims
$animation leanleft_layer "leanleft" FPS 30 subtract leanleft_layer 0
$sequence lean_left "leanleft_layer" delta activity ACT_LEAN_LEFT 1

$animation leanright_layer "leanright" FPS 30 subtract leanright_layer 0
$sequence lean_right "leanright_layer" delta activity ACT_LEAN_RIGHT 1


$include "ragdoll.qci"
$include "hitbox.qci"

$texturegroup skinfamilies
{
	{ "body.vtf", "face.vtf", "vforce_red.vtf", "invision_red.vtf", "chrome_reflect-b.vtf" }
	{ "body2_red.vtf", "face.vtf", "vforce_red.vtf", "invision_red.vtf", "chrome_reflect.vtf" }
	{ "body3_red.vtf", "face.vtf", "vforce_red.vtf", "invision_red.vtf", "chrome_reflect-b.vtf" }
	{ "body4_red.vtf", "face.vtf", "vforce_red.vtf", "invision_red.vtf", "chrome_reflect-b.vtf" }
	{ "body1_blue.vtf", "face_blue.vtf", "vforce_blue.vtf", "invision_blue.vtf", "chrome_reflect-b.vtf" }
	{ "body2_blue.vtf", "face_blue.vtf", "vforce_blue.vtf", "invision_blue.vtf", "chrome_reflect-b.vtf" }
	{ "body3_blue.vtf", "face_blue.vtf", "vforce_blue.vtf", "invision_blue.vtf", "chrome_reflect-b.vtf" }
	{ "body4_blue.vtf", "face_blue.vtf", "vforce_blue.vtf", "invision_blue.vtf", "chrome_reflect-y.vtf" }
}
