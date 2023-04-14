/******************************************************************************/
/*!
\file   Option.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Option.h"
#include "Asterisk/FontManager.h"
#include "Asterisk/GameObject/DoorUI.h"

void Option::Initialize(StateManager * statMgr, std::string state)
{
	m_statMgr = statMgr;
	m_txtMgr = statMgr->GetContext()->m_textureMgr;
	m_audMgr = statMgr->GetContext()->m_AudioMgr;

	if (state != "Main")
	{
		MakeSprite(black_screen, "BlackScreen", 0.f, 0.f);
		MakeSprite(settings, "Black_Settings", 0.f, 0.f);
		MakeSprite(key_screen, "Black_KeySettings", 0.f, 0.f);
		MakeSprite(options, "Options", 0.f, 0.f);
		MakeSprite(yesnoBlock, "YesNoBlock", 130.f, -80.f);
		MakeSprite(areUsure, "AreYouSure", 0.f, 0.f);

		soul = new SpriteSheet("PurpleSoul", m_txtMgr);
		soul->SetCurrentAnimation("Body", "Idle", true, true);
		soul->SetShader(m_statMgr->GetContext()->m_window->GetBaseWindow()->GetShaders(), "Animation");
		soul->SetSpritePosition(Vector3f{ OPTION_X, OPTION_Y, MENU_DEPTH });
		soul->SetSpriteScale("Body", 1.3f);
		soul->SetDepth(MENU_DEPTH);
		isMain = false;
	}
	else
		MakeSprite(settings, "Settings", 0.f, 0.f);

	if (m_audMgr->IsMute())
		MakeSprite(MUTE, "Checked", BUTTON_X, MUTE_Y);
	else
		MakeSprite(MUTE, "Unchecked", BUTTON_X, MUTE_Y);

	if (SDL_GL_GetSwapInterval())
		MakeSprite(VSYNC, "Checked", BUTTON_X, VSYNC_Y);
	else
		MakeSprite(VSYNC, "Unchecked", BUTTON_X, VSYNC_Y);

	if (m_statMgr->GetContext()->m_window->IsFullcreen())
		MakeSprite(FULLSCREEN, "Fullscreen", FULLSCREEN_X, FULLSCREEN_Y);
	else
		MakeSprite(FULLSCREEN, "Windowed", FULLSCREEN_X, FULLSCREEN_Y);

	if (DoorUI::GetInstance()->IsDrawing())
		MakeSprite(GUIDEUI, "Checked", BUTTON_X, GUIDEUI_Y);
	else
		MakeSprite(GUIDEUI, "Unchecked", BUTTON_X, GUIDEUI_Y);

	MakeSprite(BGM, "Scroll_button", VOLUME_START_X, BGM_Y);
	MakeSprite(SFX, "Scroll_button", VOLUME_START_X, SFX_Y);
	MakeSprite(selected_setting, "Selected_setting", SELECTED_X, BGM_Y);

	for (int i = DASHL; i <= MAP; i++)
		MakeSprite(i, "itsText", KEY_X, KEY_Y - KEY_Y_GAP * i, 2);

	current_setting = SETTING::BGM;
	current_option = OPTION::BACK;
	current_keyset = KEYS::DASHL;

	areYouSure = false;
	yesOrNo = false;
}

bool Option::Update(float dt)
{
	EventManager * evMgr = m_statMgr->GetContext()->m_evMgr;
	if (isOption)
	{
		if (areYouSure)
		{
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_LEFT) && !yesOrNo)
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
				yesOrNo = true;
				yesnoBlock.SetPosition(-93.f, -80.f);
			}
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_RIGHT) && yesOrNo)
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
				yesOrNo = false;
				yesnoBlock.SetPosition(130.f, -80.f);
			}

			if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
				if (yesOrNo)
				{
					if (m_audMgr->IsPlaying("BGM_HallowDream"))
						m_statMgr->GetContext()->m_AudioMgr->StopSound("BGM_HallowDream");
					if (m_audMgr->IsPlaying("BGM_NightStalker"))
						m_statMgr->GetContext()->m_AudioMgr->StopSound("BGM_NightStalker");

					m_statMgr->SwitchTo(StateType::MAIN);
				}
				areYouSure = false;
				yesOrNo = false;
			}
			return false;
		}

		soul->Update(dt);
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_UP) && current_option != OPTION::BACK)
		{
			current_option--;
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		}
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_DOWN) && current_option != OPTION::MAINMENU)
		{
			current_option++;
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		}
		switch (current_option)
		{
		case BACK:
		{
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
				return true;
			}
			break;
		}
		case SETTINGS:
		{
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");

				current_setting = BGM;
				isOption = false;
				isKeySet = false;
			}
			break;
		}
		case MAINMENU:
		{
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
			{
				areYouSure = true;
			}
			break;
		}
		case KEYSETTING:
		{
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");

				current_setting = DASHL;
				isKeySet = true;
				isOption = false;
			}
			break;
		}
		}
		return false;
	}

	if (isKeySet)
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");

			isKeySet = false;
			isOption = true;
			getInput = false;
			Texture * txt = m_txtMgr->GetResource("Selected_setting");
			selected_setting.SetTexture(*txt);
			selected_setting.BuildMesh(txt);
			return false;
		}

		if (!getInput)
		{
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_UP) && current_setting != KEYS::DASHL)
			{
				current_setting--;
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			}
			if (evMgr->IsKeyTriggered(SDL_SCANCODE_DOWN) && current_setting != KEYS::MAP)
			{
				current_setting++;
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			}
		}
		else
		{
			SDL_Scancode code = evMgr->GetLsatKeyPressed();
			if (evMgr->GetLsatKeyPressed() != SDL_SCANCODE_RETURN)
			{
				m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");

				KEYUI[static_cast<KEYS>(current_setting)].SetString(SDL_GetScancodeName(code));
				getInput = false;
				switch (current_setting)
				{
				case KEYS::DASHL: evMgr->ChangeBinding("Player_Dash_Left", code); break;
				case KEYS::DASHR: evMgr->ChangeBinding("Player_Dash_Right", code); break;
				case KEYS::STOMP: evMgr->ChangeBinding("Player_Stomp", code); break;
				case KEYS::UPPERCUT: evMgr->ChangeBinding("Player_Uppercut", code); break;
				case KEYS::JUMP: evMgr->ChangeBinding("Player_Jump", code); break;
				case KEYS::RIGHT: evMgr->ChangeBinding("Player_Move_Right", code); evMgr->ChangeBinding("Player_Idle_Right", code); break;
				case KEYS::LEFT: evMgr->ChangeBinding("Player_Move_Left", code); evMgr->ChangeBinding("Player_Idle_Left", code); break;
				case KEYS::MAP: evMgr->ChangeBinding("Toggle_WorldMap", code); break;
				default: break;
				}

				Texture * txt;
				txt = m_txtMgr->GetResource("Selected_setting");

				selected_setting.SetTexture(*txt);
				selected_setting.BuildMesh(txt);
			}
		}

		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");

			getInput = !getInput;
			Texture * txt;
			if (!getInput)
				txt = m_txtMgr->GetResource("Selected_setting");
			else
				txt = m_txtMgr->GetResource("Selected_setting_arrow");

			selected_setting.SetTexture(*txt);
			selected_setting.BuildMesh(txt);
		}

		selected_setting.SetPosition(selected_setting.GetPosition().x, KEYUI[static_cast<KEYS>(current_setting)].GetPosition().y - 20.f);

		return false;
	}

	if (evMgr->IsKeyTriggered(SDL_SCANCODE_UP) && current_setting != SETTING::BGM)
	{
		m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		current_setting--;
	}
	if (evMgr->IsKeyTriggered(SDL_SCANCODE_DOWN) && current_setting != SETTING::GUIDEUI)
	{
		m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		current_setting++;
	}
	selected_setting.SetPosition(selected_setting.GetPosition().x, UI[static_cast<SETTING>(current_setting)].GetPosition().y);

	switch (current_setting)
	{
	case BGM:
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_LEFT) && m_audMgr->GetBgVolume() > 0.f)
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			m_audMgr->SetBackgroundVolume(m_audMgr->GetBgVolume() - 0.1f);
			UI[BGM].Move(-SCROLLER_GAP, 0.f);
		}
		else if (evMgr->IsKeyTriggered(SDL_SCANCODE_RIGHT) && m_audMgr->GetBgVolume() < 1.f)
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			m_audMgr->SetBackgroundVolume(m_audMgr->GetBgVolume() + 0.1f);
			UI[BGM].Move(SCROLLER_GAP, 0.f);
		}
		break;
	}
	case SFX:
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_LEFT) && m_audMgr->GetSFXVolume() > 0.f)
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			m_audMgr->SetSFXVolume(m_audMgr->GetSFXVolume() - 0.1f);
			UI[SFX].Move(-SCROLLER_GAP, 0.f);
		}
		else if (evMgr->IsKeyTriggered(SDL_SCANCODE_RIGHT) && m_audMgr->GetSFXVolume() < 1.f)
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			m_audMgr->SetSFXVolume(m_audMgr->GetSFXVolume() + 0.1f);
			UI[SFX].Move(SCROLLER_GAP, 0.f);
		}
		break;
	}
	case MUTE:
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			m_audMgr->ToggleMute();

			Texture * txt;
			if (m_audMgr->IsMute())
				txt = m_txtMgr->GetResource("Checked");
			else
				txt = m_txtMgr->GetResource("Unchecked");

			UI[MUTE].SetTexture(*txt);
			UI[MUTE].BuildMesh(txt);
		}
		break;
	}
	case FULLSCREEN:
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN) || evMgr->IsKeyTriggered(SDL_SCANCODE_RIGHT) || evMgr->IsKeyTriggered(SDL_SCANCODE_LEFT))
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			m_statMgr->GetContext()->m_window->ToggleFullcreen(nullptr);

			Texture * txt;
			if (m_statMgr->GetContext()->m_window->IsFullcreen())
				txt = m_txtMgr->GetResource("Fullscreen");
			else
				txt = m_txtMgr->GetResource("Windowed");

			UI[FULLSCREEN].SetTexture(*txt);
		}
		break;
	}
	case VSYNC:
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			bool interval = SDL_GL_GetSwapInterval();
			SDL_GL_SetSwapInterval(!interval);

			Texture * txt;
			if (!interval)
				txt = m_txtMgr->GetResource("Checked");
			else
				txt = m_txtMgr->GetResource("Unchecked");

			UI[VSYNC].SetTexture(*txt);
		}
		break;
	}
	case GUIDEUI:
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
		{
			m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			auto doorUI = DoorUI::GetInstance();

			doorUI->ToggleDraw();

			Texture * txt;
			if (doorUI->IsDrawing())
				txt = m_txtMgr->GetResource("Checked");
			else
				txt = m_txtMgr->GetResource("Unchecked");

			UI[GUIDEUI].SetTexture(*txt);
		}
		break;
	}
	}

	if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE) && !isMain)
	{
		m_statMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		isOption = true;
	}
	return false;
}

void Option::Draw()
{
	auto * bwindow = m_statMgr->GetContext()->m_window->GetBaseWindow();
	if (isOption)
	{
		if (areYouSure)
		{
			bwindow->CalculateDirectProjectionDraw(areUsure, MENU_DEPTH);
			bwindow->DirectProjectionDraw(areUsure);
			bwindow->CalculateDirectProjectionDraw(yesnoBlock, MENU_DEPTH);
			bwindow->DirectProjectionDraw(yesnoBlock);
		}
		else
		{
			bwindow->Draw(black_screen);
			bwindow->Draw(options);
			soul->Draw(m_statMgr->GetContext()->m_window, false);
		}
	}
	else if (isKeySet)
	{
		bwindow->Draw(key_screen);
		bwindow->Draw(selected_setting);

		for (auto& ui : KEYUI)
			ui.second.Draw(m_statMgr->GetContext()->m_window, 0.0f);
	}
	else
	{
		bwindow->Draw(settings);
		bwindow->Draw(selected_setting);
		for (auto ui : UI)
			bwindow->Draw(ui.second);
	}
}

void Option::MakeSprite(Sprite & sp, std::string name, float x, float y)
{
	m_txtMgr->RequireResource(name);
	Texture * txt = m_txtMgr->GetResource(name);
	sp.SetTexture(*txt);
	sp.BuildMesh(txt);
	sp.SetPosition({ x, y });
	sp.SetDepth(MENU_DEPTH);
}

void Option::MakeSprite(unsigned int type, std::string name, float x, float y, unsigned kind)
{
	if (kind == 2)
	{
		EventManager * evMgr = m_statMgr->GetContext()->m_evMgr;
		Text t;

		t.SetShader(m_statMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));
		t.SetFont(*FontManager::GetInstance()->GetPixelFont());
		t.SetPosition(x, y);
		t.SetScale(0.7f, 0.7f, 1.f);
		t.SetString("a");
		t.SetDepth(GUI_TEXT_DEPTH);

		switch (type) {
		case KEYS::DASHL: t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Dash_Left"))); break;
		case KEYS::DASHR: t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Dash_Right"))); break;
		case KEYS::STOMP:  t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Stomp"))); break;
		case KEYS::UPPERCUT: t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Uppercut"))); break;
		case KEYS::JUMP: t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Jump"))); break;
		case KEYS::RIGHT: t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Move_Right"))); break;
		case KEYS::LEFT:  t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Player_Move_Left"))); break;
		case KEYS::MAP:  t.SetString(SDL_GetScancodeName(evMgr->GetBindingCode("Toggle_WorldMap"))); break;
		}
		KEYUI.insert(std::make_pair(static_cast<KEYS>(type), t));
	}
	else
	{
		m_txtMgr->RequireResource(name);
		Texture * txt = m_txtMgr->GetResource(name);
		Sprite sp;
		sp.SetTexture(*txt);
		sp.BuildMesh(txt);
		sp.SetPosition({ x, y });
		sp.SetDepth(0.8f);

		UI.insert(std::make_pair(static_cast<SETTING>(type), sp));
	}
}

void Option::MoveAllToCenter(float x, float y)
{
	black_screen.SetPosition(x, y);
	selected_setting.SetPosition(x, y);
	options.SetPosition(x, y);

	settings.SetPosition(x, y);
	UI[MUTE].SetPosition(x + BUTTON_X, y + MUTE_Y);
	UI[VSYNC].SetPosition(x + BUTTON_X, y + VSYNC_Y);
	UI[FULLSCREEN].SetPosition(x + FULLSCREEN_X, y + FULLSCREEN_Y);
	UI[BGM].SetPosition(x + VOLUME_START_X + (m_audMgr->GetBgVolume() * 10.f * SCROLLER_GAP), y + BGM_Y);
	UI[SFX].SetPosition(x + VOLUME_START_X + (m_audMgr->GetSFXVolume() * 10.f * SCROLLER_GAP), y + SFX_Y);
	UI[GUIDEUI].SetPosition(x + BUTTON_X, y + GUIDEUI_Y);

	key_screen.SetPosition(x, y);
	int i = 0;
	for (auto & ui : KEYUI)
		ui.second.SetPosition(x + KEY_X, y + KEY_Y - KEY_Y_GAP * i++);

	if (soul)
		soul->SetSpritePosition(Vector3f{ x + OPTION_X, y + OPTION_Y - OPTION_GAP * current_option, MENU_DEPTH });
}

void Option::ResetValues()
{
	yesOrNo = false;
	areYouSure = false;

	yesnoBlock.SetPosition(130.f, -80.f);
}
