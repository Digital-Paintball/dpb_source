$modelname "supair/HalfCyl.mdl"

$cdmaterials models/supair/halfcyl
$scale 1
$surfaceprop "rubber"
$staticprop
$body "Body" "rygHalfCyl"

$texturegroup skinfamilies
{
	{ "halfcyl1", "halfcyl2"}
	{ "halfcyl1_orange", "halfcyl2_orange"}
	{ "halfcyl1_blue", "halfcyl2_blue"}
}

$lod 100
{
replacemodel "rygHalfCyl" "rygHalfCyl_LOD_100"
}
$sequence idle "rygHalfCyl_idle" loop fps 1
$collisionmodel "rygHalfCyl_phy.smd"
{
$mass 1
$concave
}
