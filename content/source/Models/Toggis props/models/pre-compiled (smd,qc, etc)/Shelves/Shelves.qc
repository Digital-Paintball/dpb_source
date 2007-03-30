$modelname "props\Shelves\Shelves.mdl"
$cdmaterials "models\Shelves\"
$surfaceprop "Wood"
$staticprop
$scale 0.2

$model "Body" "Shelves.smd"





$shadowlod
{
	replacemodel "Shelves.smd" "Shelves_phy.smd"
}

$sequence "Idle" "Shelves.smd"

$collisionmodel "Shelves_phy.smd" {
	$mass 1
}

