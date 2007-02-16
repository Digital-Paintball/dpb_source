$modelname "props\Gmachine2\Gmachine2.mdl"
$cdmaterials "models\Gmachine2\"
$surfaceprop "Wood"
$staticprop

$model "Body" "Gmachine2.smd"





$shadowlod
{
	replacemodel "Gmachine2.smd" "Gmachine2_phy.smd"
}

$sequence "Idle" "Gmachine2.smd"

$collisionmodel "Gmachine2_phy.smd" {
	$mass 1
}
