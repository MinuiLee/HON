/******************************************************************************/
/*!
\file   EventManager.cpp
\author Jooho Jeong, secondary: Minui Lee
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <fstream>
#include <sstream>
#include <iostream>

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

#include "EventManager.h"
#include "GameObject/DoorUI.h"

// Ctor & Dtor
EventManager::EventManager() : m_isFocus(true), m_currentState(StateType(0))
{
	// Initialize member 'm_bindings'
	LoadBinginds();
}

EventManager::~EventManager()
{
	for (auto & itr : m_bindings)
	{
		delete itr.second;
		itr.second = nullptr;
	}
	m_bindings.clear();
}

// Event Handler
void EventManager::HandleEvent(SDL_Event event)
{
	for (auto & itr : m_bindings)
	{
		Binding * bind = itr.second;
		Events evt	 = bind->m_events;

		EventType type = evt.first;
		// Skip itr that does not match EventType
		if (type != EventType(event.type)) continue;

		// Check the specific EventType & EventCode
		// KeyboardDown
		if (type == EventType::KeyDown|| type == EventType::KeyUp)
		{
			if (event.key.keysym.scancode == evt.second.m_code && event.key.repeat == 0)
			{
				++bind->c;
				break;
			}
		}
		// MouseButtonDown
		else if (type == EventType::MButtonDown || type == EventType::MButtonUp)
		{
			if (event.button.button == evt.second.m_code)
			{
				++bind->c;
				break;
			}
		}
		// Other events
		else
			++bind->c;
	}
}

// Realtime input checking
void EventManager::Update()
{
	// Check window focus
	if (!m_isFocus) { return; }


	for (auto & itr : m_bindings)
	{
		Binding * bind		= itr.second;
		Events evt			= bind->m_events;
		const Uint8 * key_state = SDL_GetKeyboardState(nullptr);
		// Check the EventType & EventCode
		if (evt.first == EventType::Keyboard && key_state[evt.second.m_code])
			++bind->c;
		else if (evt.first == EventType::Mouse && IsMousePressed(static_cast<Uint8>(evt.second.m_code)))
			++bind->c;

		// Consider that sf::event is already dealt with!!
		// If all Event are "on"
		if (bind->c == 1)
		{
			// Check global callbacks first
			auto global	= m_callbacks.find(StateType(0));
			auto gCallback = global->second.find(bind->m_name);
			if (gCallback != global->second.end()) { gCallback->second(&bind->m_details); }
			else // Check callbacks in current state
			{
				auto state = m_callbacks.find(m_currentState);
				if (state != m_callbacks.end())
				{
					auto callback = state->second.find(bind->m_name);
					if (callback != state->second.end()) callback->second(&bind->m_details);
				}
			}
		}

		// Reset
		bind->c = 0;
	}

	DoorUI::GetInstance()->SetText(GetBindingCode("Player_Dash_Left"), GetBindingCode("Player_Stomp"));
}

bool EventManager::IsKeyTriggered(SDL_Scancode key)
{
	if (key_states[key])
	{
		key_states[key] = false;
		return true;
	}
	return false;
}

bool EventManager::IsKeyPressed(SDL_Scancode key) { return key_states[key]; }

bool EventManager::IsKeyReleased(SDL_Scancode key) { return !key_states[key]; }

void EventManager::SetKeyState(SDL_Scancode key, bool b) { key_states[key] = b; }

void EventManager::SetMouseState(Uint8 button, bool b) { mouse_states[button] = b; }

bool EventManager::IsMouseTriggered(Uint8 button)
{
	if (mouse_states[button])
	{
		mouse_states[button] = false;
		return true;
	}
	return false;
}

bool EventManager::IsMousePressed(Uint8 button)
{
	return static_cast<bool>(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button));
}

bool EventManager::IsMouseReleased(Uint8 button) { return !mouse_states[button]; }

void EventManager::SetMouseWheelMove(int m) { mouseWheelMove = m; }

int EventManager::GetMouseWheelMove() { return mouseWheelMove; }


void EventManager::SaveBindings()
{
	FILE * fp = nullptr;

	errno_t err = fopen_s(&fp, "Config/Keys.json", "wb");
	if (err != 0) return;

	char Buffer[65536];

	rapidjson::FileWriteStream os(fp, Buffer, sizeof(Buffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);

	writer.StartObject();
	writer.Key("BINDINGS");
	writer.StartArray();

	for (auto & b : m_bindings)
	{
		writer.StartObject();
		writer.Key("Name");
		writer.String(b.first.c_str());
		writer.Key("EventType");
		writer.Int(static_cast<int>(b.second->m_events.first));
		writer.Key("EventCode");
		writer.Int(static_cast<int>(b.second->m_events.second.m_code));
		writer.EndObject();
	}

	writer.EndArray();
	writer.EndObject();
	fclose(fp);
}

void EventManager::ChangeBinding(std::string name, EventCode evtCode)
{
	for (auto & itr : m_bindings)
	{
		if (itr.first != name) continue;

		itr.second->m_events.second = evtCode;
		SaveBindings();
		return;
	}
}

void EventManager::LoadBinginds()
{
	FILE * fp = nullptr;

	errno_t err = fopen_s(&fp, "Config/Keys.json", "rb");
	if (err != 0) return;

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	m_doc.ParseStream(is);

	const rapidjson::Value & Bindings = m_doc["BINDINGS"];
	assert(Bindings.IsArray());

	rapidjson::SizeType size = Bindings.Size();
	for (rapidjson::SizeType i = 0; i < size; i++)
	{
		auto binding = Bindings[i].GetObject();

		std::string name = binding.FindMember("Name")->value.GetString();

		Binding * bind		  = new Binding(name);
		bind->m_events.first  = EventType(binding.FindMember("EventType")->value.GetInt());
		bind->m_events.second = EventCode(binding.FindMember("EventCode")->value.GetInt());

		if (!m_bindings.emplace(name, bind).second) delete bind;
	}

	fclose(fp);
}

SDL_Scancode EventManager::GetBindingCode(std::string name)
{
	for (auto & itr : m_bindings)
	{
		if (itr.first == name) return static_cast<SDL_Scancode>(itr.second->m_events.second.m_code);
	}
    return SDL_Scancode::SDL_SCANCODE_UNKNOWN;
}
