/******************************************************************************/
/*!
\file   FleeGoal_Evaluator.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/26

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "FleeGoal_Evaluator.h"

#include "..\Goal_Think.h"

#include "..\Asterisk\SystemManager.h"
#include "..\Asterisk\EntityManager.h"

#include "..\Asterisk\C_Status.h"

#include "..\Asterisk\Mathematics.h"

double FleeGoal_Evaluator::CalculateDesirability(SystemManager* systemMgr, EntityId entity)
{
	auto entityMgr = systemMgr->GetEntityManager();

	auto entityHP = entityMgr->GetC_Status(entity)->GetHealth();
	auto playerHP = entityMgr->GetC_Status(systemMgr->GetPlayerID())->GetSouls();

	auto diff = playerHP.x + playerHP.y  - entityHP;

	return diff < 0 ? 0 : static_cast<double>(Mathematics::GetRandomInt(0, 10)) / 10;
}

void FleeGoal_Evaluator::SetGoal(Goal_Think* brain)
{
	brain->AddGoal_Flee();
}