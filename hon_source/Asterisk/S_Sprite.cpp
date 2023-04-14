/******************************************************************************/
/*!
\file   S_Sprite.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Sprite.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_Sprite.h"
#include "C_State.h"
#include "C_Shader.h"

//Ctor
S_Sprite::S_Sprite(SystemManager * systemMgr)
    : S_Base(System::SheetAnimation, systemMgr), m_shaders(nullptr)
{
    Bitset bits;
    bits.set((size_t)Component::State, 1);
    bits.set((size_t)Component::Sprite, 1);
    m_requirement.push_back(bits);

    MessageHandler * handler = m_systemMgr->GetMessageHandler();
    handler->Subscribe(EntityMessage::Request_Shader, this);
}

void S_Sprite::Initialize(ShaderContainer & input_)
{
    RequestShader(input_);
}

void S_Sprite::ReinitMeshes()
{
    for (auto entity : m_entities)
    {
        C_Sprite * c_sprite = m_systemMgr->GetEntityManager()->GetComponent<C_Sprite>(entity, Component::Sprite);
        c_sprite->GetSprite().GetMesh()->Update_Buffers();
    }
}

//Observer method
void S_Sprite::Notify(Message & message)
{
    C_Sprite * c_sprite = m_systemMgr->GetEntityManager()->GetComponent<C_Sprite>(message.m_receiver, Component::Sprite);

    switch (message.m_type)
    {
	    case EntityMessage::Request_Shader:
	    {
	        if (m_systemMgr->GetEntityManager()->GetComponent<C_Shader>(message.m_receiver, Component::Shader))
	        {
	            const auto shader_name = m_systemMgr->GetEntityManager()->GetComponent<C_Shader>(message.m_receiver, Component::Shader)->GetName();
                c_sprite->SetShader(*m_shaders, shader_name);
	        }
	        else
	            assert(!"Unable to find shader component.");
	        break;
	    }
    }
}

void S_Sprite::RequestShader(ShaderContainer & input_)
{
    MessageHandler * handler = m_systemMgr->GetMessageHandler();

    m_shaders = &input_;

    for (auto itr : m_entities)
    {
        Message m(EntityMessage::Request_Shader, itr);
        handler->Dispatch(m);
    }
}