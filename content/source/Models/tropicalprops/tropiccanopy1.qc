$modelname tropicalprops/tropiccanopy1.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 1
$body studio tropiccanopy1.smd
$sequence idle tropiccanopy1.smd
$collisionmodel tropiccanopy1.smd


$lod 50
{
      replacemodel "tropiccanopy1" "tropiccanopy1_LOD1.smd"
}

$lod 75
{
      replacemodel "tropiccanopy1" "tropiccanopy1_LOD2.smd"
}


$lod 125
{
      replacemodel "tropiccanopy1" "tropiccanopy1_LOD3.smd"
}