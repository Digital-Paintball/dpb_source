$modelname "props\Gmachine1\Gmachine1.mdl"
$cdmaterials "models\Gmachine1\"
$surfaceprop "metal"
$staticprop

$model "Body" "Gmachine1.smd"





$shadowlod
{
	replacemodel "Gmachine1.smd" "Gmachine1_phy.smd"
}

$sequence "Idle" "Gmachine1.smd"

$collisionmodel "Gmachine1_phy.smd" {
	$mass 1
}
