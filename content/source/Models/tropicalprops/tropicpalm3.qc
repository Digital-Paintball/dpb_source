$modelname tropicalprops/tropicpalm3.mdl
$cdmaterials models/tropicalprops
$staticprop
$surfaceprop wood
$scale 0.8
$body studio tropicpalm3.smd
$sequence idle tropicpalm3.smd
$collisionmodel tropicpalm3_phy.smd


$lod 50
{
      replacemodel "tropicpalm3" "tropicpalm3_lod1"
}

$lod 75
{
      replacemodel "tropicpalm3" "tropicpalm3_lod2"
}


$lod 125
{
      replacemodel "tropicpalm3" "tropicpalm3_lod3"
}