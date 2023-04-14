/******************************************************************************/
/*!
\file   State_Game.cpp
\author Jooho Jeong, Minui Lee, Jaejun Jang, Hyung-seob Kim
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Game.h"
#include "Game.h"

#include "Asterisk/S_Collision.h"
#include "Asterisk/S_SpriteSheet.h"
#include "Asterisk/S_Renderer.h"
#include "Asterisk/S_Particle.h"

#include "Asterisk/C_RigidBody.h"
#include "Asterisk/C_Position.h"
#include "Asterisk/C_Collidable.h"
#include "Asterisk/C_Attackable.h"
#include "Asterisk/C_State.h"

#include "Asterisk/C_Status.h"
#include "Asterisk/C_Sprite.h"

#include "Asterisk/EntityManager.h"
#include "Asterisk/C_Attackable.h"

namespace
{
	constexpr float TUTORIAL_MESSAGE_TIME = 5.f;
}

// Ctor
State_Game::State_Game(StateManager * stateMgr)
	: BaseState(stateMgr), m_map(nullptr), IsEditingMode(false)
	, m_dust_particle(150, 0.0f, glm::uvec2(16, 16), glm::uvec2(m_stateMgr->GetContext()->m_window->GetWindowSize().x, m_stateMgr->GetContext()->m_window->GetWindowSize().y))
{
	m_editor = new MapEditor(stateMgr);
}

// Dtor
State_Game::~State_Game()
{
	Destroy();
	delete m_editor;
}

// State initializing
void State_Game::Create()
{
	SharedContext * context = m_stateMgr->GetContext();
	// Allocating new map and init mapeditor
	m_map = new Map(context, "game");
	m_map->SaveMap("current");
	m_editor->SetMap(m_map);
	context->m_GUIMgr->SetMap(m_map);

	m_fadeInOut = make_pair(2.f, false);
	m_bossTimer = make_pair(4.f, false);

	const Vector2f & player_pos = context->m_entityMgr->
		GetC_Position(context->m_entityMgr->GetPlayerID())->GetPosition();
	m_map->SetView(player_pos);
	context->m_window->GetBaseWindow()->GetView()->SetCenter(player_pos);

	m_option.Initialize(m_stateMgr, "Game");
	// Initializing collision system with current map
	S_Collision * col =
		context->m_systemMgr->GetSystem<S_Collision>(System::Collision);
	col->SetMap(m_map);

	context->m_systemMgr->
		InitializeShader(&(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders()));

	// Add callback
	EventManager * evMgr = context->m_evMgr;
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Idle_Left");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Idle_Right");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Move_Left");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Move_Right");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Jump");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Dash_Left");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Dash_Right");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Stomp");
	evMgr->AddCallback(StateType::GAME, &State_Game::PlayerControl, this, "Player_Uppercut");
	evMgr->AddCallback(StateType::GAME, &State_Game::ToggleInvincible, this, "Toggle_Invincible_Cheat");
	evMgr->AddCallback(StateType::GAME, &State_Game::SetHealth1, this, "Health_1_Cheat");
	evMgr->AddCallback(StateType::GAME, &State_Game::ToggleWorldMap, this, "Toggle_WorldMap");

	IsStart = true;
	IsOption = false;

	TextureManager * texMgr = context->m_textureMgr;

	Texture* dust = texMgr->GetResource("dust");
	m_dust_particle.PushBack(dust);

	Shader* dustshader = m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Dust");
	dustshader->CompileShader();
	m_dust_particle.SetShader(dustshader);

	m_dust_particle.SetProjection(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetProjectionPtr());
	m_stateMgr->GetContext()->m_systemMgr->GetSystem<S_Renderer>(System::Renderer)->UpdateFragmentParticleTexture(m_stateMgr->GetContext()->m_textureMgr);

	m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("BGM_HallowDream", true);
	m_stateMgr->GetContext()->m_systemMgr->GetSystem<S_Particle>(System::Particle)->UpdateSoulTexture(m_stateMgr->GetContext()->m_textureMgr);

	Texture * boss_health_outer_tex = texMgr->GetResource("BossHealthOuter");
	Texture * boss_health_inner_tex = texMgr->GetResource("BossHealthInner");
	boss_health_outer.SetTexture(*boss_health_outer_tex);
	boss_health_inner.SetTexture(*boss_health_inner_tex);
	boss_health_outer.BuildMesh(boss_health_outer_tex);
	boss_health_inner.BuildMesh(boss_health_inner_tex);

	const Vector2f window_size = context->m_window->GetWindowSize();
	boss_health_outer.SetPosition(0, -window_size.y / 3 - 90);
	boss_health_outer.SetScale(Vector2f(1.5f, 1));
	boss_health_inner.SetPosition(0, -window_size.y / 3 - 90);
	boss_health_inner.SetScale(Vector2f(1.5f, 1));
}

void State_Game::Destroy()
{
	if (m_map)
	{
		delete m_map;
		m_map = nullptr;
		m_stateMgr->GetContext()->m_entityMgr->Purge();
		m_stateMgr->GetContext()->m_systemMgr->PurgeEntities();
	}
}

// Game updating
void State_Game::Update(float dt)
{
	if (IsStart)
	{
		if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn)
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer -= dt;

		if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer < 0.0f)
		{
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = false;
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.f;
			IsStart = false;
		}
	}
	BaseWindow * base_window = m_stateMgr->GetContext()->m_window->GetBaseWindow();
	base_window->m_shake_timer -= dt * 10;
	base_window->UpdateShakeTimer(dt);
	m_stateMgr->GetContext()->m_GUIMgr->Update(dt);
	if (m_map)
		m_map->SetCamera(dt, IsEditingMode || m_map->IsWorldMapOpen());

	EventManager * evMgr = m_stateMgr->GetContext()->m_evMgr;
	if (m_map->IsWorldMapOpen())
	{
		if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
			ToggleWorldMap(nullptr);

		m_stateMgr->GetContext()->m_GUIMgr->Update(dt);
		return;
	}

	if (IsOption)
	{
		m_option.MoveAllToCenter(m_map->GetViewCenter().x, m_map->GetViewCenter().y);

		if (m_option.Update(dt))
		{
			IsOption = false;
			m_option.ResetValues();
		}
	}

	if (evMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE) && !IsEditingMode)
	{
		m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Button");
		IsOption = !IsOption;
		m_option.SetIsOption(true);
		m_option.ResetValues();
	}

	if (evMgr->IsKeyTriggered(SDL_SCANCODE_TAB) && !IsOption)
	{
		IsEditingMode = !IsEditingMode;
		m_map->IsMapEditingMode(IsEditingMode);
		base_window->SetZoom(1.0f);
	}

	if (!IsEditingMode && !IsOption)
	{
		m_editor->SetIsChanged(true);
		m_stateMgr->GetContext()->m_systemMgr->Update(m_stateMgr->GetContext()->m_window, dt, IsOption);
		m_stateMgr->GetContext()->m_GUIMgr->Update(dt);


		if (evMgr->IsKeyTriggered(SDL_SCANCODE_TAB))
		{
			IsEditingMode = !IsEditingMode;
			m_map->IsMapEditingMode(IsEditingMode);
		}

		// Player Control
		m_elapsed_time_from_attack += dt;

		//fade in out when die
		if (m_stateMgr->GetContext()->m_entityMgr->GetC_State(m_map->GetPlayerId())->GetState() == EntityState::Dead)
		{
			m_fadeInOut.second = true;
			m_fadeInOut.first = 2.f;
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = true;
			if (m_map->GetMapName() == "boss")
			{
				m_stateMgr->GetContext()->m_AudioMgr->StopSound("BGM_NightStalker");
				m_stateMgr->GetEndingType() = JAEJUN;
				m_stateMgr->SwitchTo(StateType::CREDIT);
				return;
			}
		}

		//when gather all four flowers and move to Boss room
		if (m_stateMgr->GetContext()->m_entityMgr->GetC_Status(m_map->GetPlayerId())->FlowerCount() == 4)
		{
			RevivalEffect(dt, 0.3f, 1.f);

			m_bossTimer.second = true;
			if (m_stateMgr->GetContext()->m_AudioMgr->IsPlaying("BGM_HallowDream"))
			{
				m_fadeInOut.second = true;
				m_fadeInOut.first = 1.f;
				m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = true;
				m_stateMgr->GetContext()->m_AudioMgr->StopSound("BGM_HallowDream");
				m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("BGM_NightStalker", true);
			}
			if (m_bossTimer.first < 2.f)
			{
				m_map->LoadMap("boss");
				m_map->SaveMap("current");
				m_map->SetViewToPlayer();
			}
		}
		if (m_map->GetMapName() == "boss")
		{
			if (C_Status * boss_status = m_stateMgr->GetContext()->
				m_entityMgr->GetC_Status(m_map->GetBossId()))
			{
				float max_health = static_cast<float>(boss_status->GetMaxHealth());
				float health = static_cast<float>(boss_status->GetHealth());
				boss_health_inner.SetScale(Vector2f(1.5f * health / max_health, 1));
			}
			BossUpdate(dt);
		}

		if (m_fadeInOut.second)
		{
			RevivalEffect(dt, 0.33f, 0.5f);
		}
	}

	if (IsEditingMode && !IsOption)
	{
		int mouse_x = 0, mouse_y = 0;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		m_editor->Update({ mouse_x, mouse_y }, dt);
	}

	if (m_bossTimer.second)
	{
		m_bossTimer.first -= dt;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_shake = true;
	}
	if (m_bossTimer.first < 0.f)
	{
		m_bossTimer.second = false;
	}

	m_dust_particle.Update(dt);
}

void State_Game::Draw()
{
	Vector2f pos = m_map->GetViewCenter();

	if (m_map)
		m_map->Draw(); //Tile

#if _DEBUG
	EntityManager * entityMgr = m_stateMgr->GetContext()->m_entityMgr;
	auto basewindow = m_stateMgr->GetContext()->m_window->GetBaseWindow();
	for (auto& itr : m_stateMgr->GetContext()->m_entityMgr->GetEntityContainer())
	{
		if (const auto * collidable = entityMgr->GetC_Collidable(itr.first))
			basewindow->DebugDraw(collidable->GetAABB()); //Collision Box
		if (const auto * attackable = entityMgr->GetC_Attackable(itr.first))
			basewindow->DebugDraw(attackable->GetAttackRange(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); //Attack range
	}
#endif
	if (IsEditingMode)
		m_editor->Draw(); //Editor
	else
		m_stateMgr->GetContext()->m_GUIMgr->Draw(m_stateMgr->GetContext()->m_window); //GUI

	if (IsOption)
		m_option.Draw();

    if (!m_map->IsWorldMapOpen())
    {
        //Entity
        m_stateMgr->GetContext()->m_systemMgr->Draw(m_stateMgr->GetContext()->m_window);
        m_dust_particle.Draw(m_stateMgr->GetContext()->m_window->GetBaseWindow());
    }
    m_stateMgr->GetContext()->m_systemMgr->GetSystem<S_Renderer>(System::Renderer)->DrawParticles(m_stateMgr->GetContext()->m_window);

	if (m_map->GetMapName() == "boss" && !IsOption)
	{
		BaseWindow * window = m_stateMgr->GetContext()->m_window->GetBaseWindow();
		window->CalculateDirectProjectionDraw(boss_health_outer, GUI_DEPTH);
		window->DirectProjectionDraw(boss_health_outer);
		window->CalculateDirectProjectionDraw(boss_health_inner, GUI_DEPTH);
		window->DirectProjectionDraw(boss_health_inner);
	}
}

// Callback
void State_Game::PlayerControl(EventDetails * details)
{
	if (m_map)
		PlayerMovement(details);
}

void State_Game::PlayerMovement(EventDetails * details)
{
	if (m_map->IsWorldMapOpen())
		return;

	Message m(EntityMessage::Player_Moving, m_map->GetPlayerId());

	if (details->m_name == "Player_Move_Left")
		m.m_direction = Direction::Left;

	else if (details->m_name == "Player_Move_Right")
		m.m_direction = Direction::Right;

	else if (details->m_name == "Player_Idle_Left" ||
		details->m_name == "Player_Idle_Right")
		m.m_type = EntityMessage::Player_Idle;

	else if (details->m_name == "Player_Jump")
		m.m_type = EntityMessage::Player_Jumping;

	else if (details->m_name == "Player_Dash_Right")
	{
		m.m_type = EntityMessage::Player_Dash;
		m.m_direction = Direction::Right;
	}
	else if (details->m_name == "Player_Dash_Left")
	{
		m.m_type = EntityMessage::Player_Dash;
		m.m_direction = Direction::Left;
	}

	else if (details->m_name == "Player_Uppercut")
		m.m_type = EntityMessage::Player_Uppercut;

	else if (details->m_name == "Player_Stomp")
		m.m_type = EntityMessage::Player_Stomp;


	m_stateMgr->GetContext()->m_systemMgr->GetMessageHandler()->Dispatch(m);
}

void State_Game::ToggleInvincible(EventDetails * /*details*/)
{
	if (C_Status * player_status = m_stateMgr->GetContext()->m_entityMgr->
		GetC_Status(m_map->GetPlayerId()))
		player_status->ToggleInvincibleCheat();
}

