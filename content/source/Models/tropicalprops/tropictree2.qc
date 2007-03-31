$modelname tropicalprops/tropictree2.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 5
$body studio tropictree2.smd

$texturegroup skinfamilies
{
	{ "leaves1" }
	{ "leaves2" }
	{ "leaves3" }
	{ "leaves4" }
}

$sequence idle tropictree2.smd

$lod 125
{
      replacemodel "tropictree2" "tropictree2_LOD1.smd"
}