#ifndef SHAREDCONTEXT_H
#define SHAREDCONTEXT_H
/******************************************************************************/
/*!
\headerfile   SharedContext.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for SharedContext class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
class Window;
class TextureManager;
class EventManager;
class StateManager;
class SystemManager;
class EntityManager;
class AudioManager;
class GUIManager;

class SharedContext
{
public:
    SharedContext(
	Window * wind,
	TextureManager * tMgr,
	EventManager * evMgr,
	StateManager * stMgr,
	SystemManager * sysMgr,
	EntityManager * entityMgr,
	AudioManager * audioMgr,
	GUIManager * GUIMgr)
	:
	  m_window(wind),
	  m_textureMgr(tMgr),
	  m_evMgr(evMgr),
	  m_stateMgr(stMgr),
	  m_systemMgr(sysMgr),
	  m_entityMgr(entityMgr),
	  m_AudioMgr(audioMgr),
	  m_GUIMgr(GUIMgr)
    {}

    Window * m_window;
    TextureManager * m_textureMgr;
    EventManager * m_evMgr;
    StateManager * m_stateMgr;
    SystemManager * m_systemMgr;
    EntityManager * m_entityMgr;
    AudioManager * m_AudioMgr;
    GUIManager * m_GUIMgr;
};

#endif