/******************************************************************************/
/*!
\file   GlobalState_Normal.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/01/25

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "GlobalState_Normal.h"

#include "StateMachine.h"
#include "State_KnockDown.h"

#include "..\SystemManager.h"
#include "..\EntityManager.h"

#include "..\C_AI.h"
#include "..\C_Position.h"

GlobalState_Normal * GlobalState_Normal::GetInstance()
{
    static GlobalState_Normal instance;

    return &instance;
}

void GlobalState_Normal::Update(SystemManager * systemMgr, EntityId entity, float dt)
{
    auto entityMgr = systemMgr->GetEntityManager();

    //----------------------- Increase Timer ------------------------------------------------------
    auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);
    AI_component->SetTimer(AI_component->GetTimer() + dt);
}

bool GlobalState_Normal::HandleMessage(SystemManager * systemMgr, const Message & msg)
{
    auto entityMgr = systemMgr->GetEntityManager();
    auto AI_component = entityMgr->GetComponent<C_AI>(msg.m_receiver, Component::AI);
	const auto type = AI_component->GetType();

    switch (msg.m_type)
    {
    case EntityMessage::GetAttacked:
        if (type == EnemyType::Normal)
        {
            if (auto statemachine = AI_component->GetStateMachine(); !statemachine->IsInState(State_KnockDown::GetInstance()))
                statemachine->ChangeState(systemMgr, msg.m_receiver, State_KnockDown::GetInstance());
            else statemachine->GetCurrentState()->Enter(systemMgr, msg.m_receiver);
        }
        return true;
    }

    return false;
}