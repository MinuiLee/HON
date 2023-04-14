/******************************************************************************/
/*!
\file   MapEditor.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <stdlib.h>
#include <filesystem>
#include <algorithm>

#include "MapEditor.h"

#include "S_GameObject.h"
#include "GameObject\Door.h"
#include "GameObject\FragileWall.h"

#include "C_Position.h"
#include "C_Sprite.h"
#include "C_SpriteSheet.h"
#include "C_GameObject.h"
#include "TextureManager.h"
#include "SystemManager.h"
#include "StateManager.h"
#include "AudioManager.h"

MapEditor::MapEditor(StateManager * stateMgr) : m_map(nullptr), m_staMgr(stateMgr)
{
	// NEW
	m_evtMgr = m_staMgr->GetContext()->m_evMgr;
	m_entMgr = m_staMgr->GetContext()->m_entityMgr;

	windowSize = { INITIAL_WIDTH / 2, INITIAL_HEIGHT / 2 };

	TextureManager * txtMgr = m_staMgr->GetContext()->m_textureMgr;
	txtMgr->RequireResource("eraser");
	txtMgr->RequireResource("mouse");
	mouse.Initialize(txtMgr->GetResource("mouse"), windowSize.x, windowSize.y);

	MakeSprite(txtMgr, "Map_save", ENT_SEL_X, ENT_SEL_Y + BOX_GAP * 1.5f, "map_save");
	MakeSprite(
		txtMgr, "Reload_button", ENT_SEL_X + BOX_GAP, ENT_SEL_Y + BOX_GAP * 1.5f, "reload_button");
	MakeSprite(
		txtMgr, "RightClick", ENT_SEL_X + BOX_GAP / 3.f, ENT_SEL_Y + BOX_GAP * 0.8f, "RightClick");
	MakeSprite(
		txtMgr, "LeftClick", ENT_SEL_X - BOX_GAP / 3.f, ENT_SEL_Y + BOX_GAP * 0.8f, "LeftClick");
	MakeSprite(txtMgr, "GameMapButton", ENT_SEL_X + BOX_GAP * 2.f, ENT_SEL_Y + BOX_GAP * 1.5f,
		"MapButton");

	tile = MakeSprite(txtMgr, "tile", 0, 0);
}

void MapEditor::Update(glm::vec2 mouse_pos, float /*dt*/)
{
	if (IsChanged)
	{
		IsChanged = false;
		UpdateObjects();
		m_staMgr->GetContext()->m_systemMgr->Update(m_staMgr->GetContext()->m_window, 0.f, true);
	}

	if (objects.size() != m_entMgr->GetTotalEntityNum()) UpdateObjects();

	zoom = m_staMgr->GetContext()->m_window->GetBaseWindow()->GetZoom();

	if (!m_evtMgr->IsMousePressed(SDL_BUTTON_RIGHT))
		pMouse = mouse_pos;
	else
	{
		m_map->SetViewCenter({ zoom * (pMouse.x - mouse_pos.x), -zoom * (pMouse.y - mouse_pos.y) });
		pMouse = mouse_pos;
	}

	for (auto sp : UI)
		sp.second.Move(mouse_pos.x - pMouse.x, -(mouse_pos.y - pMouse.y));
	for (auto sp : EntityList[kind])
		sp.first.second.Move(mouse_pos.x - pMouse.x, -(mouse_pos.y - pMouse.y));

	mouse.SetPosition({ mouse_pos.x - windowSize.x, -(mouse_pos.y - windowSize.y) });

	ListUpdate();
	SettingMap();
	ModifyingObject();
	SetToTileBased(mouse_pos);

	if (IsMousePressed)
	{
		if (m_mode == Mode::ENTITY) ModifyEntity(zoom * mouse_pos.x, zoom * mouse_pos.y);
		if (m_mode == Mode::TILE) ModifyTiles(zoom * mouse_pos.x, zoom * mouse_pos.y);
	}

	MouseWheelUpdate();
}

Sprite MapEditor::MakeSprite(TextureManager * texMgr, string img, float x, float y, string name)
{
	Sprite s;
	texMgr->RequireResource(img);
	Texture * txt = texMgr->GetResource(img);
	s.SetTexture(*txt);
	s.BuildMesh(txt);
	s.SetPosition({ x, y });

	if (name != " ") UI.emplace_back(make_pair(name, s));

	return s;
}

