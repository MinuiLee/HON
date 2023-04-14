#ifndef GOAL_DASH_H
#define GOAL_DASH_H
/*!*******************************************************************
\headerfile   Goal_Dash.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/22
\brief
			  Header file for dahsing goal class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Goal.h"

/*!*******************************************************************
\class Goal_Dash
\brief
	   Define class makes entity dash to player.
********************************************************************/
class Goal_Dash : public Goal
{
public:
	/*!*******************************************************************
	\brief Default constructor.
	********************************************************************/
	Goal_Dash(bool left) : Goal(goal_dash), m_left(left) {}

	/*!*******************************************************************
	\brief
		   Makes entity dash to given direction.

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

private:
	bool m_left; //!< Saves direction of dash.
};

#endif