//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef VGUI_RENDERIMAGEPANEL_H
#define VGUI_RENDERIMAGEPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Panel.h>

//-----------------------------------------------------------------------------
// Purpose: Panel that renders a model
//-----------------------------------------------------------------------------
class CRenderImagePanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CRenderImagePanel, vgui::Panel );
public:
	CRenderImagePanel(vgui::Panel *parent, const char *name);
	~CRenderImagePanel();

	virtual void SetModel(const char *modelpath);

	// set the color to fill with, if no image is specified
	void SetFillColor( Color col );
	Color GetFillColor();

protected:
	virtual void PaintBackground();
	virtual void GetSettings(KeyValues *outResourceData);
	virtual void ApplySettings(KeyValues *inResourceData);
	virtual const char *GetDescription();
	//virtual void OnSizeChanged(int newWide, int newTall);
	//virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	char *m_pImage;
	char *m_pszImageName;
	char *m_pszColorName;
	bool m_bScaleImage;
	Color m_FillColor;
};

#endif // VGUI_RENDERIMAGEPANEL_H
