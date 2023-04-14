#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
/******************************************************************************/
/*!
\headerfile   EventManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for EventDetails struct.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <SDL2/include/SDL.h>
#include <unordered_map>
#include <functional>
#include <rapidjson/document.h>

enum class EventType
{
    KeyDown       = SDL_KEYDOWN,
    KeyUp	 = SDL_KEYUP,
    MButtonDown   = SDL_MOUSEBUTTONDOWN,
    MButtonUp     = SDL_MOUSEBUTTONUP,
	MouseWheel	= SDL_MOUSEWHEEL,
    Closed	= SDL_QUIT,
    Keyboard      = 773,
    Mouse
};

struct EventCode
{
    EventCode(int code = 0)
	: m_code(code) {}
    int m_code;
};

using Events = std::pair<EventType, EventCode>;

struct EventDetails
{
    EventDetails(const std::string & name)
	: m_name(name) {}

    std::string m_name;
};

struct Binding
{
    Binding(const std::string & name)
	: m_name(name), m_details(name), c(0) {}

    std::string m_name;
    EventDetails m_details;
    int c; //Number of events that are on
    Events m_events;
};

using Bindings = std::unordered_map<std::string, Binding *>;

using Callback = std::unordered_map<std::string,
    std::function<void(EventDetails *)>>;

enum class StateType; //forward declaration

using Callbacks = std::unordered_map<StateType, Callback>;


class EventManager
{
public:
    //Ctor & Dtor
    EventManager();
    ~EventManager();

    template <typename T>
    void AddCallback(const StateType & type,
	void (T::*func)(EventDetails *), T * instance,
	const std::string & name)
    {
	auto temp = std::bind(func, instance, std::placeholders::_1);

	auto itr = m_callbacks.find(type);
	if (itr == m_callbacks.end())
	    m_callbacks[type].emplace(name, temp);
	else
	    itr->second.emplace(name, temp);
    }

    void RemoveCallback(const StateType & type,
	const std::string & name)
    {
	auto itr = m_callbacks.find(type);
	if (itr != m_callbacks.end())
	    itr->second.erase(name);
    }

    //Window event handler
    void HandleEvent(SDL_Event event);
    //Real-time input handler
    void Update();

    void SetFocus(bool focus) { m_isFocus = focus; }
    void SetState(const StateType & state) { m_currentState = state; }

    //Key trigger
    bool IsKeyTriggered(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key);
    bool IsKeyReleased(SDL_Scancode key);
    void SetKeyState(SDL_Scancode key, bool b = false);
	SDL_Scancode GetLsatKeyPressed(){ return m_lastcode; }
	void SetLastKeyPressd(SDL_Scancode code) { m_lastcode = code; }

    void SetMouseState(Uint8 button, bool b = false);
	bool IsMouseTriggered(Uint8 button);
	bool IsMousePressed(Uint8 button);
	bool IsMouseReleased(Uint8 button);

	void SetMouseWheelMove(int m);
	int GetMouseWheelMove();

    void SaveBindings();
	void ChangeBinding(std::string name, EventCode evtCode);
	void LoadBinginds();
	SDL_Scancode GetBindingCode(std::string name);

private:
    bool m_isFocus = false;
    StateType m_currentState;
    Bindings m_bindings;
    Callbacks m_callbacks;
	SDL_Scancode m_lastcode;

    std::unordered_map<SDL_Scancode, bool> key_states;
    std::unordered_map<Uint8, bool> mouse_states;
	int mouseWheelMove = 0;

    rapidjson::Document m_doc;
};

#endif