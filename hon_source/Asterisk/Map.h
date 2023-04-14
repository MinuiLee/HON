#ifndef MAP_H
#define MAP_H
/******************************************************************************/
/*!
\headerfile   Map.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for Map class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>

#include "SharedContext.h"
#include "Vector.h"
#include "Sprite.h"
#include "Tile.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

class Window;

struct Entity_info;
struct Map_info;

constexpr Vector2f WORLDMAP_CENTER = { -193.f, -1057.f };

class Map
{
public:
	Map(SharedContext * context, const std::string & file);
	~Map() { PurgeMap(); }

	bool LoadMap(const std::string & file = "notneed");
	void SaveMap(std::string name); // Save current map to json file
	void Draw();
	void SemiStaticDraw(void);

	// Unloading
	void PurgeMap();

	// Game updating
	void SetCamera(float dt, bool edit_mode);

    unsigned int GetPlayerId() { return m_playerId; }
    unsigned int GetBossId() { return m_bossId; }
	const Vector2f & GetMapSize() { return m_mapSize; }
	Window * GetWindow() { return m_context->m_window; }

	rapidjson::Document & GetDocument() { return map; }

	// MapEditingMode
	void IsMapEditingMode(bool Is);
	void ToggleWorldMap();
	bool IsWorldMapOpen();
	void SetViewCenter(Vector2f center);
	void SetViewToPlayer();
	Vector2f GetViewCenter();

	int GetMapID() { return m_mapID; }
	void SetFixedView(bool b) { FixedView = b; }
	Tile * GetTile(int x, int y);
	TileSet GetTileSet() { return m_tileset; }
	void SaveTile(std::string name);
	void ConstructTiles(std::string name);
	bool AddNewTile(int x, int y, std::string type);
	bool RemoveNewTile(int x, int y);

	void SetView(const Vector2f & center)
	{
		m_viewcenter.x = center.x;
		m_viewcenter.y = center.y;
	}

	void SetFlowerSignExist(bool b, int index);
	std::string GetMapName() { return m_mapname; }
private:
	void LoadTileSet();
	void DrawTiles();
	void SetWorldSize(int x, int y);
	void SetWorldZoom();
	void MakeSprite(std::pair<Sprite, bool> & sp, std::string name);

	TileSet m_tileset;
	TileMap m_tilemap;

	SharedContext * m_context;

	Vector2f m_mapSize;

    unsigned int m_playerId;
    unsigned int m_bossId;
	int m_mapID = 0;

	rapidjson::Document map;
	rapidjson::Document current;

	bool IsMapEditor		 = false;
	bool IsWorldMap			 = false;
	bool FixedView			 = false;
	Vector2f m_viewcenter	= { 0.f, 0.f };
	Vector2f m_preViewCenter = { 0.f, 0.f };

	int last_map = 0;
	std::pair<Sprite, bool> m_playerSign, m_blueflowerSign, m_redflowerSign, m_whiteflowersign,
		m_yellowflowerSign, m_cemeterySign, m_escmSign;

	FloatRect m_worldSize;
	float m_worldZoom;

	std::string m_mapname;
	Vector2i m_playerSoul;
};
#endif