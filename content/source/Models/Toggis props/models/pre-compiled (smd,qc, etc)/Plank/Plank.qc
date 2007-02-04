$modelname "props\Plank\Plank.mdl"
$cdmaterials "models\Plank\"
$surfaceprop "Wood"
$staticprop

$model "Body" "Plank.smd"





$shadowlod
{
	replacemodel "Plank.smd" "Plank_phy.smd"
}

$sequence "Idle" "Plank.smd"

$collisionmodel "Plank_phy.smd" {
	$mass 1
}
