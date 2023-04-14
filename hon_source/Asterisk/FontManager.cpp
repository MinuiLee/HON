/*!*******************************************************************
\file         FontManager.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/31

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "FontManager.h"

FontManager::FontManager()
{
    m_Bahnschrift.LoadFromFile("Media/Font/Bahnschrift.fnt");
    m_Calibri.LoadFromFile("Media/Font/Calibri.fnt");
	m_Harrington.LoadFromFile("Media/Font/Harrington.fnt");
	m_Pixel.LoadFromFile("Media/Font/pixel_font.fnt");
}

FontManager::~FontManager()
{
    Clear();
}

FontManager* FontManager::GetInstance()
{
    static FontManager instance;
    return &instance;
}

void FontManager::Clear()
{
    m_Bahnschrift.Clear();
    m_Calibri.Clear();
	m_Harrington.Clear();
	m_Pixel.Clear();
}