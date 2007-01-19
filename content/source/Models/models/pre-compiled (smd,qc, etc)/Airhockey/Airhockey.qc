$modelname "props\Airhockey\Airhockey.mdl"
$cdmaterials "models\Airhockey\"
$surfaceprop "Wood"
$staticprop

$model "Body" "Airhockey.smd"





$shadowlod
{
	replacemodel "Airhockey.smd" "Airhockey_phy.smd"
}

$sequence "Idle" "Airhockey.smd"

$collisionmodel "Airhockey_phy.smd" {
	$mass 1
}
