#ifndef GOAL_COMPOSITE_H
#define GOAL_COMPOSITE_H
/*!*******************************************************************
\headerfile   Goal_Composite.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/11
\brief
			  Header file for base composite goal class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <list>
#include "Goal.h"

/*!*******************************************************************
\class Goal_Composite
\brief
	   Define base composite goal class.
********************************************************************/
class Goal_Composite : public Goal
{
private:
	using SubgoalList = std::list<Goal*>;

protected:
	SubgoalList m_subgoals; //!< Composite goals may have any number of subgoals.

	/*!*******************************************************************
	\brief
		   Processes any subgoals that may be present.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\return int
			Enumerated value that represents result of processing subgoal
	********************************************************************/
	int ProcessSubgoals(SystemManager* systemMgr, EntityId entity, float dt);

	/*!*******************************************************************
	\brief
		   Passes the message to the front-most subgoal.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\return bool
			If list is empty, return fals.
			If not, return reuslt of processing message by front subgoal.
	********************************************************************/
	bool ForwardMessageToFrontMostSubgoal(SystemManager* systemMgr, const Message& msg);

public:
	/*!*******************************************************************
	\brief Constructor initializes type.
	********************************************************************/
	Goal_Composite(int type) : Goal(type) {}

	/*!*******************************************************************
	\brief Virtual destructor.
	********************************************************************/
	virtual ~Goal_Composite() {}

	/*!*******************************************************************
	\brief
		   Logic to run when the goal is activated.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	virtual void Activate(SystemManager* systemMgr, EntityId entity) = 0;

	/*!*******************************************************************
	\brief
		   Logic to run each update-step.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\param dt
		   Delta time.

	\return int
			Enumerated value that represents result of processing.
	********************************************************************/
	virtual int Process(SystemManager* systemMgr, EntityId entity, float dt) = 0;

	/*!*******************************************************************
	\brief
		   Logic to run when the goal is satisfied.
		   (typically used to switch off any active steering behaviors)

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	virtual void Terminate(SystemManager* systemMgr, EntityId entity) = 0;

	/*!*******************************************************************
	\brief If a child class of Goal_Composite does not define
	       a message handler, the default behavior is 
		   to forward the message to the front-most subgoal.
	********************************************************************/
	virtual bool HandleMessage(SystemManager* systemMgr, const Message& msg) { return ForwardMessageToFrontMostSubgoal(systemMgr, msg); }

	/*!*******************************************************************
	\brief Adds a subgoal to the front of the subgoal list
	********************************************************************/
	void AddSubgoal(Goal* goal);

	/*!*******************************************************************
	\brief Iterates through the subgoals and calls each one's Terminate
		   method before deleting the subgoal and
		   removing it from the subgoal list

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void RemoveAllSubgoals(SystemManager* systemMgr, EntityId entity);
};

#endif