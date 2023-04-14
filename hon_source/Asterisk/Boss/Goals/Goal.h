#ifndef GOAL_H
#define GOAL_H
/*!*******************************************************************
\headerfile   Goal.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/11
\brief
			  Header file for base goal class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "../Asterisk/Observer.h"
#include "Goal_Types.h"
#include <stdexcept>

using EntityId = unsigned int;
class SystemManager;

/*!*******************************************************************
\class Goal
\brief
	   Define base goal class.
********************************************************************/
class Goal
{
public:
	enum
	{
		active,
		inactive,
		completed,
		failed
	};

protected:
	int m_type;                 //!< enumerated type specifying the type of goal.
	int m_status = inactive;    //<! enumerated value indicating the goal's status.

	/*!*******************************************************************
	\brief 
	       if status is inactive, then sets it to active and calls Activate().

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void ActivateIfInactive(SystemManager* systemMgr, EntityId entity)
	{
		if (IsInactive()) Activate(systemMgr, entity);
	}
	/*!*******************************************************************
	\brief 
		   if status is failed, then this method sets it to inactive,
		   so that the goal will be reactivated (re-planned).
	********************************************************************/
	void ReactivateIfFailed()
	{
		if (HasFailed()) m_status = inactive;
	}

public:
	/*!*******************************************************************
	\brief Constructor initializes type.
	********************************************************************/
	Goal(int type) : m_type(type) {}

	/*!*******************************************************************
	\brief Virtual destructor.
	********************************************************************/
	virtual ~Goal() {}

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
	\brief Goals can handle messages. 
	       Many don't though, so this defines a default behavior

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param msg
		   message to handle.

	\return bool
			For default behavior, it doesn't handle message, so
			return false.
	********************************************************************/
	virtual bool HandleMessage(SystemManager* /*systemMgr*/, const Message& /*msg*/) { return false; }

	/*!*******************************************************************
	\brief Goal is atomic and cannot aggregate subgoals yet,
		   so this method provide the uniform interface
		   required for the goal hierarchy.
	********************************************************************/
	virtual void AddSubgoal(Goal* /*goal*/) { throw std::runtime_error("Cannot add goals to atomic goals"); }

	/*!*******************************************************************
	\brief
		   Indicates current status is whether completed or not.

	\return bool
			If status is completed, true, not, false.
	********************************************************************/
	bool IsComplete() const { return m_status == completed; }
	/*!*******************************************************************
	\brief
		   Indicates current status is whether active or not.

	\return bool
			If status is active, true, not, false.
	********************************************************************/
	bool IsActive() const { return m_status == active; }
	/*!*******************************************************************
	\brief
		   Indicates current status is whether inactive or not.

	\return bool
			If status is inactive, true, not, false.
	********************************************************************/
	bool IsInactive() const { return m_status == inactive; }
	/*!*******************************************************************
	\brief
		   Indicates current status is whether failed or not.

	\return bool
			If status is failed, true, not, false.
	********************************************************************/
	bool HasFailed() const { return m_status == failed; }
	/*!*******************************************************************
	\brief
		   Getter method for goal type.

	\return int
			Type of this goal.
	********************************************************************/
	int GetType() const { return m_type; }
};

#endif