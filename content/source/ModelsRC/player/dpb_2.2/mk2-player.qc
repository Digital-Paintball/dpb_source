$modelname "player\player.mdl"
$cdmaterials models\player
    
$body Player "body"  

$bodygroup Mask
{
studio "mask"
studio "vforce_mask"
studio "invisiongoggles"
studio "proto"
}

//////////////////////
//hl2 stuff
//////////////////////

$eyeposition 0 0 70
$attachment "chest" "chest" 5.00 4.00 0.00 rotate 0 90 90
$attachment "eyes" "head" -0.0454 -3.2682 67.3843 absolute
$include "standardhierarchy.qci"
$include "standardikchains.qci"
$bbox -13 -13 0  13 13 72
$include "commonbones.qci"

//$surfaceprop "slime"
//$surfaceprop "watermelon"
$surfaceprop "alienflesh"
$popd

//max world bone
$alwayscollapse "Particle View 01"
$alwayscollapse "Particle View 01"
$alwayscollapse "smdimport"
//fragmotion world bone
$alwayscollapse "root"

//////////////////////
//hl2
//////////////////////

$bodygroup Hooper
{
blank
studio "hooper_ref"
}

//$origin 0 0 36

// 2 skin families
$texturegroup skinfamilies
{
{ "body.bmp" "face.bmp" "vforce_red.bmp" "invision_red.bmp" "proto_red.bmp" }
{ "body1_blue.bmp" "face_blue.bmp" "vforce_blue.bmp" "invision_blue.bmp" "proto_blue.bmp" }
{ "body2_red.bmp" "face.bmp" "vforce_red.bmp" "invision_red.bmp" "proto_red.bmp" }
{ "body2_blue.bmp" "face_blue.bmp" "vforce_blue.bmp" "invision_blue.bmp" "proto_blue.bmp" }
{ "body3_red.bmp" "face.bmp" "vforce_red.bmp" "invision_red.bmp" "proto_red.bmp" }
{ "body3_blue.bmp" "face_blue.bmp" "vforce_blue.bmp" "invision_blue.bmp" "proto_blue.bmp" }
{ "body4_red.bmp" "face.bmp" "vforce_red.bmp" "invision_red.bmp" "proto_red.bmp" }
{ "body4_blue.bmp" "face_blue.bmp" "vforce_blue.bmp" "invision_blue.bmp" "proto_blue.bmp" }
}

//	  [Name]		[File]			[FPS]		[Lock]	[Origen]	[ID]
$sequence "stand_idle"		"stand_idle"		fps   7	loop
$sequence "stand_aim"		"stand_aim"		fps  14	loop
$sequence "stand_aim-right"	"stand_aim-right"	fps  50
$sequence "stand_right"		"stand_right"			loop
$sequence "stand_right-aim"	"stand_right-aim"	fps  50
$sequence "stand_aim-left"	"stand_aim-left"	fps  50
$sequence "stand_left"		"stand_left"			loop
$sequence "stand_left-aim"	"stand_left-aim"	fps  50
$sequence "stand_aim-reload"	"stand_aim-reload"	fps  20
$sequence "stand_reload"	"stand_reload"			loop
$sequence "stand_reload-aim"	"stand_reload-aim"	fps  20
$sequence "stand_walk"		"stand_walk"		fps  22	loop	LX			ACT_WALK 1
$sequence "stand_run"		"stand_run"		fps  40	loop	LX 
$sequence "stand_aim-out"	"stand_aim-out"		fps  11
$sequence "stand_out"		"stand_out"		fps  13
$sequence "stand_jump"		"stand_jump"		fps  38
$sequence "stand_aim-duck"	"stand_aim-duck"	fps  50		LX LY
$sequence "stand_aim-prone"	"stand_aim-prone"	fps  20		LX LY	origin 0 0 4

$sequence "duck_aim"		"duck_aim"		fps   5	loop		origin 0 0 18
$sequence "duck_aim-right"	"duck_aim-right"	fps  24			origin 0 0 18
$sequence "duck_right"		"duck_right"			loop		origin 0 0 18
$sequence "duck_right-aim"	"duck_right-aim"	fps  24			origin 0 0 18
$sequence "duck_aim-left"	"duck_aim-left"		fps  72			origin 0 0 18
$sequence "duck_left"		"duck_left"			loop		origin 0 0 18
$sequence "duck_left-aim"	"duck_left-aim"		fps  72			origin 0 0 18
$sequence "duck_aim-reload"	"duck_aim-reload"	fps  20			origin 0 0 18
$sequence "duck_reload"		"duck_reload"			loop		origin 0 0 18
$sequence "duck_reload-aim"	"duck_reload-aim"	fps  20			origin 0 0 18
$sequence "duck_crawl"		"duck_crawl"		fps  17	loop	LX	origin 0 0 18
$sequence "duck_aim-prone"	"duck_aim-prone"	fps 120		LX LY	origin 0 4 6
$sequence "duck_aim-stand"	"duck_aim-stand"	fps  50		LX LY
$sequence "duck_aim-stand_out"	"duck_aim-stand_out"	fps  16		LX LY	origin 0 0 18

