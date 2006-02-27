$modelname "supair/Cylinder.mdl"


$cdmaterials models/supair/cylinder/
$scale 1
$surfaceprop "rubber"
$staticprop
$body "Body" "rygCylinder"
$texturegroup skinfamilies
{
	{ "cylinder1", "cylinder2", "cylinder3", "cylinder4"  }
	{ "cylinder1_yellow", "cylinder2_red", "cylinder3_orange", "cylinder4_yellow"  }
	{ "cylinder1_grey", "cylinder2_blue", "cylinder3_blue", "cylinder4_grey"  }
}
$lod 100
{
replacemodel "rygCylinder" "rygCylinder_LOD_100"
}
$sequence idle "rygCylinder_idle" loop fps 1
$collisionmodel "rygCylinder_phy.smd"
{
$mass 1
$concave
}
