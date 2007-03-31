$modelname tropicalprops/tropicbanana1.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 1
$body studio tropicbanana1.smd
$sequence idle tropicbanana1.smd
$collisionmodel tropicbanana1.smd


$lod 50
{
      replacemodel "tropicbanana1" "tropicbanana1_LOD1.smd"
}

$lod 75
{
      replacemodel "tropicbanana1" "tropicbanana1_LOD2.smd"
}


$lod 125
{
      replacemodel "tropicbanana1" "tropicbanana1_LOD3.smd"
}