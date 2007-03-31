$modelname tropicalprops/tropictree1.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 1
$body studio tropictree1.smd
$sequence idle tropictree1.smd
$collisionmodel tropictree1.smd


$lod 50
{
      replacemodel "tropictree1" "tropictree1_LOD1.smd"
}

$lod 75
{
      replacemodel "tropictree1" "tropictree1_LOD2.smd"
}


$lod 125
{
      replacemodel "tropictree1" "tropictree1_LOD3.smd"
}