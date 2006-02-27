$modelname "hyperball/hyperTwoStack.mdl"

$cdmaterials models/hyperball/
$cdmaterials models/netting/
$scale 1
$surfaceprop "plastic"
$staticprop
$body "Body" "hyperTwoStack"
$sequence idle "hyperTwoStack_idle" loop fps 1
$collisionmodel "hyperTwoStack_phy.smd"
{
$mass 1
$concave
}
