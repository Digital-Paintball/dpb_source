#include "cbase.h"
#include "changeskinpanel.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
PanelGlobals(CChangeSkin,CChangeSkinPanel,ChangeSkin);
CON_COMMAND(ShowChangeSkin,NULL)
{
	ToggleVisibility(ChangeSkin->GetPanel());
}
CChangeSkinPanel::CChangeSkinPanel( vgui::VPANEL parent ) : BaseClass( NULL, "ChangeSkin" )
{
	SetParent(parent);
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme");
	SetScheme( scheme );
	LoadControlSettings("Resource/UI/ChangeSkinPanel.res");
	CenterThisPanelOnScreen();//keep in mind, hl2 supports widescreen 
	SetVisible(false);//made visible on command later 

	//Other useful options
	SetSizeable(false);
	//SetMoveable(false);
} 
void CChangeSkinPanel::OnCommand(const char *command)
{
	if (!stricmp(command, "Skin1"))
	{
		engine->ClientCmd("changeskin 0");
		engine->ClientCmd("ShowChangeSkin");
	}
	if (!stricmp(command, "Skin2"))
	{
		engine->ClientCmd("changeskin 1");
		engine->ClientCmd("ShowChangeSkin");
	}
	if (!stricmp(command, "Skin3"))
	{
		engine->ClientCmd("changeskin 2");
		engine->ClientCmd("ShowChangeSkin");
	}
	if (!stricmp(command, "Skin4"))
	{
		engine->ClientCmd("changeskin 3");
		engine->ClientCmd("ShowChangeSkin");
	}

	BaseClass::OnCommand(command);
}