void MapEditor::Draw()
{
	int size = EntityList[kind].size() >= MAX_ENT_ROW ? MAX_ENT_ROW : static_cast<int>(EntityList[kind].size());
	for (auto i = 0; i < size; i++)
	{
		// EntityListBox
		m_staMgr->GetContext()->m_window->GetBaseWindow()->CalculateDirectProjectionDraw(
			EntityListBox[i].second, GUI_DEPTH);
		m_staMgr->GetContext()->m_window->GetBaseWindow()->DirectProjectionDraw(
			EntityListBox[i].second);
	}
	for (auto sp : UI)
	{
		m_staMgr->GetContext()->m_window->GetBaseWindow()->CalculateDirectProjectionDraw(
			sp.second, GUI_DEPTH);
		m_staMgr->GetContext()->m_window->GetBaseWindow()->DirectProjectionDraw(sp.second);
	}
	for (auto i = EntListStart; i < EntListEnd; i++)
	{
		m_staMgr->GetContext()->m_window->GetBaseWindow()->CalculateDirectProjectionDraw(
			EntityList[kind][i].first.second, GUI_DEPTH);
		m_staMgr->GetContext()->m_window->GetBaseWindow()->DirectProjectionDraw(
			EntityList[kind][i].first.second);
	}
	for (auto i = TileListStart; i < TileListEnd; i++)
	{
		m_staMgr->GetContext()->m_window->GetBaseWindow()->CalculateDirectProjectionDraw(
			TileList[i].second, GUI_DEPTH);
		m_staMgr->GetContext()->m_window->GetBaseWindow()->DirectProjectionDraw(TileList[i].second);
	}

	m_staMgr->GetContext()->m_window->GetBaseWindow()->CalculateDirectProjectionDraw(
		mouse.GetSprite(), GUI_DEPTH);
	m_staMgr->GetContext()->m_window->GetBaseWindow()->DirectProjectionDraw(mouse.GetSprite());

	if (m_mode == Mode::TILE || IsEraseMode)
	{
		m_staMgr->GetContext()->m_window->GetBaseWindow()->CalculateDirectProjectionDraw(
			tile, GUI_DEPTH);
		m_staMgr->GetContext()->m_window->GetBaseWindow()->Draw(tile);
	}
}

void MapEditor::ListUpdate()
{
	float size_inv = 1.f / TileList.size();
	// Tile list update
	for (auto i = TileListStart; i < TileListEnd; i++)
	{
		if (!IsEraseMode && mouse.IsCollidingWith(TileList[i].second, 1.f))
		{
			TileList[i].second.SetScale(size_inv * 1.5f, 1.5f);
			if (m_evtMgr->IsMouseTriggered(SDL_BUTTON_LEFT))
			{
				if (CurrentTileType == TileList[i].first)
				{
					m_mode			= Mode::NOTHING;
					CurrentTileType = " ";
				}
				else
				{
					m_mode			= Mode::TILE;
					CurrentTileType = TileList[i].first;
				}
			}
		}
		else if (CurrentTileType == TileList[i].first)
			TileList[i].second.SetScale(size_inv * 1.5f, 1.5f);
		else
			TileList[i].second.SetScale(size_inv * .8f, .8f);
	}

	if (IsEraseMode)
	{
		CurrentTileType = " ";
		return;
	}

	// Entity list update
	for (int i = static_cast<int>(EntListStart); i < EntListEnd; i++)
	{
		if (mouse.IsCollidingWith(EntityList[kind][i].first.second, { 50.f, 50.f }))
		{
			EntityList[kind][i].first.second.SetScale(
				EntityList[kind][i].second * Vector2f{ 1.2f, 1.2f });
			if (m_evtMgr->IsMouseTriggered(SDL_BUTTON_LEFT))
			{
				m_mode = Mode::ENTITY;

				CreateNewEntity(EntityList[kind][i].first.first);
				CurrentTileType = " ";
				m_staMgr->GetContext()->m_systemMgr->Update(
					m_staMgr->GetContext()->m_window, 0.f, true);
			}
		}
		else
			EntityList[kind][i].first.second.SetScale(EntityList[kind][i].second);
	}
}

