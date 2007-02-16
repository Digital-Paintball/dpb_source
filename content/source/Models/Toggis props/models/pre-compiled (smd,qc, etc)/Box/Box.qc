$modelname "props\Box\Box.mdl"
$cdmaterials "models\Box\"
$surfaceprop "rubber"
$staticprop

$model "Body" "Box.smd"





$shadowlod
{
	replacemodel "Box.smd" "Box_phy.smd"
}

$sequence "Idle" "Box.smd"

$collisionmodel "Box_phy.smd" {
	$mass 1
}

