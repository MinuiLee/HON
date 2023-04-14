#ifndef GOAL_THINK_H
#define GOAL_THINK_H
/*!*******************************************************************
\headerfile   Goal_Composite.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/18
\brief
			  Header file for Think Goal that manages all other goals.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <vector>
#include <string>

#include "Goal_Composite.h"
#include "..\Asterisk\Boss\Goals\GoalEvaluation\Goal_Evaluator.h"

/*!*******************************************************************
\class Goal_Think
\brief
	   Define class to arbitrate between a collection of high level
	   goals, and to process those goals.
********************************************************************/
class Goal_Think : public Goal_Composite
{
private:
	using GoalEvaluators = std::vector<Goal_Evaluator*>;

	GoalEvaluators m_Evaluators; //!< Container for Goal_Evaluators.

public:
	/*!*******************************************************************
	\brief Default constructor.
	********************************************************************/
	Goal_Think();
	/*!*******************************************************************
	\brief Default destructor.
	********************************************************************/
	~Goal_Think();

	/*!*******************************************************************
	\brief 
	       Iterates through each goal evaluator and selects the one that
		   has the highest score as the current goal.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Arbitrate(SystemManager* systemMgr, EntityId entity);

	/*!*******************************************************************
	\brief
		   Checks given goal is whether at the front of the subgoal list
		   or not.

	\param GoalType
		   Type to compare.

	\return bool
			True if goal is not at the front.
			False if it is.
	********************************************************************/
	bool notPresent(int GoalType) const;


	/*!*******************************************************************
	\brief
		   Logic to run when the goal is activated.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Activate(SystemManager* systemMgr, EntityId entity) override;

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
	int Process(SystemManager* systemMgr, EntityId entity, float dt) override;

	/*!*******************************************************************
	\brief
		   No behavior.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Terminate(SystemManager* /*systemMgr*/, EntityId /*entity*/) override {}

	/*!*******************************************************************
	\brief Add strategy subgoal Goal_Attack.
	********************************************************************/
	void AddGoal_Attack();
	/*!*******************************************************************
	\brief Add strategy subgoal Goal_Flee.
	********************************************************************/
	void AddGoal_Flee();
};

#endif