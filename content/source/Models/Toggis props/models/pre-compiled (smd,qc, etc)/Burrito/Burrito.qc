$modelname "props\Burrito\Burrito.mdl"
$cdmaterials "models\Burrito\"
$surfaceprop "rubber"
$staticprop

$model "Body" "Burrito.smd"





$shadowlod
{
	replacemodel "Burrito.smd" "Burrito_phy.smd"
}

$sequence "Idle" "Burrito.smd"

$collisionmodel "Burrito_phy.smd" {
	$mass 1
}