void State_Game::SetHealth1(EventDetails * details)
{
	if (C_Status * player_status = m_stateMgr->GetContext()->m_entityMgr->
		GetC_Status(m_map->GetPlayerId()))
		player_status->SetSouls(0, 1);
}

void State_Game::ToggleWorldMap(EventDetails* /* details*/)
{
	m_stateMgr->GetContext()->m_AudioMgr->PlaySoundM("Map");
	if (IsOption || IsEditingMode)
		return;
	m_map->ToggleWorldMap();
}

void State_Game::RevivalEffect(float dt, float in, float out)
{
	m_fadeInOut.first -= dt;

	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn && m_fadeInOut.first < 0.f)
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer -= dt * in;
	else if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut)
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer += dt * out;

	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer > 1.f)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = true;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.0f;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.0f;
	}
	else if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer < 0.0f)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = true;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.0f;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.0f;
		m_fadeInOut.second = false;
	}
}

void State_Game::BossUpdate(float dt)
{
	if (m_stateMgr->GetContext()->m_entityMgr->GetC_Position(m_map->GetBossId()) == nullptr)
	{
		m_stateMgr->GetContext()->m_AudioMgr->StopSound("BGM_NightStalker");

		auto status = m_stateMgr->GetContext()->m_entityMgr->GetC_Status(m_map->GetPlayerId());
		if (status->GetSouls().x > status->GetSouls().y)
			m_stateMgr->GetEndingType() = EndingType::HYUNGSEOB;
		else if (status->GetSouls().x < status->GetSouls().y)
			m_stateMgr->GetEndingType() = EndingType::JOOHO;
		else
			m_stateMgr->GetEndingType() = EndingType::MINUI;

		m_stateMgr->SwitchTo(StateType::CREDIT);
	}
}
