$modelname "hyperball/hypertube.mdl"

$cdmaterials models/hyperball/
$scale 1
$surfaceprop "plastic"
$staticprop
$body "Body" "hypertube"
$sequence idle "hypertube_idle" loop fps 1
$collisionmodel "hypertube_phy.smd"
{
$mass 1
$concave
}