void MapEditor::CreateNewEntity(string name)
{
	int id			 = m_entMgr->AddEntity(name);
	C_Position * pos = m_entMgr->GetC_Position(id);
	pos->SetPosition(m_map->GetViewCenter().x, m_map->GetViewCenter().y, 0.f);
	pos->SetInitialPosition(m_map->GetViewCenter().x, m_map->GetViewCenter().y, 0.f);
	m_staMgr->GetContext()->m_systemMgr->InitializeShader(
		&(m_staMgr->GetContext()->m_window->GetBaseWindow()->GetShaders()));

	if (m_entMgr->HasComponent(id, Component::GameObject))
	{
		S_GameObject * s_gameObject =
			m_staMgr->GetContext()->m_systemMgr->GetSystem<S_GameObject>(System::GameObject);
		if (m_entMgr->GetEntityType(id) == "Entity_Door")
			s_gameObject->InitializeObject(id, 2);
		else if (m_entMgr->GetEntityType(id) == "Entity_PlateTower")
			s_gameObject->InitializeObject(id);
	}
}

void MapEditor::SettingMap()
{
	TextureManager * txtMgr = m_staMgr->GetContext()->m_textureMgr;
	bool temp_isEraseMode   = IsEraseMode;
	if (m_evtMgr->IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		mouse.Initialize(txtMgr->GetResource("eraser"), windowSize.x, windowSize.y);
		IsEraseMode = true;
	}
	else
	{
		mouse.Initialize(txtMgr->GetResource("mouse"), windowSize.x, windowSize.y);
		IsEraseMode = false;
	}

	if (temp_isEraseMode != IsEraseMode)
	{
		if (IsEraseMode) CurrentTileType = " ";
		m_mode = Mode::NOTHING;
	}

	if (IsEraseMode) return;

	if (m_evtMgr->IsMouseTriggered(SDL_BUTTON_LEFT))
	{
		if (mouse.IsCollidingWith(UI, "map_save"))
		{
			m_map->SaveMap(m_map->GetMapName());
			m_map->SaveMap("current");
			m_map->SaveTile(m_map->GetMapName());
			CurrentTileType = " ";
			m_mode			= Mode::NOTHING;
		}
		else if (mouse.IsCollidingWith(UI, "reload_button"))
		{
			LoadMap(m_map->GetMapName());
			UpdateObjects();
			m_staMgr->GetContext()->m_systemMgr->Update(
				m_staMgr->GetContext()->m_window, 0.f, true);
			m_mode = Mode::NOTHING;
			std::filesystem::remove("Media/Maps/map_current.json");
		}
		else if (mouse.IsCollidingWith(UI, "RightClick"))
		{
			kind++;
			kind = kind == TOTAL_KIND ? ENTITYY : kind;
			EntListEnd =
				EntityList[kind].size() >= MAX_ENT_ROW ? MAX_ENT_ROW : EntityList[kind].size();
			EntListStart = 0;
			TilePositionUpdate(ENTITY);
		}
		else if (mouse.IsCollidingWith(UI, "LeftClick"))
		{
			kind = kind == ENTITYY ? TOTAL_KIND - 1 : --kind;
			EntListEnd =
				EntityList[kind].size() >= MAX_ENT_ROW ? MAX_ENT_ROW : EntityList[kind].size();
			EntListStart = 0;
			TilePositionUpdate(ENTITY);
		}
		else if (mouse.IsCollidingWith(UI, "MapButton"))
		{
			std::string name;
			TextureManager * texMgr = m_staMgr->GetContext()->m_textureMgr;
			auto & sp				= std::find_if(UI.begin(), UI.end(),
				  [&name](
					  const std::pair<std::string, Sprite> & p) { return p.first == "MapButton"; })
							->second;
			if (m_map->GetMapName() == "game")
			{
				name = "boss";
				Texture * txt = texMgr->GetResource("BossMapButton");
				sp.SetTexture(*txt);
				sp.BuildMesh(txt);

				if(m_staMgr->GetContext()->m_AudioMgr->IsPlaying("BGM_HallowDream"))
					m_staMgr->GetContext()->m_AudioMgr->StopSound("BGM_HallowDream");
				m_staMgr->GetContext()->m_AudioMgr->PlaySoundM("BGM_NightStalker", true);
			}
			else
			{
				name = "game";
				Texture * txt = texMgr->GetResource("GameMapButton");
				sp.SetTexture(*txt);
				sp.BuildMesh(txt);

				if (m_staMgr->GetContext()->m_AudioMgr->IsPlaying("BGM_NightStalker"))
					m_staMgr->GetContext()->m_AudioMgr->StopSound("BGM_NightStalker");
				m_staMgr->GetContext()->m_AudioMgr->PlaySoundM("BGM_HallowDream", true);
			}
			LoadMap(name);
			m_map->SaveMap("current");
			m_map->SetViewToPlayer();
			UpdateObjects();

			m_staMgr->GetContext()->m_systemMgr->Update(
				m_staMgr->GetContext()->m_window, 0.f, true);
			m_mode = Mode::NOTHING;
		}
	}
}

