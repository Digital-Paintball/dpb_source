$modelname "props\Fire_thingy\Fire_thingy.mdl"
$cdmaterials "models\Fire_thingy\"
$surfaceprop "solidmetal"
$staticprop
$scale 0.2

$model "Body" "Fire_thingy.smd"





$shadowlod
{
	replacemodel "Fire_thingy.smd" "Fire_thingy_phy.smd"
}

$sequence "Idle" "Fire_thingy.smd"

$collisionmodel "Fire_thingy_phy.smd" {
	$mass 1
}
