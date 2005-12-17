$modelname "inflatables\dorito01.mdl"
$cdmaterials "models\inflatables\"
$surfaceprop "rubber"
$staticprop

$model "Body" "dorito01.smd"

$lod 50 {
	replacemodel "dorito01.smd" "dorito01_lod1.smd"
}

$shadowlod
{
	replacemodel "dorito01.smd" "dorito01_phy.smd"
}

$sequence "Idle" "dorito01.smd"

$collisionmodel "dorito01_phy.smd" {
	$mass 1
}
