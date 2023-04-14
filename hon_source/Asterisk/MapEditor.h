#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H
/******************************************************************************/
/*!
\headerfile   MapEditor.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for Map Editor class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "../Game/Mouse.h"
#include "Map.h"
#include "StateManager.h"
#include "EntityManager.h"

namespace
{
	constexpr auto ENT_SEL_X	  = -600.f; // entity selection x pos
	constexpr auto ENT_SEL_Y	  = 250.f;  // entity selection y pos;
	constexpr auto BOX_GAP		  = 70.f;   // gap between entity selection boxes;
	constexpr auto TILE_SEL_X	 = 600.f;   // tile selection x pos
	constexpr auto TILE_SEL_Y	 = 350.f;   // tile selection y pos
	constexpr auto MAX_TILE_ROW = 11;		// tile selection x pos
	constexpr auto MAX_ENT_ROW	= 9;		// ENT selection x pos
};										 
using namespace std;
using EntitySprite = pair<string, Sprite>;

enum Mode
{
	NOTHING,
	ENTITY,
	TILE
};

enum ENTITY_KIND
{
	ENTITYY,
	OBJECT,
	EXTRA,
	TOTAL_KIND
};

class MapEditor
{
public:
	MapEditor(StateManager * stateMgr);
	void SetMap(Map * map);

	void Update(glm::vec2 mouse_pos, float dt);
	void Draw();
	void LoadMap(string mapname = "notneed");
	void SetIsChanged(bool b) { IsChanged = b; }
private:
	Sprite MakeSprite(TextureManager * texMgr, string img, float x, float y, string name = " ");
	void ListUpdate();
	void CreateNewEntity(string name);
	void SettingMap();
	void InitializeLists();
	void ModifyingObject();
	void UpdateObjects();
	void ModifyEntity(float x, float y);
	void ModifyTiles(float x, float y);
	void MouseWheelUpdate();
	void TilePositionUpdate(Mode mode);
	void SetNumber();
	void SetToTileBased(Vector2f mousePos);

	Mouse mouse;
	Vector2f pMouse;

	Vector2u windowSize;
	StateManager * m_staMgr;
	EventManager * m_evtMgr;
	EntityManager * m_entMgr;
	Map * m_map;

	vector<EntitySprite> UI;
	vector<EntitySprite> EntityListBox;
	vector<pair<EntitySprite, Vector2f>> EntityList[TOTAL_KIND];
	vector<EntitySprite> TileList;
	vector<pair<EntityId, Sprite>> objects;
	Sprite tile;

	int selectedEntity = 0;
	string CurrentTileType;

	bool IsMousePressed = false, CanErase = true, IsChanged = true, IsEraseMode = false;
	size_t TileListStart = 0, TileListEnd = MAX_TILE_ROW, kind = ENTITYY, EntListStart = 0, EntListEnd = MAX_ENT_ROW;
	Mode m_mode = Mode::NOTHING;
	float zoom = 1.f;
};

#endif