void MapEditor::LoadMap(string mapname)
{
	m_map->LoadMap(mapname);

	m_staMgr->GetContext()->m_systemMgr->InitializeShader(
		&(m_staMgr->GetContext()->m_window->GetBaseWindow()->GetShaders()));
}

void MapEditor::InitializeLists()
{
	TextureManager * txtMgr = m_staMgr->GetContext()->m_textureMgr;
	TileList.clear();
	for (auto & temp : EntityList)
		temp.clear();
	EntityListBox.clear();

	// Initialize entity list
	int num[TOTAL_KIND] = { 0 };
	ENTITY_KIND entkind = ENTITYY;
	Sprite sprite;
	Vector2f scale;
	for (const auto & texture_file : fs::directory_iterator("Media\\Entities\\"))
	{
		if (texture_file.path().stem().string().find("Player") != string::npos) continue;

		if (texture_file.path().stem().string().find("Entity") != string::npos)
			entkind = ENTITYY;
		else if (texture_file.path().stem().string().find("Object") != string::npos)
			entkind = OBJECT;
		else if (texture_file.path().stem().string().find("Extra") != string::npos)
			entkind = EXTRA;

		sprite = MakeSprite(txtMgr, texture_file.path().stem().string(), ENT_SEL_X,
			ENT_SEL_Y - num[entkind] * BOX_GAP);
		scale  = 50.f / sprite.GetSize();

		EntityList[entkind].push_back(
			make_pair(make_pair(texture_file.path().stem().string(), sprite), scale));

		num[entkind]++;
	}

	// Initialize tile list'
	TileSet tileset = m_map->GetTileSet();

	auto i = 0;
	for_each(tileset.begin(), tileset.end(), [&](pair<string, TileTemplate *> element) {
		Sprite temp = element.second->m_sprite;
		temp.Move(TILE_SEL_X, TILE_SEL_Y - i * BOX_GAP);
		TileList.push_back(make_pair(element.first, temp));

		i++;
	});

	TileListEnd = TileList.size() >= MAX_TILE_ROW ? MAX_TILE_ROW : TileList.size();

	for (i = 0; i < TileListEnd; i++)
		MakeSprite(
			txtMgr, "EntityBox", TILE_SEL_X, TILE_SEL_Y - i * BOX_GAP, "tilebox" + to_string(i));


	EntListEnd = num[kind] >= MAX_ENT_ROW ? MAX_ENT_ROW : num[kind];
	EntListStart = 0;

	for (auto j = 0; j < TOTAL_KIND; j++)
	{
		for (i = 0; i < num[j]; i++)
		{
			EntityList[j][i].first.second.SetScale(EntityList[j][i].second);
		}
	}

	for (i = 0; i < EntListEnd; i++)
	{
		EntityListBox.push_back(make_pair("entbox" + to_string(i),
			MakeSprite(txtMgr, "EntityBox", ENT_SEL_X, ENT_SEL_Y - i * BOX_GAP)));
	}
}

void MapEditor::ModifyingObject()
{
	IsMousePressed =
		(m_evtMgr->IsMousePressed(SDL_BUTTON_LEFT) || m_evtMgr->IsMouseTriggered(SDL_BUTTON_LEFT));

	if (IsEraseMode)
	{
		if (!IsMousePressed)
		{
			m_mode   = Mode::TILE;
			CanErase = true;
		}
		for (auto obj : objects)
		{
			if (CanErase &&
				mouse.IsCollidingWith(
					obj.second, zoom, m_map->GetViewCenter().x, m_map->GetViewCenter().y))
			{
				m_mode		   = Mode::ENTITY;
				selectedEntity = obj.first;
				return;
			}
		}
	}
	else
	{
		if (m_mode == Mode::TILE) return;

		if (!IsMousePressed)
		{
			m_mode = Mode::NOTHING;
			UpdateObjects();
		}

		if (m_mode == Mode::ENTITY) return;

		for (auto obj : objects)
		{
			if (mouse.IsCollidingWith(
					obj.second, zoom, m_map->GetViewCenter().x, m_map->GetViewCenter().y))
			{
				m_mode		   = Mode::ENTITY;
				selectedEntity = obj.first;
				return;
			}
		}
	}
}

