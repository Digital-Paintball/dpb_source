#ifndef ARENAPANEL_H
#define ARENAPANEL_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_helpers.h"
#include <vgui_controls/Frame.h>
using namespace vgui;
class CJoinArenaPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(CJoinArenaPanel,Frame);
public:
	CJoinArenaPanel( vgui::VPANEL parent );
	void OnCommand(const char *command);
	void CJoinArenaPanel::ShowPanel(bool bShow);
};
DeclarePanel(CJoinArena,CJoinArenaPanel,JoinArena);
#endif // ARENAPANEL_H 