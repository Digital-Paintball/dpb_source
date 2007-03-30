$modelname "props\blimps\blimp.mdl"
$cdmaterials "models\Blimp\"
$surfaceprop "rubber"
$staticprop
$scale 0.0625

$model "Body" "blimp.smd"





$shadowlod
{
	replacemodel "blimp.smd" "blimp_phy.smd"
}

$sequence "Idle" "blimp.smd"

$collisionmodel "blimp_phy.smd" {
	$mass 1
}
