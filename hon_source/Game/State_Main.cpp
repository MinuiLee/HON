/******************************************************************************/
/*!
\file   State_Main.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Main.h"
#include "Asterisk/ShaderContainer.h"
#include "Asterisk/SystemManager.h"
#include "Asterisk/C_Position.h"
#include "Asterisk/C_Audio.h"
#include "Asterisk/AudioManager.h"
#include "Asterisk/FontManager.h"

#include "MoveCharacter.h"

State_Main::State_Main(StateManager * stateMgr) : BaseState(stateMgr)
{
}

State_Main::~State_Main()
{
	Destroy();
}

void State_Main::Create()
{
	m_stateMgr->GetContext()->m_systemMgr->InitializeShader(&(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders()));

	TextureManager * texMgr = m_stateMgr->GetContext()->m_textureMgr;

	MakeSprite(start, texMgr, "main_start", MENU_X - 37.f, MENU_Y);
	MakeSprite(setting, texMgr, "main_setting", MENU_X, MENU_Y + MENU_GAP);
	MakeSprite(credit, texMgr, "main_credit", MENU_X - 20.f, MENU_Y + MENU_GAP * 2);
	MakeSprite(quit, texMgr, "main_quit", MENU_X - 45.f, MENU_Y + MENU_GAP * 3);

	MakeSprite(main, texMgr, "Main", 0.f, 0.f);

	option.Initialize(m_stateMgr, "Main");

	MakeSprite(background, texMgr, "MainCredit", 0.f, 0.f);
	MakeSprite(creditText, texMgr, "creditMain", 350.f, -1950.f);
	MakeSprite(copywright, texMgr, "copywright", 0.f, -380.f);

	MakeSpriteSheet(soul, texMgr, "PurpleSoul", "Idle", MENU_X - 150.f, MENU_Y + 5.f);
	MakeSpriteSheet(bsoul1, texMgr, "BlueSoul", "Idle", -505.f, 150.f);
	MakeSpriteSheet(bsoul2, texMgr, "PurpleSoul", "Idle", -195.f, 150.f);
	MakeSpriteSheet(player, texMgr, "MainPlayer", "Idle", 150.f, -250.f);

	m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("BGM_GhostsWalkHere", true);
	button = BUTTON::START;
	isSwitching = false;
	isStart = false;
	isStart = false;

	elapsedTime = BestFit::StartInMain;

	MakeSprite(yesnoBlock, texMgr, "YesNoBlock", 130.f, -80.f);
	MakeSprite(areUsure, texMgr, "AreYouSure", 0.f, 0.f);
}

void State_Main::Destroy()
{
}

void State_Main::Update(float dt)
{
	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn)
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer -= dt;

	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer < 0.0f)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.f;
	}

	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut)
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer += dt * BestFit::TimerFactor;

	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer > 1.0f)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.f;

		if (isSwitching)
		{
			m_stateMgr->GetContext()->m_AudioMgr->StopSound("BGM_GhostsWalkHere");
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = true;
			m_stateMgr->SwitchTo(StateType::GAME);
			return;
		}
	}

	if (isStart && !isSwitching)
	{
		elapsedTime += dt;
		if (elapsedTime > BestFit::FinishInMain)
		{
			isSwitching = true;
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = true;
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.0f;
		}
		float y = BestFit::GetY(elapsedTime*0.85f, BestFit::FinishInMain);
		float x = BestFit::GetX(y);
		player->SetSpritePosition(Vector3f(x, y, 1.f));
		player->SetSpriteScale("Body", BestFit::GetScale(elapsedTime*0.95f, BestFit::StartInMain, BestFit::FinishInMain));
	}

	EventManager * evMgr = m_stateMgr->GetContext()->m_evMgr;

	soul->Update(dt);
	bsoul1->Update(dt);
	bsoul2->Update(dt);
	player->Update(dt);

	if (areYouSure)
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_LEFT) && !yesOrNo)
		{
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			yesOrNo = true;
			yesnoBlock.SetPosition(-93.f, -80.f);
		}
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RIGHT) && yesOrNo)
		{
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			yesOrNo = false;
			yesnoBlock.SetPosition(130.f, -80.f);
		}

		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
		{
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			if (yesOrNo)
			{
				m_stateMgr->GetContext()->m_window->Close(nullptr);
			}
			areYouSure = false;
			yesOrNo = false;
		}
		return;
	}

	if (!isOption && !isCredit && !isStart)
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_UP) && button != BUTTON::START)
		{
			button--;
			soul->GetSprite("Body")->Move(0.f, -MENU_GAP);
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		}
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_DOWN) && button != BUTTON::QUIT)
		{
			button++;
			soul->GetSprite("Body")->Move(0.f, MENU_GAP);
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		}

		if (evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN))
		{
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");

			switch (button)
			{
			case BUTTON::START:
			{
				isStart = true;
				button = BUTTON::START;
				break;
			}
			case BUTTON::SETTING:
			{
				isOption = !isOption;
				if (isOption)
					option.MoveAllToCenter(0.f, 0.f);
				break;
			}
			case BUTTON::CREDIT:
			{
				creditText.SetPosition(350.f, -1950.f);
				isCredit = true;
				break;
			}
			case BUTTON::QUIT:
			{
				areYouSure = true;
				break;
			}
			}
		}
	}
	else if (isOption)
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
		{
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
			isOption = false;
		}
		option.Update(dt);
	}
	else if (isCredit)
	{
		if (evMgr->IsKeyPressed(SDL_SCANCODE_RETURN) || evMgr->IsKeyPressed(SDL_SCANCODE_SPACE))
			scroll_speed = 500.f;
		else
			scroll_speed = 150.f;

		creditText.Move(0.f, dt * scroll_speed);

		if (creditText.GetPosition().y >= 1800.f)
			creditText.SetPosition(350.f, -1950.f);

		if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
		{
			isCredit = false;
			m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		}
	}
}

void State_Main::Draw()
{
	m_stateMgr->GetContext()->m_systemMgr->Draw(m_stateMgr->GetContext()->m_window);

	if (!isCredit)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(main);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(credit);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(setting);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(start);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(quit);

		soul->Draw(m_stateMgr->GetContext()->m_window, false);
	}

	if (isOption)
		option.Draw();

	if (isCredit)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(background);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(creditText);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(copywright);
	}

	bsoul1->Draw(m_stateMgr->GetContext()->m_window, false);
	bsoul2->Draw(m_stateMgr->GetContext()->m_window, false);
	if (!isSwitching)
		player->Draw(m_stateMgr->GetContext()->m_window, false);

	if (areYouSure)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(areUsure);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(yesnoBlock);
	}
}

void State_Main::MakeSprite(Sprite & sp, TextureManager * texMgr, std::string name, float x, float y)
{
	texMgr->RequireResource(name);
	Texture * txt = texMgr->GetResource(name);
	sp.SetTexture(*txt);
	sp.BuildMesh(txt);
	sp.SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));
	sp.SetPosition({ x, y });
}


void State_Main::MakeSpriteSheet(SpriteSheet *& sp, TextureManager * texMgr, std::string name, std::string type, float x, float y)
{
	sp = new SpriteSheet(name, texMgr);
	sp->SetCurrentAnimation("Body", type, true, true);
	sp->SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders(), "Animation");
	sp->SetSpritePosition(Vector3f(x, y, 0.f));
}