$modelname "supair/Pyramid.mdl"

$cdmaterials models/supair/pyramid
$scale 1
$surfaceprop "rubber"
$staticprop
$body "Body" "rygPyramid"
$texturegroup skinfamilies
{
	{ "pyramid1", "pyramid2", "pyramid3" }
	{ "pyramid1_orange", "pyramid2_orange", "pyramid3_yellow" }
	{ "pyramid1_blue", "pyramid2_blue", "pyramid3_grey" }
}


$lod 100
{
replacemodel "rygPyramid" "rygPyramid_LOD_100"
}
$sequence idle "rygPyramid_idle" loop fps 1
$collisionmodel "rygPyramid_phy.smd"
{
$mass 1
$concave
}
