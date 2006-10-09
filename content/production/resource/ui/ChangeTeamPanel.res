"Resource/UI/ChangeTeamPanel.res"
{
	"ChangeTeam"
	{
		"ControlName"	"CChangeTeamPanel"
		"fieldName"	"ChangeTeam"
		"title"		"Change Team"
		"xpos"		"0"
		"ypos"		"0"
		"wide"		"300"
		"tall"		"200"
		"autoResize"	"1"
		"pinCorner"	"0"
		"visible"	"1"
		"enabled"	"1"
		"tabPosition"	"0"
	}
	"Label1"
	{
		"ControlName"	"Label"
		"fieldName"	"Label1"
		"xpos"		"16"
		"ypos"		"30"
		"wide"		"260"
		"tall"		"50"
		"wrap"		"1"
		"autoResize"	"0"
		"pinCorner"	"0"
		"visible"	"1"
		"enabled"	"1"
		"tabPosition"	"0"
		"labelText"	"Please choose what team you'd like to join:"
		"textAlignment"	"north"
		"dulltext"	"0"
		"brighttext"	"0"
	}
	"JoinRed"
	{
		"ControlName"	"Button"
		"fieldName"	"join"
		"xpos"		"30"
		"ypos"		"80"
		"wide"		"70"
		"tall"		"62"
		"labelText"	"Red Team"
		"Command"	"SwitchRed"
		"textAlignment"	"west"
		"autoResize"	"0"
		"pinCorner"	"0"
		"visible"	"1"
		"enabled"	"1"
		"tabPosition"	"0"
	}
	"JoinBlue"
	{
		"ControlName"	"Button"
		"fieldName"	"join"
		"xpos"		"205"
		"ypos"		"80"
		"wide"		"70"
		"tall"		"62"
		"labelText"	"Blue Team"
		"Command"	"SwitchBlue"
		"textAlignment"	"west"
		"autoResize"	"0"
		"pinCorner"	"0"
		"visible"	"1"
		"enabled"	"1"
		"tabPosition"	"0"
	}
	"Decline"
	{
		"ControlName"	"Button"
		"fieldName"	"close"
		"xpos"		"190"
		"ypos"		"170"
		"wide"		"100"
		"tall"		"25"
		"labelText"	"Cancel    "
		"Command"	"Close"
		"textAlignment"	"east"
		"autoResize"	"0"
		"pinCorner"	"0"
		"visible"	"1"
		"enabled"	"1"
		"tabPosition"	"0"
	}
}