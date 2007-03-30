$modelname "props\Doritos\Dorito.mdl"
$cdmaterials "models\Dorito\"
$surfaceprop "rubber"
$staticprop
$scale 0.35

$model "Body" "Dorito.smd"





$shadowlod
{
	replacemodel "Dorito.smd" "Dorito_phy.smd"
}

$sequence "Idle" "Dorito.smd"

$collisionmodel "Dorito_phy.smd" {
	$mass 1
}
