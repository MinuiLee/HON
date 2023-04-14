#ifndef GOAL_STOMP_H
#define GOAL_STOMP_H
/*!*******************************************************************
\headerfile   Goal_Stomp.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/23
\brief
			  Header file for stomping goal class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Goal.h"

/*!*******************************************************************
\class Goal_Stomp
\brief
	   Define class makes entity stomp to player.
********************************************************************/
class Goal_Stomp : public Goal
{
public:
	/*!*******************************************************************
	\brief Default constructor.
	********************************************************************/
	Goal_Stomp() : Goal(goal_stomp) {}

	/*!*******************************************************************
	\brief
		   Makes entity stomp.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Activate(SystemManager* systemMgr, EntityId entity) override;

	/*!*******************************************************************
	\brief
		   Just checks status and activate if inactivated.

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
		   Changes status to completed.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Terminate(SystemManager* systemMgr, EntityId entity) override;
};

#endif