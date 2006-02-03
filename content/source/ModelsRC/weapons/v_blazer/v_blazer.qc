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



$sequence draw "Draw" fps 30 snap activity ACT_VM_DRAW 1 node Ready
$sequence idle "Idle" fps 30 snap activity ACT_VM_IDLE 1 node Ready
