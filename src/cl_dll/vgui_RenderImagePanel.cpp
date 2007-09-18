//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#include <vgui/IBorder.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/IBorder.h>
#include <KeyValues.h>

#include <vgui_RenderImagePanel.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CRenderImagePanel::CRenderImagePanel(vgui::Panel *parent, const char *name) : vgui::Panel(parent, name)
{
	m_pImage = NULL;
	m_pszImageName = NULL;
	m_pszColorName = NULL;
	m_bScaleImage = false;
	m_FillColor = Color(0, 0, 0, 0);

	SetModel(m_pImage);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CRenderImagePanel::~CRenderImagePanel()
{
	delete [] m_pszImageName;
	delete [] m_pszColorName;
}

//-----------------------------------------------------------------------------
// Purpose: sets a model by file name
//-----------------------------------------------------------------------------
void CRenderImagePanel::SetModel(const char *modelPath)
{
	int len = Q_strlen(modelPath) + 1;
	m_pszImageName = new char[ len ];
	Q_strncpy(m_pszImageName, modelPath, len );
	InvalidateLayout(false, true); // forrce applyschemesettings to run
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRenderImagePanel::PaintBackground()
{
	if (m_FillColor[3] > 0)
	{
		// draw the specified fill color
		int wide, tall;
		GetSize(wide, tall);
		surface()->DrawSetColor(m_FillColor);
		surface()->DrawFilledRect(0, 0, wide, tall);
	}
	/* render model here 
	if (m_pImage)
	{
		surface()->DrawSetColor(255, 255, 255, 255);
		m_pImage->SetPos(0, 0);
		m_pImage->Paint();
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: Gets control settings for editing
//-----------------------------------------------------------------------------
void CRenderImagePanel::GetSettings(KeyValues *outResourceData)
{
	BaseClass::GetSettings(outResourceData);
	if (m_pszImageName)
	{
		outResourceData->SetString("image", m_pszImageName);
	}
	if (m_pszColorName)
	{
		outResourceData->SetString("fillcolor", m_pszColorName);
	}
	if (GetBorder())
	{
		outResourceData->SetString("border", GetBorder()->GetName());
	}

	outResourceData->SetInt("scaleImage", m_bScaleImage);
}

//-----------------------------------------------------------------------------
// Purpose: Applies designer settings from res file
//-----------------------------------------------------------------------------
void CRenderImagePanel::ApplySettings(KeyValues *inResourceData)
{
	delete [] m_pszImageName;
	delete [] m_pszColorName;
	m_pszImageName = NULL;
	m_pszColorName = NULL;

	m_bScaleImage = inResourceData->GetInt("scaleImage", 0);
	const char *imageName = inResourceData->GetString("model", "");
	if (*imageName)
	{
		SetModel( imageName );
	}

	const char *pszFillColor = inResourceData->GetString("fillcolor", "");
	if (*pszFillColor)
	{
		int r = 0, g = 0, b = 0, a = 255;
		int len = Q_strlen(pszFillColor) + 1;
		m_pszColorName = new char[ len ];
		Q_strncpy( m_pszColorName, pszFillColor, len );

		if (sscanf(pszFillColor, "%d %d %d %d", &r, &g, &b, &a) >= 3)
		{
			// it's a direct color
			m_FillColor = Color(r, g, b, a);
		}
		else
		{
			IScheme *pScheme = scheme()->GetIScheme( GetScheme() );
			m_FillColor = pScheme->GetColor(pszFillColor, Color(0, 0, 0, 0));
		}
	}

	const char *pszBorder = inResourceData->GetString("border", "");
	if (*pszBorder)
	{
		IScheme *pScheme = scheme()->GetIScheme( GetScheme() );
		SetBorder(pScheme->GetBorder(pszBorder));
	}

	BaseClass::ApplySettings(inResourceData);
}


//-----------------------------------------------------------------------------
// Purpose: Describes editing details
//-----------------------------------------------------------------------------
const char *CRenderImagePanel::GetDescription()
{
	static char buf[1024];
	Q_snprintf( buf, sizeof(buf), "%s, string image, string border, string fillcolor, bool scaleImage", BaseClass::GetDescription());
	return buf;
}

//-----------------------------------------------------------------------------
// Purpose: set the color to fill with, if no Image is specified
//-----------------------------------------------------------------------------
void CRenderImagePanel::SetFillColor( Color col )
{
	m_FillColor = col;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
Color CRenderImagePanel::GetFillColor()
{
	return m_FillColor;
}



