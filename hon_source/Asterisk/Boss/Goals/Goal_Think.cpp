/******************************************************************************/
/*!
\file   Goal_Think.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/18

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <list>
#include <cassert>

#include "Goal_Think.h"

#include "Goal_Attack.h"
#include "Goal_Flee.h"

#include "GoalEvaluation/AttackGoal_Evaluator.h"
#include "GoalEvaluation/FleeGoal_Evaluator.h"

#include "Goal_Types.h"
#include "..\Asterisk\Mathematics.h"
#include "Boss.h"

Goal_Think::Goal_Think()
	: Goal_Composite(goal_think)
{
	const int min = 5;
	const int max = 15;

	using namespace Mathematics;

	double attackBias = static_cast<double>(GetRandomInt(min, max) / 10);
	double fleeBias = static_cast<double>(GetRandomInt(min, max) / 10);

	m_Evaluators.push_back(new AttackGoal_Evaluator(attackBias));
	m_Evaluators.push_back(new FleeGoal_Evaluator(fleeBias));
}

Goal_Think::~Goal_Think()
{
	for (auto& curDes : m_Evaluators)
	{
		delete curDes;
	}
}

void Goal_Think::Activate(SystemManager* systemMgr, EntityId entity)
{
	Arbitrate(systemMgr, entity);
	
	m_status = active;
}

int Goal_Think::Process(SystemManager* systemMgr, EntityId entity, float dt)
{
	ActivateIfInactive(systemMgr, entity);

	int SubgoalStatus = ProcessSubgoals(systemMgr, entity, dt);

	if (SubgoalStatus == completed || SubgoalStatus == failed)
	{
		m_status = inactive;
	}

	return m_status;
}

// Iterates through each goal option to determine which one has the highest desirability.
void Goal_Think::Arbitrate(SystemManager* systemMgr, EntityId entity)
{
	double best = -1;
	Goal_Evaluator* MostDesirable = nullptr;

	for (auto evaluator : m_Evaluators)
	{
		double desirability = evaluator->CalculateDesirability(systemMgr, entity);

		if (desirability >= best)
		{
			best = desirability;
			MostDesirable = evaluator;
		}
	}

	assert(MostDesirable && "<Goal_Think::Arbitrate>: no evaluator selected");
	MostDesirable->SetGoal(this);
}

// Returns true if the goal type passed as a parameter is
// the same as this goal or any of its subgoals
bool Goal_Think::notPresent(int GoalType)const
{
	if (!m_subgoals.empty())
	{
		return m_subgoals.front()->GetType() != GoalType;
	}

	return true;
}

void Goal_Think::AddGoal_Attack()
{
	AddSubgoal(new Goal_Attack());
}

void Goal_Think::AddGoal_Flee()
{
	AddSubgoal(new Goal_Flee());
}