#include "cbase.h"
#include "changeGunpanel.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
PanelGlobals(CChangeGun,CChangeGunPanel,ChangeGun);
CON_COMMAND(ShowChangeGun,NULL)
{
	ToggleVisibility(ChangeGun->GetPanel());
}
CChangeGunPanel::CChangeGunPanel( vgui::VPANEL parent ) : BaseClass( NULL, "ChangeGun" )
{
	SetParent(parent);
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme");
	SetScheme( scheme );

//	SetProportional(true); // scale with resolution (we hope) looks really bad
	LoadControlSettings("Resource/UI/ChangeGunPanel.res");
	CenterThisPanelOnScreen();
	SetVisible(false);//made visible on command later 

	//Other useful options
	SetSizeable(false);
	//SetMoveable(false);
} 
void CChangeGunPanel::OnCommand(const char *command)
{
		BaseClass::OnCommand(command);
}