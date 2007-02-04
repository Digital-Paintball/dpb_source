$modelname "props\JerseyHanger\JerseyHanger.mdl"
$cdmaterials "models\JerseyHanger\"
$surfaceprop "Wood"
$staticprop

$model "Body" "JerseyHanger.smd"





$shadowlod
{
	replacemodel "JerseyHanger.smd" "JerseyHanger_phy.smd"
}

$sequence "Idle" "JerseyHanger.smd"

$collisionmodel "JerseyHanger_phy.smd" {
	$mass 1
}
