#include "cbase.h"
#include "arenapanel.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
PanelGlobals(CJoinArena,CJoinArenaPanel,JoinArena);

static ConVar friendly_ui("friendly_ui", "1", 0, "Enables or disables the friendly join arena UI", true, 0, true, 1);

CON_COMMAND(ShowJoinArena,NULL)
{
	if(friendly_ui.GetBool()) // jeff - don't show this panel if friendly_ui is 0.
		ToggleVisibility(JoinArena->GetPanel());
}
CJoinArenaPanel::CJoinArenaPanel( vgui::VPANEL parent ) : BaseClass( NULL, "JoinArena" )
{
	SetParent(parent);
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme");
	SetScheme( scheme );
	LoadControlSettings("Resource/UI/JoinArenaPanel.res");
	CenterThisPanelOnScreen();//keep in mind, hl2 supports widescreen 
	SetVisible(false);//made visible on command later 

	//Other useful options
	SetSizeable(false);
	//SetMoveable(false);
} 
void CJoinArenaPanel::OnCommand(const char *command)
{
	if (!stricmp(command, "Join"))
	{
		engine->ClientCmd("joingame");
		engine->ClientCmd("ShowJoinArena");
	}
		BaseClass::OnCommand(command);
}