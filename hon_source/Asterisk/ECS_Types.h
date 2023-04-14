#ifndef ECS_TYPES_H
#define ECS_TYPES_H
/******************************************************************************/
/*!
\headerfile   ECS_Types.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for lists of components and systems.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <bitset>

#define N_COMPONENT 16

using Bitset = std::bitset<N_COMPONENT>;

enum class Component
{
	Position = 0,
	Sprite,
	SpriteSheet,
	State,
	RigidBody,
	Controllable,
	Collidable,
	AI,
	Status,
	Attackable,
	Shader,
	Audio,
	Event,
    GameObject
};

enum class System
{
	Renderer = 0,
	Movement,
	State,
	Control,
	Collision,
	SheetAnimation,
	Sprite,
	AI,
	Attackable,
	Audio,
	Event,
    GameObject,
    Tutorial,
	Particle
};

#endif