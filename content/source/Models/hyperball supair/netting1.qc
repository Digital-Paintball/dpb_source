$modelname "netting/netting1.mdl"

$cdmaterials models/netting/
$scale 1
$surfaceprop "default"
$staticprop
$body "Body" "netting1"
$mostlyopaque
$lod 100
{
replacemodel "netting1" "netting1_LOD_10"
}
$sequence idle "netting1_idle" loop fps 1
$collisionmodel "netting1_phy.smd"
{
$mass 1
$concave
}
