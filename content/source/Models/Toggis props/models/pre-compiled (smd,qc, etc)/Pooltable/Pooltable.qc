$modelname "props\Pooltable\Pooltable.mdl"
$cdmaterials "models\Pooltable\"
$surfaceprop "Wood"
$staticprop
$scale 0.22

$model "Body" "Pooltable.smd"





$shadowlod
{
	replacemodel "Pooltable.smd" "Pooltable_phy.smd"
}

$sequence "Idle" "Pooltable.smd"

$collisionmodel "Pooltable_phy.smd" {
	$mass 1
}
