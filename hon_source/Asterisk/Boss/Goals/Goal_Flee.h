#ifndef GOAL_FLEE_H
#define GOAL_FLEE_H
/*!*******************************************************************
\headerfile   Goal_Flee.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/26
\brief
			  Header file for fleeing goal class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Goal_Composite.h"

/*!*******************************************************************
\class Goal_Flee
\brief
	   Define class represents strategy goal flee that trying to
	   avoid player.
********************************************************************/
class Goal_Flee : public Goal_Composite
{
public:
	/*!*******************************************************************
	\brief Default constructor.
	********************************************************************/
	Goal_Flee() : Goal_Composite(goal_flee) {}

	/*!*******************************************************************
	\brief
		   Checks position of player and moves opposite to player.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Activate(SystemManager* systemMgr, EntityId entity) override;

	/*!*******************************************************************
	\brief
		   Process subgoal and handle result.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\param dt
		   Delta time.

	\return int
			Enumerated value that represents result of processing.
	********************************************************************/
	int Process(SystemManager* systemMgr, EntityId entity, float dt) override;

	/*!*******************************************************************
	\brief
		   Save status of final result.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Terminate(SystemManager* systemMgr, EntityId entity) override;

	/*!*******************************************************************
	\brief
		   Check whether goal is failed by given message.
		   If message is get attacked, then status become failed.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param msg
		   message to handle.

	\return bool
			If message is get attacked, then return true.
			If it is not, because it's unnecessary message, so return false.
	********************************************************************/
	bool HandleMessage(SystemManager* systemMgr, const Message& msg) override;

private:
	int m_count = 0; //!< When succeed to attack player, increase it to flee.
	bool m_left = true; //!< Saves direction to move after successing to hit player.
};


#endif