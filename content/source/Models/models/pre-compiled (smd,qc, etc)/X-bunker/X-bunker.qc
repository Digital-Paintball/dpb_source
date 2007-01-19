$modelname "props\X-bunkers\X-bunker.mdl"
$cdmaterials "models\X-bunker\"
$surfaceprop "rubber"
$staticprop

$model "Body" "X-bunker.smd"





$shadowlod
{
	replacemodel "X-bunker.smd" "X-bunker_phy.smd"
}

$sequence "Idle" "X-bunker.smd"

$collisionmodel "X-bunker_phy.smd" {
	$mass 1
}
