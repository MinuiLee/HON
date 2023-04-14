#ifndef S_COLLISION_H
#define S_COLLISION_H
/******************************************************************************/
/*!
\headerfile   S_Collision.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for collision sytem class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"
#include "Tile.h"

class Map;
class C_Collidable;
class C_Position;
class S_Collision : public S_Base
{
public:
	// Ctor
	S_Collision(SystemManager * systemMgr);

	// S_Base methods
	void Update(Window * window, float, bool);

	// Observer method
	void Notify(Message & /*message*/);

	// S_Collision methods
	void SetMap(Map * map) { m_map = map; }
	Map * GetMap() { return m_map; }

private:
	struct CollisionInfo
	{
		CollisionInfo(float _width = 0.f, float _height = 0.f)
			: area(_width * _height), width(_width), height(_height)
		{
		}

		void Set(float _width, float _height)
		{
			area   = _width * _height;
			width  = _width;
			height = _height;
		}

		float area   = 0.f;
		float width  = 0.f;
		float height = 0.f;
		Direction dir_X;
		Direction dir_Y;
		Tile * currentTile = nullptr;
		int tile_coord_x = 0, tile_coord_y = 0;
		EntityId entity = 0;
	};

	// Entity vs tile collision detection
	void TileCollision(const EntityId & entity) const;
	// Entity vs tile collision response
	void ResolveTileCollision(const CollisionInfo & collision) const;
	// Check colliding tiles
	void CollidingTileUpdate(const EntityId & entity);

	// Entity vs entity collision detection & response
	void EntityCollision(const EntityId & entity);
	void PlateCollision(const EntityId & entity);

	// Raycasting
	void TileCollisionRayCasting(C_Collidable * col, C_Position * pos, const EntityId & entity) const;
	bool TileExist(const Vector2f & pos, Direction dir, const EntityId & entity) const;

	Map * m_map;
};

#endif