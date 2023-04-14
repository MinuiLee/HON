/******************************************************************************/
/*!
\file   State_Credit.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Credit.h"
#include "Asterisk/AudioManager.h"

#include "MoveCharacter.h"

State_Credit::State_Credit(StateManager * stateMgr) : BaseState(stateMgr)
{
}

State_Credit::~State_Credit()
{
	Destroy();
}

void State_Credit::Create()
{
	TextureManager * texMgr = m_stateMgr->GetContext()->m_textureMgr;

	MakeSprite(background, texMgr, "MainCredit", 0.f, 0.f);
	MakeSprite(credit, texMgr, "creditMain", 350.f, -1950.f);
	
	InitializeGhost(m_stateMgr->GetEndingType());

	MakeSpriteSheet(bsoul, texMgr, "BlueSoul", "Idle", -505.f, 150.f);
	MakeSpriteSheet(psoul, texMgr, "PurpleSoul", "Idle", -195.f, 150.f);

	m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("BGM_AutumnTime", true);
}

void State_Credit::Destroy()
{
}

void State_Credit::Update(float dt)
{
	if (isStart)
	{
		elapsedTime -= dt;

		if (elapsedTime < 0.f)
			isStart = false;
		else
		{
			float y = BestFit::GetY(elapsedTime, BestFit::FinishInCredit);
			float x = BestFit::GetX(y);
			human.SetPosition(Vector3f(x, y, 1.f));

			float scale = BestFit::GetScale(elapsedTime, 0.f, BestFit::FinishInCredit);
			if (scale < 0.f) scale = 0.1f;
			human.SetScale(scale, scale);
		}
	}
	else
	{
		moveStart += dt;
		isStart = true;
	}

	EventManager * evMgr = m_stateMgr->GetContext()->m_evMgr;
	if (evMgr->IsKeyPressed(SDL_SCANCODE_SPACE) || evMgr->IsKeyPressed(SDL_SCANCODE_RETURN))
		scroll_speed = 500.f;
	else
		scroll_speed = 150.f;

	credit.Move(0.f, dt * scroll_speed);

	if (credit.GetPosition().y >= 1800.f)
		credit.SetPosition(350.f, -1950.f);

	if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
	{
		m_stateMgr->GetContext()->m_AudioMgr->StopSound("BGM_AutumnTime");
		m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = false;
		m_stateMgr->SwitchTo(StateType::MAIN);
	}

	if (evMgr->IsKeyTriggered(SDL_SCANCODE_TAB))
	{
		int type = m_stateMgr->GetEndingType();
		type++;
		type = type > EndingType::MINUI ? EndingType::JAEJUN : type;
		InitializeGhost(static_cast<EndingType>(type));
		m_stateMgr->GetEndingType() = static_cast<EndingType>(type);
	}

	bsoul->Update(dt);
	psoul->Update(dt);
}

void State_Credit::Draw()
{
	m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(background);
	m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(credit);
	m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(human);

	bsoul->Draw(m_stateMgr->GetContext()->m_window, false);
	psoul->Draw(m_stateMgr->GetContext()->m_window, false);
}

void State_Credit::MakeSprite(Sprite & sp, TextureManager * texMgr, std::string name, float x, float y)
{
	texMgr->RequireResource(name);
	Texture * txt = texMgr->GetResource(name);
	sp.SetTexture(*txt);
	sp.BuildMesh(txt);
	sp.SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));
	sp.SetPosition({ x, y });
}

void State_Credit::MakeSpriteSheet(SpriteSheet *& sp, TextureManager * texMgr, std::string name, std::string type, float x, float y)
{
	sp = new SpriteSheet(name, texMgr);
	sp->SetCurrentAnimation("Body", type, true, true);
	sp->SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders(), "Animation");
	sp->SetSpritePosition(Vector3f(x, y, 0.f));
}

void State_Credit::InitializeGhost(EndingType type)
{
	TextureManager * texMgr = m_stateMgr->GetContext()->m_textureMgr;

	elapsedTime = BestFit::StartInCredit;
	float y = BestFit::GetY(elapsedTime, BestFit::FinishInCredit);
	float x = BestFit::GetX(y);

	switch (type)
	{
	case EndingType::HYUNGSEOB:
		MakeSprite(human, texMgr, "KHS", x, y);
		break;
	case EndingType::JAEJUN:
		MakeSprite(human, texMgr, "JJJ", x, y);
		break;
	case EndingType::JOOHO:
		MakeSprite(human, texMgr, "JJH", x, y);
		break;
	case EndingType::MINUI:
		MakeSprite(human, texMgr, "LMU", x, y);
		break;
	default:
		break;
	}

	float scale = BestFit::GetScale(elapsedTime, 0.f, BestFit::FinishInCredit);
	if (scale < 0.f) scale = 0.1f;
	human.SetScale(scale, scale);
}