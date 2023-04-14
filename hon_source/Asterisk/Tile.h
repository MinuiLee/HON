#ifndef TILE_H
#define TILE_H
/******************************************************************************/
/*!
\headerfile   Tile.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2019/01/16
\brief
		Header file for tile class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <map>
#include <unordered_map>
#include <string>

#include "Sprite.h"
#include "TextureManager.h"
#include "Direction.h"

enum { Tile_Size = 48, Tile_Sprite_Size = 52 };

class Map;
//A type of tile - only 1 intance for each
struct TileTemplate
{
    TileTemplate(TextureManager * texMgr, const std::string & texture_name, unsigned int id)
        : m_texMgr(texMgr), m_texture_name(texture_name), m_id(id)
    {
        m_texMgr->RequireResource(texture_name);
        m_sprite.SetTexture(*m_texMgr->GetResource(texture_name));
        m_sprite.BuildMesh(m_texMgr->GetResource(texture_name));

        FloatRect rect(static_cast<float>(id * Tile_Sprite_Size + id * 2 + 1), 1, Tile_Sprite_Size, Tile_Sprite_Size);
        m_sprite.SetTextureRect(rect);
    }

    ~TileTemplate()
    {
        m_texMgr->ReleaseResource(m_texture_name);
    }

    friend std::stringstream & operator>>(std::stringstream & sstream, TileTemplate & tile)
    {
		sstream >> tile.m_tile_name >> tile.m_friction.x >> tile.m_friction.y;

        std::string direction;
        sstream >> direction;

        if (direction == "Top")
            tile.m_collision_y = Direction::Up;
        else if (direction == "Bottom")
            tile.m_collision_y = Direction::Down;
        else
            tile.m_collision_y = Direction::None;

        sstream >> direction;

        if (direction == "Left")
            tile.m_collision_x = Direction::Left;
        else if (direction == "Right")
            tile.m_collision_x = Direction::Right;
        else
            tile.m_collision_x = Direction::None;

		sstream >> direction;
		if (direction == "true") tile.m_isPassable = true;

		sstream >> tile.is_deadly;

        return sstream;
    }

    TextureManager* m_texMgr;
    std::string m_texture_name;
    Sprite m_sprite;
    unsigned int m_id;
    std::string m_tile_name;
	int is_deadly;

    Direction m_collision_x = Direction::None;
    Direction m_collision_y = Direction::None;
    
    Vector2f m_friction; //damping

	bool m_isPassable = false;
};

//Actual tile object - have the pointer to a tile template
struct Tile
{
    TileTemplate * tile;
};

using TileCoordinate = std::pair<int, int>;//same as (unit x-coordinate, unit y_coordinate)
using TileSet = std::unordered_map<std::string, TileTemplate*>;
using TileMap = std::map<TileCoordinate, Tile*>;

#endif