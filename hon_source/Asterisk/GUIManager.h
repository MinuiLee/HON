#ifndef GUIMANAGER_H
#define GUIMANAGER_H
/******************************************************************************/
/*!
\headerfile   GUIManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for graphic UI manager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>
#include <unordered_map>

#include "GUI.h"
#include "Map.h"

class Window;
class GUIManager
{
public:
	GUIManager(SharedContext * context);
	~GUIManager();

	void Update(float dt);
	void Draw(Window * window);
	void SetMap(Map * map_);

private:
	void LoadGUI(const std::string & config);

	std::unordered_map<std::string, GUI*> m_GUI;
	SharedContext * m_context;
	Map * m_map;
};

#endif