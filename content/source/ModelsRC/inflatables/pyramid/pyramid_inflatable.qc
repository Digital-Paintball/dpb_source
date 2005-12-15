$modelname "inflatables\pyramid.mdl"
$cdmaterials "models\inflatables\"
$surfaceprop "rubber"
$staticprop

$model "Body" "pyramid_inflatable.smd"

$lod 50 {
	replacemodel "pyramid_inflatable.smd" "lod1_pyramid_inflatable.smd"
}

$shadowlod
{
	replacemodel "pyramid_inflatable.smd" "phymodel.smd"
}

$sequence "Idle" "pyramid_inflatable.smd"

$collisionmodel "phymodel.smd" {
	$mass 1
}
