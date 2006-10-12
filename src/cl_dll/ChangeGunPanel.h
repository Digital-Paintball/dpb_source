#ifndef CHANGEGunPANEL_H
#define CHANGEGunPANEL_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_helpers.h"
#include <vgui_controls/Frame.h>
using namespace vgui;
class CChangeGunPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(CChangeGunPanel,Frame);
public:
	CChangeGunPanel( vgui::VPANEL parent );
	void OnCommand(const char *command);
	void CChangeGunPanel::ShowPanel(bool bShow);
};
DeclarePanel(CChangeGun,CChangeGunPanel,ChangeGun);
#endif // CHANGEGunPANEL_H 