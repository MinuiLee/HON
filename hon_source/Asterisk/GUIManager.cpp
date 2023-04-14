/******************************************************************************/
/*!
\file   GUIManager.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <fstream>
#include <sstream>

#include "SharedContext.h"
#include "GUIManager.h"
#include "TextureManager.h"
#include "Window.h"

GUIManager::GUIManager(SharedContext * context) : m_context(context) { LoadGUI("Config/GUI.cfg"); }

GUIManager::~GUIManager()
{
	for (auto & gui_element : m_GUI)
	{
		delete gui_element.second;
		gui_element.second = nullptr;
	}
	m_GUI.clear();
}

void GUIManager::Update(float dt)
{
	for (auto & gui : m_GUI)
	{
		gui.second->Update(dt, m_map);
	}
}

void GUIManager::Draw(Window * window)
{
	for (auto & gui_element : m_GUI)
		gui_element.second->Draw(window);
}

void GUIManager::SetMap(Map * map_) 
{
	m_map = map_;
}

void GUIManager::LoadGUI(const std::string & config)
{
	std::ifstream infile(config);

	std::string line;
	while (std::getline(infile, line))
	{
		if (line[0] == '|' || line.size() == 0) continue;

		std::stringstream keystream(line);
		std::string name;
		std::string location;
		Vector3f position(0, 0, 0);

		keystream >> name;					   // Name of GUI (or texture name)
		keystream >> location;				   // Location of GUI with respect to view space
		keystream >> position.x >> position.y; // Location offsets

		if (location == "TopLeft")
		{
			position.x -= m_context->m_window->GetWindowSize().x / 2.f;
			position.y += m_context->m_window->GetWindowSize().y / 2.f;
		}
		else if (location == "BottomRight")
		{
			position.x += m_context->m_window->GetWindowSize().x / 2.f;
			position.y -= m_context->m_window->GetWindowSize().y / 2.f;
		}

		GUI * newGUI = nullptr;

		newGUI = new GUI(m_context, position, name);

		if (!m_GUI.emplace(std::make_pair(name, newGUI)).second) delete newGUI;
	}

	infile.close();
}
