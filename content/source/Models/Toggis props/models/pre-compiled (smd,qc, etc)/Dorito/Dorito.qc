$modelname "props\Doritos\Dorito.mdl"
$cdmaterials "models\Dorito\"
$surfaceprop "rubber"
$staticprop

$model "Body" "Dorito.smd"





$shadowlod
{
	replacemodel "Dorito.smd" "Dorito_phy.smd"
}

$sequence "Idle" "Dorito.smd"

$collisionmodel "Dorito_phy.smd" {
	$mass 1
}
