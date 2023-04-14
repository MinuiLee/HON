/******************************************************************************/
/*!
\file   Window.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "StateManager.h"
#include "Window.h"

//Ctors
Window::Window(StateManager * stateMgr)
	: m_stateMgr(stateMgr)
{
	Setup(Vector2u(INITIAL_WIDTH, INITIAL_HEIGHT), std::string("HON"));
    m_elapsedTime = std::chrono::system_clock::now();
    m_frame       = 0;
    m_time	= 0.f;

    SDL_SetWindowResizable(m_window.GetSDLWindow(), SDL_FALSE);
}

//Window updating
void Window::Update()
{
	SDL_Event event;

	//Event polling
	while (SDL_PollEvent(&event))
	{
		//Focus setting
        switch (event.type)
        {
        case SDL_QUIT:
        {
            m_isDone = true;
            break;
        }
        case SDL_WINDOWEVENT:
        {
            if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
            {
                m_isFocus = false;
                m_evMgr.SetFocus(false);
            }
            else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
            {
                m_isFocus = true;
                m_evMgr.SetFocus(true);
            }
            else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                GetBaseWindow()->ResizeWindow();
            break;
        }
        case SDL_KEYDOWN:
        {
            m_evMgr.SetKeyState(event.key.keysym.scancode, true);
			m_evMgr.SetLastKeyPressd(event.key.keysym.scancode);
            break;
        }
        case SDL_KEYUP:
        {
            m_evMgr.SetKeyState(event.key.keysym.scancode);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            m_evMgr.SetMouseState(event.button.button, true);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            m_evMgr.SetMouseState(event.button.button);
            break;
        }
        }
		
		//mouse wheel move set (+ : up, - : down)
		if (event.type == SDL_MOUSEWHEEL)
			m_evMgr.SetMouseWheelMove(static_cast<int>(event.wheel.y));
		else
			m_evMgr.SetMouseWheelMove(0);

		//Window event handler
		m_evMgr.HandleEvent(event);
	}
	//Real-time input handler
	m_evMgr.Update();

        UpdateFPS();
}

//Callback method
void Window::ToggleFullcreen(EventDetails * /*details*/)
{
    SDL_Window * window = m_window.GetSDLWindow();
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;

    SDL_SetWindowFullscreen(window, IsFullscreen ? 0 : FullscreenFlag);
    SDL_ShowCursor(IsFullscreen);
    
	m_isFullscreen = !m_isFullscreen;
}

////////Helper method
//Initializing window
void Window::Setup(const Vector2u & size,
	const std::string & title)
{
	m_windowSize = size;
	m_title = title;
	m_isDone = false;
	m_isFullscreen = true;
	m_isFocus = true;

	m_evMgr.AddCallback(StateType(0), &Window::Close, this, "Window_Close");
	m_evMgr.AddCallback(StateType(0), &Window::ToggleFullcreen, this, "Toggle_Fullscreen");
	m_evMgr.AddCallback(StateType(0), &BaseWindow::ResizeWindow, &m_window, "Window_Resized");

    m_window.Create(Vector2u(m_windowSize.x, m_windowSize.y), m_title);
}

void Window::UpdateFPS()
{
    std::chrono::duration<double> deltatime = std::chrono::system_clock::now() - m_elapsedTime;
    m_elapsedTime			    = std::chrono::system_clock::now();

    ++m_frame;
    m_time += static_cast<float>(deltatime.count());

    if (m_time >= 1.f)
    {
	const std::string title = m_title + " FPS[" + std::to_string(m_frame) + "]";
    SDL_SetWindowTitle(m_window.GetSDLWindow(), title.c_str());

	m_time  = 0.f;
	m_frame = 0;
    }
}