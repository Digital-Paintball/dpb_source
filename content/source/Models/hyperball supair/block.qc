$modelname "supair/block.mdl"

$cdmaterials models/supair/block/
$scale 1
$surfaceprop "rubber"
$staticprop
$body "Body" "block"
$texturegroup skinfamilies
{
	{ "block1", "block2", "block3" }
	{ "block1_red", "block2_orange", "block3_yellow" }
	{ "block1_blue", "block2_blue", "block3_grey" }

}
$sequence idle "block_idle" loop fps 1
$collisionmodel "block_phy.smd"
{
$mass 1
$concave
}
