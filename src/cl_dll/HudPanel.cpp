#include "cbase.h"
#include "hudpanel.h"
#include "vgui/ISurface.h"
#include "vgui/IPanel.h"
#include "vgui/IVGui.h"
#include "IGameResources.h"
#include "cl_dll/iviewport.h"
#include <mapoverview.h>
#include "c_multiarena.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
PanelGlobals(CMyHud,CMyHudPanel,MyHud);
CON_COMMAND(ShowHud,NULL)
{
	ToggleVisibility(MyHud->GetPanel());
}
CMyHudPanel::CMyHudPanel( vgui::VPANEL parent ) : BaseClass( NULL, "MyHud" )
{	
	SetParent(parent);
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme");
	SetScheme( scheme );
	LoadControlSettings("Resource/UI/HudPanel.res");
	MoveToFront();
	m_nTextureID = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile( m_nTextureID, "vgui/gui/hud", true, false);
	ivgui()->AddTickSignal(this->GetVPanel(), 1000); // call our tick every second
    SetPos((ScreenWidth()-GetWide())/2,0); // center on screen

	rText = new wchar_t[10]; // jeff - moved these to the constructor to prevent thrashing
	bText = new wchar_t[10];
	tText = new wchar_t[40];

//	AlignPanel(this, ipanel()->GetPanel(parent, GetControlsModuleName()), 3);
//	IViewPortPanel* newpanel = NULL;
//	newpanel = new CMapOverview(  );
//	newpanel->ShowPanel(true);

// TODO I'll come back to the map overview later -Jeff

	SetVisible(false);
	SetEnabled(true);
} 
void CMyHudPanel::OnCommand(const char *command)
{
	BaseClass::OnCommand(command);
}

CMyHudPanel::~CMyHudPanel()
{
	delete[] rText; // jeff - moved these to the deconstructor to prevent thrashing
	delete[] bText;
	delete[] tText;
}

void CMyHudPanel::OnThink()
{
	BaseClass::OnThink();
	SetMouseInputEnabled(false);
}
void CMyHudPanel::Paint()
{
	BaseClass::Paint();  
	vgui::surface()->DrawSetTexture( m_nTextureID );
	vgui::surface()->DrawSetColor(50,50,50,150);
	vgui::surface()->DrawTexturedRect( 0, 0, 256, 30 );

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;
	C_Arena* pArena = pPlayer->GetArena();

	if (!pArena)
	{
		return;
	}

	swprintf( bText, L"%i", pArena->m_iRedTeamScore);
	swprintf( rText, L"%i", pArena->m_iBlueTeamScore);
	swprintf( tText, L"%i sec", pArena->m_iRoundTime);

	surface()->DrawSetTextFont(  vgui::scheme()->GetIScheme(GetScheme())->GetFont( "Default" ) );	
	surface()->DrawSetTextColor( 255, 255, 255, 255 ); 

	surface()->DrawSetTextPos( 36, 2 ); 
	surface()->DrawPrintText( rText, wcslen(rText) );

	surface()->DrawSetTextPos( 206, 2 ); 
	surface()->DrawPrintText( bText, wcslen(bText) );	
	
	surface()->DrawSetTextPos( 110, 6 ); 
	surface()->DrawPrintText( tText, wcslen(tText) );	
}

void CMyHudPanel::PaintBackground()
{
	BaseClass::PaintBackground();

}

void CMyHudPanel::OnTick()
{
	
	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();

	if (!pPlayer)
		return;

	C_Arena* pArena = pPlayer->GetArena();

	if (!pArena)
	{
		SetVisible(false);
		return;
	}
	
	SetVisible(true);
}
