/******************************************************************************/
/*!
\file   Map.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Map.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "StateManager.h"
#include "Window.h"

#include "C_Position.h"
#include "C_Sprite.h"
#include "C_Collidable.h"
#include "C_Attackable.h"
#include "C_GameObject.h"
#include "C_Status.h"
#include "C_State.h"
#include "C_SpriteSheet.h"
#include "GameObject/Door.h"
#include "GameObject/FragileWall.h"

#include "S_GameObject.h"

#include "AudioManager.h"
#include "SystemManager.h"

#include <cassert>

namespace
{
	constexpr int CAMERA_VELOCITY_SCALE = 4;
	constexpr int CAMERA_HALF_WIDTH		= 100;
} // namespace

using namespace rapidjson;

Map::Map(SharedContext * context, const std::string & file)
	: m_context(context), m_mapSize(0, 0), m_playerId(0)
{
	MakeSprite(m_playerSign, "PlayerSign");
	MakeSprite(m_blueflowerSign, "BlueflowerSign");
	MakeSprite(m_redflowerSign, "RedflowerSign");
	MakeSprite(m_whiteflowersign, "WhiteflowerSign");
	MakeSprite(m_yellowflowerSign, "YellowflowerSign");
	MakeSprite(m_cemeterySign, "CemeterySign");
	MakeSprite(m_escmSign, "ESCM");

	LoadTileSet();
	LoadMap(file);

	C_Position * player_pos = m_context->m_entityMgr->GetC_Position(m_playerId);
	m_viewcenter			= player_pos->GetPosition();
}

bool Map::LoadMap(const std::string & file)
{
	m_mapname = file != "current" ? file : m_mapname;
	FILE * fp = nullptr;

	if (file != "notneed")
	{
		m_mapID = atoi(file.c_str());

		std::string filename = "Media/Maps/map_" + file + ".json";

		errno_t err = fopen_s(&fp, filename.c_str(), "rb");
		if (err != 0) return false;

		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		map.ParseStream(is);
	}

	const Value & Entities = map["ENTITIES"];
	assert(Entities.IsArray());

	EntityManager * EntityMgr = m_context->m_entityMgr;

	if (file == "boss")
	{
		C_Status * status = m_context->m_entityMgr->GetC_Status(m_playerId);
		m_playerSoul.x	= status->GetSouls().x;
		m_playerSoul.y	= status->GetSouls().y;
	}
	
	EntityMgr->Purge();
	m_context->m_systemMgr->PurgeEntities();

	EntityId cemeteryId = 0;

	m_blueflowerSign.second   = false;
	m_redflowerSign.second	= false;
	m_whiteflowersign.second  = false;
	m_yellowflowerSign.second = false;
	m_cemeterySign.second	 = false;

	rapidjson::SizeType size = Entities.Size();
	for (SizeType i = 0; i < size; i++)
	{
		auto entity = Entities[i].GetObject();

		int id = 0;

		std::string name = entity.FindMember("Name")->value.GetString();
		id				 = EntityMgr->AddEntity(name);

		if (EntityMgr->HasComponent(id, Component::Position))
		{
			C_Position * tran = EntityMgr->GetC_Position(id);

			Vector3f position = Vector3f(entity.FindMember("X_pos")->value.GetDouble(),
				entity.FindMember("Y_pos")->value.GetDouble(),
				entity.FindMember("Z_pos")->value.GetDouble());

			tran->SetPosition(position);
			tran->SetInitialPosition(position);

			// Update AABB & AttackRange position
			if (C_Collidable * col = EntityMgr->GetC_Collidable(id))
			{
				col->SetPosition(tran->GetPosition());
				if (C_Attackable * attack = EntityMgr->GetC_Attackable(id))
					attack->SetTopleft(col->GetAABB().m_top,
						col->GetAABB().m_left + col->GetAABB().m_width + attack->GetOffset());
			}

			if (file == "current" && EntityMgr->GetEntityType(id) == "Object_Cemetery")
				cemeteryId = id;

			if (name == "Object_Flower_Blue")
			{
				m_blueflowerSign.second = true;
				m_blueflowerSign.first.SetPosition(tran->GetPosition());
			}
			else if (name == "Object_Flower_Red")
			{
				m_redflowerSign.second = true;
				m_redflowerSign.first.SetPosition(tran->GetPosition());
			}
			else if (name == "Object_Flower_White")
			{
				m_whiteflowersign.second = true;
				m_whiteflowersign.first.SetPosition(tran->GetPosition());
			}
			else if (name == "Object_Flower_Yellow")
			{
				m_yellowflowerSign.second = true;
				m_yellowflowerSign.first.SetPosition(tran->GetPosition());
			}
			else if (name == "Object_Cemetery")
			{
				m_cemeterySign.second = true;
				m_cemeterySign.first.SetPosition(tran->GetPosition());
			}
		}

		if (EntityMgr->HasComponent(id, Component::GameObject) && entity.HasMember("Condition"))
		{
			auto s_gameObject = m_context->m_systemMgr->GetSystem<S_GameObject>(System::GameObject);

			int condition = entity.FindMember("Condition")->value.GetInt();

			s_gameObject->InitializeObject(id, condition);
		}
		else if (EntityMgr->HasComponent(id, Component::GameObject) &&
			entity.HasMember("Direction"))
		{
			FragileWall * wall =
				std::any_cast<FragileWall>(EntityMgr->GetC_GameObject(id)->Get_Object());

			int direction = entity.FindMember("Direction")->value.GetInt();

			wall->SetDirection(EntityMgr, id, direction);
		}

		if (name == "Player")
		{
			m_playerId = id;
			m_context->m_systemMgr->SetPlayerID(id);
			if (file == "boss")
			{
				C_Status * status = m_context->m_entityMgr->GetC_Status(m_playerId);
				status->SetSouls(m_playerSoul.x, m_playerSoul.y);
			}
		}
		else if (name == "Entity_PlateTower")
		{
			S_GameObject * game_object_system =
				m_context->m_systemMgr->GetSystem<S_GameObject>(System::GameObject);
			game_object_system->InitializeObject(id);
		}
		else if (file == "current" && name == "Object_Vase")
		{
			EntityMgr->GetC_SpriteSheet(id)->GetSpriteSheet()->SetCurrentAnimation(
				"Body", entity.FindMember("Flower")->value.GetString(), true, true);

			std::string state_str = entity.FindMember("Flower")->value.GetString();
			if (state_str != "Idle") EntityMgr->GetC_State(id)->SetState(EntityState::Other);
		}
		else if (name == "Entity_Boss")
		{
			m_bossId = id;
		}
	}

	m_context->m_systemMgr->InitializeShader(&(m_context->m_window->GetBaseWindow()->GetShaders()));

	if (file == "current")
	{
		C_Status * status = EntityMgr->GetC_Status(m_playerId);
		if (!m_blueflowerSign.second) status->PutFlowerToVase(2);
		if (!m_redflowerSign.second) status->PutFlowerToVase(1);
		if (!m_whiteflowersign.second) status->PutFlowerToVase(4);
		if (!m_yellowflowerSign.second) status->PutFlowerToVase(3);

		status->SetSouls(map["PurpleSoul"].GetInt(), map["BlueSoul"].GetInt());

		if (map["FlowerEarned"].GetInt() != 0)
		{
			C_Position * cemetaryPos = EntityMgr->GetC_Position(cemeteryId);
			EntityMgr->GetC_Position(m_playerId)->SetPosition(cemetaryPos->GetPosition());
		}
	}

	if (file != "notneed") fclose(fp);

	if (file != "current") ConstructTiles(file);
	return true;
}

void Map::SaveMap(std::string name)
{
	FILE * fp;
	std::string filename = "Media/Maps/map_" + name + ".json";
	errno_t err			 = fopen_s(&fp, filename.c_str(), "wb");
	if (err != 0) return;

	char Buffer[65536];

	FileWriteStream os(fp, Buffer, sizeof(Buffer));
	PrettyWriter<FileWriteStream> writer(os);

	writer.StartObject();

	EntityManager * EntityMgr = m_context->m_entityMgr;
	if (name == "current")
	{
		C_Status * playerstat = EntityMgr->GetC_Status(m_playerId);
		writer.Key("PurpleSoul");
		writer.Int(playerstat->GetSouls().x);
		writer.Key("BlueSoul");
		writer.Int(playerstat->GetSouls().y);
		writer.Key("FlowerEarned");
		writer.Int(playerstat->FlowerCount());
	}

	writer.Key("ENTITIES");
	writer.StartArray();

	auto entities = EntityMgr->GetEntityContainer();

	for (auto & ent : entities)
	{
		writer.StartObject();
		writer.Key("Name");
		writer.String(ent.second.first.first.c_str());
		writer.Key("X_pos");
		writer.Int(static_cast<int>(EntityMgr->GetC_Position(ent.first)->GetPosition().x));
		writer.Key("Y_pos");
		writer.Int(static_cast<int>(EntityMgr->GetC_Position(ent.first)->GetPosition().y));
		writer.Key("Z_pos");
		writer.Int(static_cast<int>(EntityMgr->GetC_Position(ent.first)->GetPosition().z));
		if (EntityMgr->GetEntityType(ent.first) == "Entity_Door")
		{
			writer.Key("Condition");
			Door * door = std::any_cast<Door>(EntityMgr->GetC_GameObject(ent.first)->Get_Object());
			writer.Int(door->GetCondition());
		}
		else if (EntityMgr->GetEntityType(ent.first) == "Entity_FragileWall")
		{
			writer.Key("Direction");
			FragileWall * wall =
				std::any_cast<FragileWall>(EntityMgr->GetC_GameObject(ent.first)->Get_Object());
			writer.Int(wall->GetDirectionAsInt());
		}
		if (name == "current")
		{
			if (EntityMgr->GetEntityType(ent.first) == "Object_Vase")
			{
				writer.Key("Flower");
				SpriteSheet * sheet = static_cast<SpriteSheet *>(
					EntityMgr->GetC_SpriteSheet(ent.first)->GetSpriteSheet());
				writer.String(sheet->GetCurrentAnimation().c_str());
			}
		}
		writer.EndObject();
	}

	writer.EndArray();
	writer.EndObject();
	fclose(fp);
}

void Map::PurgeMap()
{
	m_context->m_entityMgr->Purge();

	// Tiles
	for (auto & tilemap : m_tilemap)
		delete tilemap.second;
	for (auto & tileset : m_tileset)
		delete tileset.second;
}

void Map::Draw()
{
	DrawTiles();
	if (IsWorldMap)
	{
		int middleX = static_cast<int>(m_worldSize.m_left + m_worldSize.m_width / 2);
		int middleY = static_cast<int>(m_worldSize.m_top - m_worldSize.m_height / 2);

		BaseWindow * BSWindow = m_context->m_stateMgr->GetContext()->m_window->GetBaseWindow();

		SetView(Vector2f(middleX * Tile_Size, middleY * Tile_Size));
		BSWindow->SetZoom(m_worldZoom);
		if (m_blueflowerSign.second) BSWindow->Draw(m_blueflowerSign.first);
		if (m_redflowerSign.second) BSWindow->Draw(m_redflowerSign.first);
		if (m_whiteflowersign.second) BSWindow->Draw(m_whiteflowersign.first);
		if (m_yellowflowerSign.second) BSWindow->Draw(m_yellowflowerSign.first);
		if (m_cemeterySign.second) BSWindow->Draw(m_cemeterySign.first);

		BSWindow->CalculateDirectProjectionDraw(m_escmSign.first, GUI_DEPTH);
		BSWindow->DirectProjectionDraw(m_escmSign.first);
		BSWindow->Draw(m_playerSign.first);
	}
}

void Map::SemiStaticDraw(void) { DrawTiles(); }

void Map::SetCamera(float dt, bool edit_mode)
{
	View * view = m_context->m_window->GetBaseWindow()->GetView();

	if (!edit_mode)
	{
		EntityManager * entityMgr = m_context->m_entityMgr;
		C_Position * playerC_pos  = entityMgr->GetC_Position(m_playerId);

		if (!playerC_pos) return;

		Vector2f player_pos = playerC_pos->GetPosition();
		Vector2f view_pos   = view->GetCenter();

		Vector2f camera_velocity(0, 0);

		float x_diff = player_pos.x - view_pos.x;
		if (x_diff > CAMERA_HALF_WIDTH)
			camera_velocity.x = x_diff - CAMERA_HALF_WIDTH;
		else if (x_diff < -CAMERA_HALF_WIDTH)
			camera_velocity.x = x_diff + CAMERA_HALF_WIDTH;

		float y_diff = player_pos.y - view_pos.y;
		if (y_diff > CAMERA_HALF_WIDTH)
			camera_velocity.y = y_diff - CAMERA_HALF_WIDTH;
		else if (y_diff < -CAMERA_HALF_WIDTH)
			camera_velocity.y = y_diff + CAMERA_HALF_WIDTH;

		glm::vec2 delta(camera_velocity.x * dt * CAMERA_VELOCITY_SCALE,
			camera_velocity.y * dt * CAMERA_VELOCITY_SCALE);
		m_viewcenter += delta;
		view->SetDelta(delta);
	}

	view->SetCenter(m_viewcenter.x, m_viewcenter.y);
}

void Map::IsMapEditingMode(bool Is) { IsMapEditor = Is; }
void Map::ToggleWorldMap()
{
	IsWorldMap		   = !IsWorldMap;
	Vector3f playerPos = m_context->m_entityMgr->GetC_Position(m_playerId)->GetPosition();
	if (IsWorldMap)
	{
		m_preViewCenter = m_viewcenter;
		m_playerSign.first.SetPosition(playerPos);
		m_escmSign.first.SetPosition(505.f, -355.f);
	}
	else
	{
		m_context->m_window->GetBaseWindow()->GetView()->SetCenter(playerPos);
		SetView(playerPos);
		m_context->m_stateMgr->GetContext()->m_window->GetBaseWindow()->SetZoom(1.f);
	}
}
bool Map::IsWorldMapOpen() { return IsWorldMap; }
void Map::SetViewCenter(Vector2f center) { m_viewcenter += center; }

void Map::SetViewToPlayer()
{
	View * view			= m_context->m_window->GetBaseWindow()->GetView();
	Vector2f player_pos = m_context->m_entityMgr->GetC_Position(m_playerId)->GetPosition();

	view->SetCenter(player_pos);
	m_viewcenter = player_pos;
}

Vector2f Map::GetViewCenter() { return m_viewcenter; }

Tile * Map::GetTile(int x, int y)
{
	TileCoordinate key(x, y);
	auto tile = m_tilemap.find(key);
	return tile != m_tilemap.end() ? tile->second : nullptr;
}

void Map::SaveTile(std::string name)
{
	FILE * fp;
	std::string filename = "Media/Maps/tiles_" + name + ".json";
	errno_t err			 = fopen_s(&fp, filename.c_str(), "wb");
	if (err != 0) return;

	char Buffer[65536];

	FileWriteStream os(fp, Buffer, sizeof(Buffer));
	PrettyWriter<FileWriteStream> writer(os);

	writer.StartObject();
	writer.Key("TILES");
	writer.StartArray();

	for (auto & tile : m_tilemap)
	{
		writer.StartObject();
		writer.Key("type");
		writer.String(tile.second->tile->m_tile_name.c_str());
		writer.Key("x");
		writer.Int(tile.first.first);
		writer.Key("y");
		writer.Int(tile.first.second);
		writer.EndObject();
	}

	SetWorldZoom();

	writer.EndArray();
	writer.EndObject();
	fclose(fp);
}

void Map::ConstructTiles(std::string name)
{
	for (auto & tilemap : m_tilemap)
		delete tilemap.second;
	m_tilemap.clear();

	FILE * fp = nullptr;
	rapidjson::Document tile_doc;
	std::string filename = "Media/Maps/tiles_" + name + ".json";

	errno_t err = fopen_s(&fp, filename.c_str(), "rb");
	if (err != 0) std::cout << "cannot open tile json file\n";

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	tile_doc.ParseStream(is);

	const Value & tiles = tile_doc["TILES"];
	assert(tiles.IsArray());

	for (SizeType i = 0; i < tiles.Size(); i++)
	{
		auto tile		= tiles[i].GetObject();
		Tile * new_tile = new Tile;

		auto tileset   = m_tileset.find(tile.FindMember("type")->value.GetString());
		new_tile->tile = tileset->second;

		int x_ = tile.FindMember("x")->value.GetInt(), y_ = tile.FindMember("y")->value.GetInt();

		new_tile->tile->m_sprite.SetDepth(TILES_DEPTH);
		if (!m_tilemap.emplace(std::make_pair(x_, y_), new_tile).second)
		{
			std::cerr << "Cannot empalce tile : " << new_tile->tile->m_tile_name << " (" << x_
					  << ", " << y_ << ")" << std::endl;
			delete new_tile;
			continue;
		}
		else
		{
			if (i == 0)
			{
				m_worldSize.m_left   = static_cast<float>(x_);
				m_worldSize.m_top	 = static_cast<float>(y_);
				m_worldSize.m_height = 0.f;
				m_worldSize.m_width  = 0.f;
			}
			// set worldMap min max
			SetWorldSize(x_, y_);
		}
	}
	SetWorldZoom();

	fclose(fp);
}

bool Map::AddNewTile(int x, int y, std::string type)
{
	Tile * new_tile = new Tile;

	auto tileset   = m_tileset.find(type);
	new_tile->tile = tileset->second;
	return (m_tilemap.emplace(std::make_pair(x, y), new_tile).second);
}

bool Map::RemoveNewTile(int x, int y)
{
	for (auto & tile : m_tilemap)
	{
		if (tile.first.first == x && tile.first.second == y)
		{
			TileCoordinate temp(tile.first);
			m_tilemap.erase(temp);
			return true;
		}
	}
	return false;
}

void Map::SetFlowerSignExist(bool b, int index)
{
	switch (index)
	{
	case 1:
		m_redflowerSign.second = b;
		break;
	case 2:
		m_blueflowerSign.second = b;
		break;
	case 3:
		m_yellowflowerSign.second = b;
		break;
	case 4:
		m_whiteflowersign.second = b;
		break;
	default:
		break;
	}
}

void Map::LoadTileSet()
{
	std::ifstream inFile("Config\\Tiles.cfg");
	if (!inFile.is_open())
	{
		std::cerr << "Cannot open Tile.cfg file\n";
		return;
	}

	unsigned int tile_id = 0;

	std::string line;
	while (std::getline(inFile, line))
	{
		if (line[0] == '|') continue;

		TileTemplate * new_tile_type = new TileTemplate(m_context->m_textureMgr, "Tiles", tile_id);

		std::stringstream keystream(line);
		keystream >> *new_tile_type;

		if (!m_tileset.emplace(new_tile_type->m_tile_name, new_tile_type).second)
		{
			std::cerr << "Cannot empalce tile type: " << new_tile_type->m_tile_name << std::endl;
			delete new_tile_type;
			continue;
		}
		++tile_id;
	}

	inFile.close();
}

void Map::DrawTiles()
{
	FloatRect view_space = m_context->m_window->GetBaseWindow()->GetViewSpace();
	BaseWindow * window  = m_context->m_window->GetBaseWindow();

	for (auto & tile : m_tilemap)
	{
		int x_unit = tile.first.first;
		int y_unit = tile.first.second;

		// Constantly change sprite position
		tile.second->tile->m_sprite.SetPosition(
			static_cast<float>(x_unit * Tile_Size + Tile_Size / 2),
			static_cast<float>(y_unit * Tile_Size + Tile_Size / 2));
		FloatRect tile_rect(static_cast<float>(x_unit * Tile_Size),
			static_cast<float>((y_unit + 1) * Tile_Size), static_cast<float>(Tile_Size),
			static_cast<float>(Tile_Size));

		if (view_space.Intersects(tile_rect)) window->Draw(tile.second->tile->m_sprite);
	}
}

void Map::SetWorldSize(int x, int y)
{
	float x_ = static_cast<float>(x);
	float y_ = static_cast<float>(y);
	if (m_worldSize.m_left >= x_) m_worldSize.m_left = x_;

	if (m_worldSize.m_width <= x_ - m_worldSize.m_left)
		m_worldSize.m_width = x_ - m_worldSize.m_left;

	if (m_worldSize.m_top <= y_) m_worldSize.m_top = y_;

	if (m_worldSize.m_height <= m_worldSize.m_top - y_)
		m_worldSize.m_height = m_worldSize.m_top - y_;
}

void Map::SetWorldZoom()
{
	BaseWindow * BSWindow = m_context->m_stateMgr->GetContext()->m_window->GetBaseWindow();
	float xRatio		  = (m_worldSize.m_width * Tile_Size) / BSWindow->GetInitialWindowSize().x,
		  yRatio = (m_worldSize.m_height * Tile_Size) / (BSWindow->GetInitialWindowSize().y);
	m_worldZoom  = xRatio > yRatio ? xRatio : yRatio;
	m_worldZoom += 0.5f;
}

void Map::MakeSprite(std::pair<Sprite, bool> & sp, std::string name)
{
	m_context->m_textureMgr->RequireResource(name);
	Texture * txt = m_context->m_textureMgr->GetResource(name);
	sp.first.SetTexture(*txt);
	sp.first.BuildMesh(txt);
	sp.first.SetDepth(GUI_DEPTH);

	sp.second = true;
}