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

//	AlignPanel(this, ipanel()->GetPanel(parent, GetControlsModuleName()), 3);
//	IViewPortPanel* newpanel = NULL;
//	newpanel = new CMapOverview(  );
//	newpanel->ShowPanel(true);

// I'll come back to the map overview later -Jeff

	SetVisible(false);
	SetEnabled(true);
} 
void CMyHudPanel::OnCommand(const char *command)
{
	BaseClass::OnCommand(command);
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
	vgui::surface()->DrawSetColor(50,50,50,100);
//	vgui::surface()->DrawFilledRect(0,0,256,30);
	vgui::surface()->DrawTexturedRect( 0, 0, 256, 30 );

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;
	C_Arena* pArena = pPlayer->GetArena();

	if (!pArena)
	{
		return;
	}
	
	int red = pArena->m_iRedTeamScore;
	int blue = pArena->m_iBlueTeamScore;

	wchar_t * rText = new wchar_t[10];
	wchar_t * bText = new wchar_t[10];
	wchar_t * tText = new wchar_t[40];

	swprintf( rText, L"%i", red);
	swprintf( bText, L"%i", blue);
	swprintf( tText, L"%i sec", pArena->m_iRoundTime);

	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	vgui::HFont hFont = pScheme->GetFont( "DefaultSmall" );
	surface()->DrawSetTextFont( hFont );	
	surface()->DrawSetTextColor( 255, 255, 255, 255 ); 

	surface()->DrawSetTextPos( 24, 4 ); 
	surface()->DrawPrintText( rText, wcslen(rText) );

	surface()->DrawSetTextPos( 192, 4 ); 
	surface()->DrawPrintText( bText, wcslen(bText) );	
	
	surface()->DrawSetTextPos( 96, 13 ); 
	surface()->DrawPrintText( tText, wcslen(tText) );	

	delete[] rText;
	delete[] bText;
	delete[] tText;
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

	/*
	IGameResources *gr = GameResources();
	if (!gr)
		return;
	int red = gr->GetTeamScore(0);
	int blue = gr->GetTeamScore(1);

	wchar_t * rText = new wchar_t[10];
	wchar_t * bText = new wchar_t[10];
	const wchar_t * fomat = L"%i";

	swprintf( rText, fomat, red);
	swprintf( bText, fomat, blue);

	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	vgui::HFont hFont = pScheme->GetFont( "DefaultSmall" );
	surface()->DrawSetTextFont( hFont );	
	surface()->DrawSetTextColor( 255, 255, 255, 255 ); 

	surface()->DrawSetTextPos( 24, 4 ); 
	surface()->DrawPrintText( rText, wcslen(rText) );

	surface()->DrawSetTextPos( 192, 4 ); 
	surface()->DrawPrintText( bText, wcslen(bText) );	

	delete[] rText;
	delete[] bText;
	*/
}
