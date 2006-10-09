//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef RENDERIMAGEPANEL_H
#define RENDERIMAGEPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/VGUI.h>
#include <vgui_controls/Panel.h>

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: Panel that renders a model
//-----------------------------------------------------------------------------
class RenderImagePanel : public Panel
{
	DECLARE_CLASS_SIMPLE( RenderImagePanel, Panel );
public:
	RenderImagePanel(Panel *parent, const char *name);
	~RenderImagePanel();

	virtual void SetModel(const char *modelpath);

	// set the color to fill with, if no image is specified
	void SetFillColor( Color col );
	Color GetFillColor();

protected:
	virtual void PaintBackground();
	virtual void GetSettings(KeyValues *outResourceData);
	virtual void ApplySettings(KeyValues *inResourceData);
	virtual const char *GetDescription();
	virtual void OnSizeChanged(int newWide, int newTall);
	virtual void ApplySchemeSettings( IScheme *pScheme );

private:
	char *m_pImage;
	char *m_pszImageName;
	char *m_pszColorName;
	bool m_bScaleImage;
	Color m_FillColor;
};

} // namespace vgui

#endif // IMAGEPANEL_H
