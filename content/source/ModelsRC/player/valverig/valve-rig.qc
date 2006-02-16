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

$include "ragdoll.qci"
$include "hitbox.qci"

