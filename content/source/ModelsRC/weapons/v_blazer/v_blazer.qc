$modelname "weapons/v_blazer.mdl"

//$cdmaterials "materials\models\weapons"
$cdmaterials "models\weapons"
//really can't quite figure this out yet, working on it.


// reference meshes
$body body "reference"


//$bodygroup barrel
//{
//studio "default"
//studio "stealth"
//studio "boomstick"
//}


$mostlyopaque
//$origin 7 0 4 11 0 0
$origin 0 0 2.5 0

$sequence draw "Draw" fps 30 snap activity ACT_VM_DRAW 1 node Ready
$sequence idle "Idle" fps 30 snap activity ACT_VM_IDLE 1 node Ready
$sequence holster "Holster" fps 30 snap activity ACT_VM_HOLSTER 1 node Ready
$sequence fire "Fire" fps 63 snap activity ACT_VM_PRIMARYATTACK 1 node Ready //9BPS, 7 frames each
$sequence reload1a "Reload1a" fps 30 snap activity ACT_VM_RELOAD1A 1 node Ready
$sequence reload1b "Reload1b" fps 30 snap activity ACT_VM_RELOAD1B 1 node Ready
$sequence reload2 "Reload2" fps 30 snap activity ACT_VM_RELOAD2 1 node Ready
$sequence reload3 "Reload3" fps 30 snap activity ACT_VM_RELOAD3 1 node Ready