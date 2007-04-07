#ifndef HUDPANEL_H
#define HUDPANEL_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_helpers.h"
#include <vgui_controls/Frame.h>
#include <vgui_controls/EditablePanel.h>
using namespace vgui;
class CMyHudPanel : public EditablePanel
{
	DECLARE_CLASS_SIMPLE(CMyHudPanel,EditablePanel);
public:
	CMyHudPanel( vgui::VPANEL parent );
	~CMyHudPanel();
	void OnCommand(const char *command);
	void CMyHudPanel::ShowPanel(bool bShow);
	void CMyHudPanel::OnThink();
	void CMyHudPanel::Paint();
	void CMyHudPanel::PaintBackground();
	void CMyHudPanel::OnTick();
	
	wchar_t * rText;
	wchar_t * bText;
	wchar_t * tText;
private:
	int m_nTextureID;
};
DeclarePanel(CMyHud,CMyHudPanel,MyHud);
#endif // HUDPANEL_H 