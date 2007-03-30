$modelname "props\Burrito\Burrito.mdl"
$cdmaterials "models\Burrito\"
$surfaceprop "rubber"
$staticprop
$scale 0.3


$model "Body" "Burrito.smd"





$shadowlod
{
	replacemodel "Burrito.smd" "Burrito_phy.smd"
}

$sequence "Idle" "Burrito.smd"

$collisionmodel "Burrito_phy.smd" {
	$mass 1
}


