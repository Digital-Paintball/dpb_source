$modelname tropicalprops/tropicpalm2.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 0.8
$body studio tropicpalm2.smd
$sequence idle tropicpalm2.smd
$collisionmodel tropicpalm2_phy.smd


$lod 50
{
      replacemodel "tropicpalm2" "tropicpalm2_lod1"
}

$lod 75
{
      replacemodel "tropicpalm2" "tropicpalm2_lod2"
}


$lod 125
{
      replacemodel "tropicpalm2" "tropicpalm2_lod3"
}