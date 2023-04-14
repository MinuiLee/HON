#ifndef WINDOW_H
#define WINDOW_H
/******************************************************************************/
/*!
\headerfile   Window.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for window class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <chrono>
#include <filesystem>

#include "EventManager.h"
#include "BaseWindow.h"

#define INITIAL_WIDTH 1280
#define INITIAL_HEIGHT 800

class StateManager;
class Window
{
public:
    //Ctor & Dtor
    Window(StateManager * stateMgr);
    ~Window() { Destroy(); }

    //Event polling
    void Update();

    //Drawing
    void StartDraw() { m_window.StartDraw(); }
    void EndDraw() { m_window.EndDraw(); }

    //Data getting methods
    BaseWindow * GetBaseWindow() { return &m_window; }
    EventManager * GetEventManager() { return &m_evMgr; }
    Vector2u GetWindowSize() { return m_windowSize; }
	bool IsFullcreen() const { return m_isFullscreen; }
    bool IsDone() const { return m_isDone; }
    bool IsWindowFocused()const { return m_isFocus; }

    //Callbacks
	void ToggleFullcreen(EventDetails * /*details*/);
    void Close(EventDetails * /*details*/) { m_isDone = true; }

private:
    //Helper methods
    void Setup(const Vector2u & size, const std::string & title);
    void Destroy() { m_window.Destroy();
		std::filesystem::remove("Media/Maps/map_current.json");
	}
    void UpdateFPS();

    BaseWindow m_window; //Actual window
    Vector2u m_windowSize; //Size of window
    std::string m_title;       //Name of window
    StateManager * m_stateMgr;

    bool m_isDone;
	bool m_isFullscreen;
    bool m_isFocus;
    EventManager m_evMgr;

    // For FPS
    std::chrono::system_clock::time_point m_elapsedTime;
    unsigned int m_frame;
    float m_time;
};

#endif