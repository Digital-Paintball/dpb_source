$modelname tropicalprops/tropicpalm1.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 0.8
$body studio tropicpalm1.smd
$sequence idle tropicpalm1.smd
$collisionmodel tropicpalm1_phy.smd


$lod 50
{
      replacemodel "tropicpalm1" "tropicpalm1_lod1"
}

$lod 75
{
      replacemodel "tropicpalm1" "tropicpalm1_lod2"
}


$lod 125
{
      replacemodel "tropicpalm1" "tropicpalm1_lod3"
}