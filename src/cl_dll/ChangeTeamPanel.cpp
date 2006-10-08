#include "cbase.h"
#include "changeteampanel.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
PanelGlobals(CChangeTeam,CChangeTeamPanel,ChangeTeam);
CON_COMMAND(ShowChangeTeam,NULL)
{
	ToggleVisibility(ChangeTeam->GetPanel());
}
CChangeTeamPanel::CChangeTeamPanel( vgui::VPANEL parent ) : BaseClass( NULL, "ChangeTeam" )
{
	SetParent(parent);
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme");
	SetScheme( scheme );

//	SetProportional(true); // scale with resolution (we hope) looks really bad
	LoadControlSettings("Resource/UI/ChangeTeamPanel.res");
	CenterThisPanelOnScreen();
	SetVisible(false);//made visible on command later 

	//Other useful options
	SetSizeable(false);
	//SetMoveable(false);
} 
void CChangeTeamPanel::OnCommand(const char *command)
{
	if (!stricmp(command, "SwitchRed"))
	{
		engine->ClientCmd("changeteam 1");
		engine->ClientCmd("ShowChangeTeam");
	}

	if (!stricmp(command, "SwitchBlue"))
	{
		engine->ClientCmd("changeteam 0");
		engine->ClientCmd("ShowChangeTeam");
	}

	if (!stricmp(command, "SwitchSpec"))
	{
//		engine->ClientCmd("joingame");
		engine->ClientCmd("ShowChangeTeam");
	}

		BaseClass::OnCommand(command);
}