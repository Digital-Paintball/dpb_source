//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef VGUI_HELPERS_H
#define VGUI_HELPERS_H
#ifdef _WIN32
#pragma once
#endif


#include <vgui_controls/panel.h>
#include <vgui_controls/label.h>

class IGameUI
// added by Jeff 10/6 vgui stuff
{
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;
	virtual vgui::Panel *GetPanel(void) = 0;
};
#define DeclarePanel(className,panelClassName,globalPanel)\
	class className : public IGameUI\
	{\
	private:\
		panelClassName *myPanel;\
	public:\
		className(void)\
		{\
			myPanel = NULL;\
		}\
		void Create( vgui::VPANEL parent )\
		{\
			myPanel = new panelClassName( parent );\
		}\
		void Destroy( void )\
		{\
			if(myPanel)\
			{\
				myPanel->SetParent( (vgui::Panel *)NULL );\
				delete myPanel;\
			}\
		}\
		vgui::Panel *GetPanel(void)\
		{\
			return myPanel;\
		}\
	};\
	extern IGameUI *globalPanel
//jeff 
#define PanelGlobals(className,panelClassName,globalPanel)\
	static className g_##className##Panel;\
	IGameUI *globalPanel = (IGameUI *)&g_##className##Panel

#define ToggleVisibility(panel)\
	panel->SetVisible(!panel->IsVisible())

//jeff
#define CenterThisPanelOnScreen()\
	int x,w,h;\
	GetBounds(x,x,w,h);\
	SetPos((ScreenWidth()-w)/2,(ScreenHeight()-h)/2)
#define CenterPanelOnScreen(panel)\
	int x,w,h;\
	panel->GetBounds(x,x,w,h);\
	panel->SetPos((panel->ScreenWidth()-w)/2,(panel->ScreenHeight()-h)/2)

inline int PanelTop(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->GetBounds(x,y,w,h); return y;}
inline int PanelLeft(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->GetBounds(x,y,w,h); return x;}
inline int PanelRight(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->GetBounds(x,y,w,h); return x+w;}
inline int PanelBottom(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->GetBounds(x,y,w,h); return y+h;}
inline int PanelWidth(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->GetBounds(x,y,w,h); return w;}
inline int PanelHeight(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->GetBounds(x,y,w,h); return h;}

// Places child at the requested position inside pParent. iAlignment is from Label::Alignment.
void AlignPanel(vgui::Panel *pChild, vgui::Panel *pParent, int alignment);


#endif // VGUI_HELPERS_H

