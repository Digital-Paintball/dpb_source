$modelname "weapons/w_smg1.mdl"
$cdmaterials "models\weapons"
$surfaceprop "weapon"

$model "studio" "w_smg1.smd"

$collisionmodel "w_smg1.smd" {
	$mass 20.0
}


$attachment "muzzle" "ValveBiped.muzzle" -18 -.5 -.3 rotate -0 0 -90
$attachment "1" "ValveBiped.w_shotgun" 1 -7 3 rotate 0 0 -0


$sequence idle1 "w_smg1.smd" loop ACT_VM_IDLE 1


//SMG1 fire
$sequence attack1 "fire" ACT_RANGE_ATTACK_SMG1 1 fps 20.00 {
  { event AE_NPC_MUZZLEFLASH 0 "SMG1 MUZZLE" }
  { event 3002 0 "lefthand" }
  { event 6001 0 "0" }
}

//SMG1 granade
$sequence attackoffcenter "fire" fps 20.00 {
  { event AE_NPC_MUZZLEFLASH 0 "SMG1 MUZZLE" }
  { event 3002 0 "lefthand" }
  { event 6001 0 "0" }
}

//IRifle fire
$sequence fire "fire" ACT_RANGE_ATTACK_AR2 1 fps 30.00 {
  { event AE_NPC_MUZZLEFLASH 0 "SMG1 MUZZLE" }
  { event 3002 0 }
  { event 6001 0 "0" }
}