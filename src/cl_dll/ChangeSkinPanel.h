#ifndef CHANGESKINPANEL_H
#define CHANGESKINPANEL_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_helpers.h"
#include <vgui_controls/Frame.h>
using namespace vgui;
class CChangeSkinPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(CChangeSkinPanel,Frame);
public:
	CChangeSkinPanel( vgui::VPANEL parent );
	void OnCommand(const char *command);
	void CChangeSkinPanel::ShowPanel(bool bShow);
};
DeclarePanel(CChangeSkin,CChangeSkinPanel,ChangeSkin);
#endif // ChangeSkinPANEL_H 