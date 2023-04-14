#ifndef BOSS_H
#define BOSS_H
/*!*******************************************************************
\headerfile   Boss.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/20
\brief
			  Header file for Boss class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <utility>
#include "..\Asterisk\Vector.h"

using EntityId = unsigned int;
class SystemManager;

/*!*******************************************************************
\class Boss
\brief
	   Define Boss class has helper functions for boss AI.
********************************************************************/
class Boss
{
private: // Constructors and assginment operator should be private
	/*!*******************************************************************
	\brief Priavte default constructor.
	********************************************************************/
	Boss();
	/*!*******************************************************************
	\brief
		   Private copy constructor.

	\param
		   Reference of this class.
	********************************************************************/
	Boss(const Boss&);
	/*!*******************************************************************
	\brief
		   Private assignment operator.

	\param
		   Reference of this class.
	********************************************************************/
	Boss& operator=(const Boss&);

public:
	enum { Impossible, Dash, Uppercut, Stomp };

public:
	/*!*******************************************************************
	\brief
		   Singleton pattern method.

	\return Boss*
		    Returns pointer to instance of Boss.
   ********************************************************************/
	static Boss* GetInstance();

	/*!*******************************************************************
	\brief
		   Icrease health of boss by time.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\param dt
		   Delta time.
	********************************************************************/
	void Update(SystemManager* systemMgr, EntityId entity, float dt);

	/*!*******************************************************************
	\brief
		   Checks whether entity can act or not.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\return bool
			True if it act.
			False if it can't.
	********************************************************************/
	bool CanAct(SystemManager* systemMgr, EntityId entity) const;

	/*!*******************************************************************
	\brief
		   Checks whether entity is stomping or not.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\return bool
			True if it is.
			False if it isn't.
	********************************************************************/
	bool IsStomping(SystemManager* systemMgr, EntityId entity) const;

	/*!*******************************************************************
	\brief
		   Checks whether entity can dash or not.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\return bool
			True if it can.
			False if it can't.
	********************************************************************/
	bool CanDash(SystemManager* systemMgr, EntityId entity) const;

	/*!*******************************************************************
	\brief
		   Checks whether entity can dash or not.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\return bool
			True if it can.
			False if it can't.
	********************************************************************/
	bool CanUppercut(SystemManager* systemMgr, EntityId entity) const;

	/*!*******************************************************************
	\brief
		   Checks whether entity can hit player with dash or not.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\return int
			Enumerated type specifies what kinds of attack can be used.
	********************************************************************/
	int CanHitPlayer(SystemManager* systemMgr, EntityId entity) const;

	/*!*******************************************************************
	\brief
		   Checks where player is by direction.

	\param entity
		   Position of entity.

	\param player
		   Position of player.

	\return std::pair<bool, bool>
			Return direction.
			First means left, second means up.
	********************************************************************/
	std::pair<bool, bool> GetDirectionToPlayer(Vector3f entity, Vector3f player) const;

private:
	float m_dash_distance; //!< Pre-calculated amount of distance moving by dash.
	float m_uppercut_distance; //!< Pre-calculated amount of distance moving by uppercut.

	float m_time = 0.f; //!< Elapsed time for cheking conditions.
};

#endif