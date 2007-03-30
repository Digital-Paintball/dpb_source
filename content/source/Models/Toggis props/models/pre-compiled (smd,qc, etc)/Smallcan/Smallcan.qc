$modelname "props\Smallcan\Smallcan.mdl"
$cdmaterials "models\Smallcan\"
$surfaceprop "rubber"
$staticprop
$scale 0.3

$model "Body" "Smallcan.smd"





$shadowlod
{
	replacemodel "Smallcan.smd" "Smallcan_phy.smd"
}

$sequence "Idle" "Smallcan.smd"

$collisionmodel "Smallcan_phy.smd" {
	$mass 1
}
