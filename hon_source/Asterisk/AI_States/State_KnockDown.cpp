/******************************************************************************/
/*!
\file   State_KnockDown.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/01/28

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_KnockDown.h"

#include "StateMachine.h"

#include "..\SystemManager.h"
#include "..\EntityManager.h"

#include "..\C_AI.h"

#define TIME_TO_STAND 2.f

State_KnockDown * State_KnockDown::GetInstance()
{
    static State_KnockDown instance;

    return &instance;
}

void State_KnockDown::Enter(SystemManager * systemMgr, EntityId entity)
{
    auto AI_component = systemMgr->GetEntityManager()->GetComponent<C_AI>(entity, Component::AI);
    AI_component->SetTimer(0.f);
    AI_component->SetCondition(TIME_TO_STAND);
}

void State_KnockDown::Update(SystemManager * systemMgr, EntityId entity, float)
{
    auto AI_component = systemMgr->GetEntityManager()->GetComponent<C_AI>(entity, Component::AI);

    if (AI_component->GetCondition() < AI_component->GetTimer())
        AI_component->GetStateMachine()->Revert_To_Previous_State(systemMgr, entity);
}