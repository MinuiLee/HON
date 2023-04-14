#ifndef FONTMANAGER_H
#define FONTMANAGER_H
/*!*******************************************************************
\headerfile   FontManager.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/31
\brief
              Header file for Font manager class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Font.h"

class FontManager
{
public:
	~FontManager();

	static FontManager * GetInstance();
    void Clear();

    Font * GetBahnschrift() { return &m_Bahnschrift; }
    Font * GetCalibri() { return &m_Calibri; }
	Font * GetHarrington() { return &m_Harrington; }
	Font * GetPixelFont() { return &m_Pixel; }

private:
    FontManager();
    FontManager(const FontManager&);
    FontManager& operator=(const FontManager&);

    Font m_Bahnschrift;
    Font m_Calibri;
	Font m_Harrington;
	Font m_Pixel;
};

#endif //FONTMANAGER_H