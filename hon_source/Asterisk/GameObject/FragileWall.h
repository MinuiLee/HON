/*!*******************************************************************
\headerfile   FragileWall.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/03/18
\brief
			  Header file for Fragile wall object.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#ifndef FRAGILEWALL_H
#define FRAGILEWALL_H

#include "../Direction.h"

using EntityId = unsigned int;

class SystemManager;
class EntityManager;
struct Message;

/*!*******************************************************************
\class FragileWall
\brief
	   Define behavior of Game Object Fragile Wall.
********************************************************************/
class FragileWall
{
public:
	/*!*******************************************************************
	\brief Default constructor
	********************************************************************/
	FragileWall() = default;

	/*!*******************************************************************
	\brief
		   Returns direction.

	\return Direction
			Current direction of wall.
	********************************************************************/
	Direction GetDirection() const { return m_direction; }
	/*!*******************************************************************
	\brief
		   Returns direction as integer.

	\return int
			Current direction of wall.
	********************************************************************/
	int GetDirectionAsInt() const;
	/*!*******************************************************************
	\brief
		   Set direction by given Direction.

	\param direction
		   New direction for wall.
	********************************************************************/
	void SetDirection(Direction direction) { m_direction = direction; }

	/*!*******************************************************************
	\brief
		   Set direction by given unsigned int.

	\param entityMgr
		   Pointer to EntityManager.

	\param entity
		   Id of this entity.

	\param direction
		   New direction for wall.
	********************************************************************/
	void SetDirection(EntityManager* entityMgr, EntityId entity, int direction);
	/*!*******************************************************************
	\brief 
		   Change direction of wall 90 degree to clockwise direction.

	\param entityMgr
		   Pointer to EntityManager.

	\param entity
		   Id of this entity.
	********************************************************************/
	void RotateWall(EntityManager* entityMgr, EntityId entity);

	/*!*******************************************************************
	\brief
		   If correct message is given, then remove this entity.

	\param systemMgr
		   Pointer to SystemManager.

	\param message
		   Pointer to Message.
	********************************************************************/
	void HandleMessage(SystemManager* systemMgr, Message* message);

private:
	Direction m_direction = Direction::Left; //!< Reprsents facing direction of wall.

private:
	/*!*******************************************************************
	\brief
		   Rotate spritesheet and collision box.

	\param entityMgr
		   Pointer to EntityManager.

	\param entity
		   Id of this entity.

	\param angle
		   Amount of rotation.
	********************************************************************/
	void RotateSpriteAndCollision(EntityManager* entityMgr, EntityId entity);
};

#endif //FRAGILEWALL_H