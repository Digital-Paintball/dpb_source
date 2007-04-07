#ifndef CHANGETEAMPANEL_H
#define CHANGETEAMPANEL_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_helpers.h"
#include <vgui_controls/Frame.h>
using namespace vgui;
class CChangeTeamPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(CChangeTeamPanel,Frame);
public:
	CChangeTeamPanel( vgui::VPANEL parent );
	void OnCommand(const char *command);
	void CChangeTeamPanel::ShowPanel(bool bShow);
};
DeclarePanel(CChangeTeam,CChangeTeamPanel,ChangeTeam);
#endif // CHANGETEAMPANEL_H 