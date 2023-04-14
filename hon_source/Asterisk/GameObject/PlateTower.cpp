/******************************************************************************/
/*!
\file   PlateTower.cpp
\author Jooho Jeong
\par    email: jooho556 \@ gmail.com
\date   2019/02/17

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PlateTower.h"
#include "../Asterisk/EntityManager.h"
#include "../Asterisk/SystemManager.h"
#include "../Asterisk/C_Position.h"
#include "../Asterisk/C_State.h"
#include "../Asterisk/C_Status.h"

namespace {
    constexpr float RELOADING = 2.f;
    constexpr float HURT_DEALY = 1.f;
}

void PlateTower::Initialize(unsigned int id, EntityManager * entityMgr)
{
    m_entityMgr = entityMgr;
    m_id = id;
}

void PlateTower::Update(float dt)
{
    C_State * state = m_entityMgr->GetC_State(m_id);
    
    EntityState entity_state = state->GetState();
    if (entity_state == EntityState::Hurt)
    {
        state->UpdateDelay(dt);
        if (state->GetDelay() > HURT_DEALY)
        {
            state->ResetDelay();
            state->SetAction(false);
            Message msg(EntityMessage::Became_Idle, m_id);
            m_entityMgr->GetSystemManager()->GetMessageHandler()->Dispatch(msg);
        }
    }
    else if(entity_state == EntityState::Dead)
    {
        Message msg(EntityMessage::IsDeadAnimationEnd, m_id);
        m_entityMgr->GetSystemManager()->GetMessageHandler()->Dispatch(msg);
        if (msg.is_action)
            m_entityMgr->RemoveEntity(m_id);
    }
    else
    {
        m_reloading_time += dt;
        if (m_reloading_time > RELOADING)
        {
            Shoot();
            m_reloading_time = 0.f;
        }
    }
}

void PlateTower::HandleMessage(Message & msg)
{
    switch (msg.m_type)
    {
    case EntityMessage::GetAttacked:
    {
        m_reloading_time = 0.f;
        break;
    }
    }
}

void PlateTower::Shoot()
{
    C_Position * pos = m_entityMgr->GetC_Position(m_id);
    m_entityMgr->ReserveEntity("Object_Plate", pos->GetPosition());
}