void MapEditor::UpdateObjects()
{
	objects.clear();
	auto entities = m_entMgr->GetEntityContainer();
	std::for_each(begin(entities), end(entities), [&](std::pair<EntityId, Entity> ent) {
		if (C_Sprite * sp = m_entMgr->GetC_Sprite(ent.first); sp != nullptr)
			objects.push_back(make_pair(ent.first, sp->GetSprite()));
		else if (C_SpriteSheet * sps = m_entMgr->GetC_SpriteSheet(ent.first); sps != nullptr)
			objects.push_back(make_pair(ent.first, *sps->GetSpriteSheet()->GetSprite("Body")));
	});

	selectedEntity  = -1;
	CurrentTileType = " ";
}

void MapEditor::ModifyEntity(float x, float y)
{
	if (IsEraseMode)
	{
		if (CanErase && selectedEntity != static_cast<int>(m_map->GetPlayerId()))
		{
			m_entMgr->RemoveEntity(selectedEntity);
			UpdateObjects();
			CanErase = false;
		}
	}
	else
	{
		if (selectedEntity == -1) return;

		m_entMgr->GetC_Position(selectedEntity)
			->SetPosition(x + m_map->GetViewCenter().x - windowSize.x * zoom,
				-(y - windowSize.y * zoom) + m_map->GetViewCenter().y, 0.f);

		m_staMgr->GetContext()->m_systemMgr->Update(m_staMgr->GetContext()->m_window, 0.f, true);

		SetNumber();

		if (m_entMgr->GetEntityType(selectedEntity) == "Entity_FragileWall")
			if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_R))
				std::any_cast<FragileWall>(m_entMgr->GetC_GameObject(selectedEntity)->Get_Object())
					->RotateWall(m_entMgr, selectedEntity);
	}
}

void MapEditor::SetMap(Map * map)
{
	m_map = map;
	InitializeLists();
}

void MapEditor::ModifyTiles(float x_, float y_)
{
	if (!IsEraseMode && mouse.IsCollidingWith(TileList, CurrentTileType)) return;

	float x = x_ + m_map->GetViewCenter().x - windowSize.x * zoom;
	float y = -(y_ - windowSize.y * zoom) + m_map->GetViewCenter().y;

	x = x / Tile_Size;
	y = y / Tile_Size;

	if (IsEraseMode)
	{
		m_map->RemoveNewTile(static_cast<int>(floor(x)), static_cast<int>(floor(y)));
		CanErase = false;
	}
	else
		m_map->AddNewTile(static_cast<int>(floor(x)), static_cast<int>(floor(y)), CurrentTileType);
}