$sequence "prone_aim"		"prone_aim"		fps  14	loop		origin 0 2 17.5
$sequence "prone_aim-right"	"prone_aim-right"	fps  48			origin 0 2 4
$sequence "prone_right"		"prone_right"			loop		origin 0 2 4
$sequence "prone_right-aim"	"prone_right-aim"	fps  40			origin 0 2 4
$sequence "prone_aim-left"	"prone_aim-left"	fps  40			origin 0 2 4
$sequence "prone_left"		"prone_left"			loop		origin 0 2 4
$sequence "prone_left-aim"	"prone_left-aim"	fps  30			origin 0 2 4
$sequence "prone_aim-reload"	"prone_aim-reload"	fps  40			origin 0 2 4
$sequence "prone_reload"	"prone_reload"			loop		origin 0 2 17.75
$sequence "prone_reload-aim"	"prone_reload-aim"	fps  21			origin 0 2 17.75
$sequence "prone_crawl"		"prone_crawl"		fps  12	loop	LX LY	origin 0 2 4
$sequence "prone_aim-stand-aim"	"prone_aim-stand"	fps  50			origin 0 2 0
$sequence "prone_aim-stand_out"	"prone_aim-stand_out"	fps  12		LX LY	origin 0 2 0
$sequence "prone_aim-duck-aim"	"prone_aim-duck"	fps  75		LX LY	origin 0 -2 14

$sequence "stand_sprint"	"stand_sprint"	fps  11 loop

// 25 hit boxes
// group "name"			bottom back right  top      left front
$hbox 0 "spine1"               -6.3  -7.5  -7.43   9.57     6    7.43
$hbox 0 "chest"                 0    -4    -6.5   14        6.5  7.11
$hbox 0 "head"                 -2.77 -3.5  -3.09   6.5      5.4  3.28
$hbox 0 "right_upper_arm"       0    -2.48 -2.16  12.85     2.94 2
$hbox 0 "right_lower_arm"       0    -2.13 -2.01  10.03     2.28 2.19
$hbox 0 "right_hand"            0    -2.4  -1.5   10.25     7.5 1.5
$hbox 0 "right_hand"           10.25 3  0.5   20.25     4  -0.5
$hbox 0 "left_upper_arm"        0    -2.31 -2.47  12.8      1.9  2.94
$hbox 0 "left_lower_arm"        0    -2.04 -2.15   9.99     2.31 2.26
$hbox 0 "left_hand"             0    -1.24 -2.69   4.34     0.93 2.31
$hbox 0 "left_fingers1"         0    -1.04 -0.52   2.51     1.19 0.5
$hbox 0 "left_fingers2"         0    -1.04 -0.52   2.51     1.19 0.5
$hbox 0 "right_fingers1"        0    -1.04 -0.52   2.51     1.19 0.5
$hbox 0 "right_fingers2"        0    -1.04 -0.52   2.51     1.19 0.5
$hbox 0 "left_thom"             0    -0.4  -0.5    2.14     0.54 0.8
$hbox 0 "right_thom"            0    -0.4  -0.5    2.14     0.54 0.8
$hbox 0 "left_index1"           0    -0.49 -0.53   2.14     0.58 0.84
$hbox 0 "right_index1"          0    -0.49 -0.53   2.14     0.58 0.84
$hbox 0 "left_index2"           0    -0.49 -0.53   2.14     0.58 0.84
$hbox 0 "right_index2"          0    -0.49 -0.53   2.14     0.58 0.84
$hbox 0 "left_thigh"            5    -4.16 -3.8   19.446508 3.61 5
$hbox 0 "left_shin"            -1    -3.23 -3.5   14.75     2.6  3.69
$hbox 0 "right_thigh"           4    -3.72 -3.8   19.341131 4.49 5
$hbox 0 "right_shin"           -1    -2.92 -3.5   14.93     2.6  3.72
$hbox 0 "right_foot"           -4.5  -0.57 -1.93   7.47     3    2.7
$hbox 0 "left_foot"            -4.5  -0.53 -2.56   7.49     3    2.07
