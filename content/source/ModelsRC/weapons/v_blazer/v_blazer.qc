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
$origin 0 0 0


$sequence idle "Idle" fps 30 snap activity ACT_VM_IDLE 1 node Ready
$sequence draw "Draw" fps 30 snap activity ACT_VM_DRAW 1 node Ready
$sequence holster "Holster" fps 30 snap activity ACT_VM_HOLSTER 1 node Ready
$sequence fire "Fire" fps 63 snap activity ACT_VM_PRIMARYATTACK 1 node Ready //9BPS, 7 frames each