void MapEditor::MouseWheelUpdate()
{
	if (m_evtMgr->IsMousePressed(SDL_BUTTON_MIDDLE))
		m_staMgr->GetContext()->m_window->GetBaseWindow()->SetZoom(1.f);

	if (mouse.GetSprite().GetPosition().x >= ENT_SEL_X + BOX_GAP &&
		mouse.GetSprite().GetPosition().x <= TILE_SEL_X - BOX_GAP)
	{
		auto base_window = m_staMgr->GetContext()->m_window->GetBaseWindow();
		if (m_evtMgr->GetMouseWheelMove() < 0)
		{
			base_window->SetZoom(base_window->GetZoom() + 0.2f);
			m_evtMgr->SetMouseWheelMove(0);
		}
		else if (m_evtMgr->GetMouseWheelMove() > 0)
		{
			float zoom_ = base_window->GetZoom();
			base_window->SetZoom(zoom_ > 0.6f ? zoom_ - 0.2f : zoom_);
			m_evtMgr->SetMouseWheelMove(0);
		}
		return;
	}

	if (TileList.size() >= MAX_TILE_ROW && mouse.GetSprite().GetPosition().x > TILE_SEL_X - BOX_GAP)
	{
		if (m_evtMgr->GetMouseWheelMove() > 0)
		{
			if (TileListStart < 1) return;
			TileListStart--;
			TileListEnd--;
			m_evtMgr->SetMouseWheelMove(0);
			TilePositionUpdate(TILE);
		}

		if (m_evtMgr->GetMouseWheelMove() < 0)
		{
			if (TileListEnd + 1 > TileList.size()) return;
			TileListStart++;
			TileListEnd++;
			m_evtMgr->SetMouseWheelMove(0);
			TilePositionUpdate(TILE);
		}
	}
	else if (EntityList[kind].size() >= MAX_ENT_ROW &&
		mouse.GetSprite().GetPosition().x<ENT_SEL_X + BOX_GAP)
	{
		if (m_evtMgr->GetMouseWheelMove() > 0)
		{
			if (EntListStart < 1) return;
			EntListStart--;
			EntListEnd--;
			m_evtMgr->SetMouseWheelMove(0);
			TilePositionUpdate(ENTITY);
		}

		if (m_evtMgr->GetMouseWheelMove() < 0)
		{
			if (EntListEnd + 1 > EntityList[kind].size()) return;
			EntListStart++;
			EntListEnd++;
			m_evtMgr->SetMouseWheelMove(0);
			TilePositionUpdate(ENTITY);
		}
		return;
	}
}

void MapEditor::TilePositionUpdate(Mode mode)
{
	if (mode == TILE)
	{
		for (auto i = TileListStart; i < TileListEnd; i++)
		{
			TileList[i].second.SetPosition(TILE_SEL_X, TILE_SEL_Y - (i - TileListStart) * BOX_GAP);
		}
	}
	else if (mode == ENTITY)
	{
		for (auto i = EntListStart; i < EntListEnd; i++)
		{
			EntityList[kind][i].first.second.SetPosition(
				ENT_SEL_X, ENT_SEL_Y - (i - EntListStart) * BOX_GAP);
		}
	}
}

void MapEditor::SetNumber()
{
	if (m_entMgr->GetEntityType(selectedEntity) == "Entity_Door")
	{
		if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_MINUS))
			std::any_cast<Door>(m_entMgr->GetC_GameObject(selectedEntity)->Get_Object())
				->DecreaseCondition();
		if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_EQUALS))
			std::any_cast<Door>(m_entMgr->GetC_GameObject(selectedEntity)->Get_Object())
				->IncreaseCondition();
	}
}

void MapEditor::SetToTileBased(Vector2f mouse_pos)
{
	if (m_mode == Mode::TILE || IsEraseMode)
	{
		float x = mouse_pos.x * zoom + m_map->GetViewCenter().x - windowSize.x * zoom;
		float y = -(mouse_pos.y * zoom - windowSize.y * zoom) + m_map->GetViewCenter().y;

		x = floor(x / Tile_Size);
		y = floor(y / Tile_Size);

		x = x * Tile_Size + Tile_Size / 2;
		y = y * Tile_Size + Tile_Size / 2;

		tile.SetPosition(static_cast<float>(x), static_cast<float>(y));
	}

	if (selectedEntity != -1 && m_mode == Mode::ENTITY)
	{
		C_Position * pos = m_entMgr->GetC_Position(selectedEntity);
		if (pos == nullptr) return;

		float x = pos->GetPosition().x, y = pos->GetPosition().y;

		if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_KP_5))
		{
			y = (floor(y / Tile_Size) * Tile_Size) + Tile_Size / 2;
			x = (floor(x / Tile_Size) * Tile_Size) + Tile_Size / 2;
		}
		else if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_KP_8))
			y += Tile_Size / 2;
		else if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_KP_2))
			y -= Tile_Size / 2;
		else if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_KP_6))
			x += Tile_Size / 2;
		else if (m_evtMgr->IsKeyTriggered(SDL_SCANCODE_KP_4))
			x -= Tile_Size / 2;
		else
			return;

		pos->SetPosition(static_cast<float>(x), static_cast<float>(y), 0.f);

		m_staMgr->GetContext()->m_systemMgr->Update(m_staMgr->GetContext()->m_window, 0.f, true);
		m_staMgr->GetContext()->m_systemMgr->Update(m_staMgr->GetContext()->m_window, 0.f, true);
	}
}