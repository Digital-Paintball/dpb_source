$modelname "props\Semitaco\Semitaco.mdl"
$cdmaterials "models\Semitaco\"
$surfaceprop "rubber"
$staticprop

$model "Body" "Semitaco.smd"





$shadowlod
{
	replacemodel "Semitaco.smd" "Semitaco_phy.smd"
}

$sequence "Idle" "Semitaco.smd"

$collisionmodel "Semitaco_phy.smd" {
	$mass 1
}
