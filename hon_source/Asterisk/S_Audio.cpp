/*!*******************************************************************
\file         S_Audio.cpp
\author       Primary: Kim Hyungseob
			  Second: Jooho Jeong
\par          email: hn02415 \@ gmail.com
\date         2019/02/04

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "S_Audio.h"

#include "SystemManager.h"
#include "EntityManager.h"
#include "AudioManager.h"

#include "C_Audio.h"

S_Audio::S_Audio(SystemManager * systemMgr) : S_Base(System::Audio, systemMgr)
{
    Bitset bits;
    bits.set((size_t)Component::Audio, true);
    m_requirement.push_back(bits);

    MessageHandler * handler = m_systemMgr->GetMessageHandler();
    handler->Subscribe(EntityMessage::GetAttacked, this);
    handler->Subscribe(EntityMessage::Dead, this);
    handler->Subscribe(EntityMessage::DeadBySpike, this);
	handler->Subscribe(EntityMessage::Knockdown, this);
    handler->Subscribe(EntityMessage::Uppercut_Start, this);
    handler->Subscribe(EntityMessage::Dash_Start, this);
    handler->Subscribe(EntityMessage::Jumping_Start, this);
    handler->Subscribe(EntityMessage::Stomp, this);
    handler->Subscribe(EntityMessage::Air_Stomp_End, this);
    handler->Subscribe(EntityMessage::GetFlower, this);
	handler->Subscribe(EntityMessage::ShieldBlocking, this);
	handler->Subscribe(EntityMessage::Wall_Break, this);
}

void S_Audio::Update(Window * /*window*/, float, bool /*editor_mode*/)
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    AudioManager * audioMgr = m_systemMgr->GetAudioManager();

	if (audioMgr->IsMute()) return;

    for (auto itr : m_entities)
    {
        C_Audio * audio = entityMgr->GetC_Audio(itr);
	    auto soundList = audio->GetSoundList();

	    for (auto sound : soundList)
	    {
	        std::string name = sound.first;
	        bool shouldPlay = sound.second;

	        if (shouldPlay)
	        {
		        audioMgr->PlaySoundM(name);
		        audio->SetSoundOff(name);
	        }
	    }
    }
}

void S_Audio::Notify(Message& message)
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    C_Audio* audio = entityMgr->GetC_Audio(message.m_receiver);

    switch (message.m_type)
    {
	case EntityMessage::Knockdown:
	{
		audio->SetSoundOn("Spike");
		break;
	}
    case EntityMessage::GetAttacked:
    {
        std::string entity_type = entityMgr->GetEntityType(message.m_receiver);

        if (entity_type == "Player" || entity_type == "Entity_Boss")
            audio->SetSoundOn("Ghost_Hit");
        else 
			audio->SetSoundOn("Broom_Hit");
        
        break;
    }
    case EntityMessage::DeadBySpike:
    {
        audio->SetSoundOn("Spike");
        break;
    }
    case EntityMessage::Dead:
    {
        std::string entity_type = entityMgr->GetEntityType(message.m_receiver);

        if (entity_type == "Object_Soul_purple" || entity_type == "Object_Soul_blue")
            audio->SetSoundOn("Absorb");
        else if (entity_type == "Entity_Statue" || 
            entity_type == "Entity_Book" || 
            entity_type == "Entity_Defense" || 
            entity_type == "Entity_Fork" || 
            entity_type == "Entity_Steam" )
            audio->SetSoundOn("Enemy_Dead");
        else if (entity_type == "Object_Plate")
            audio->SetSoundOn("Candy");
        else if (entity_type == "Entity_Door")
            audio->SetSoundOn("DoorOpening");
            
        break;
    }
    case EntityMessage::Jumping_Start:
    {
        audio->SetSoundOn("Jump");
        break;
    }
    case EntityMessage::Uppercut_Start:
    case EntityMessage::Dash_Start:
    {
        audio->SetSoundOn("Dash");
        break;
    }
    case EntityMessage::Air_Stomp_End:
    {
        if(message.is_action)
            audio->SetSoundOn("High_Air_Stomp");
        else
            audio->SetSoundOn("Air_stomp");
        break;
    }
    case EntityMessage::Stomp:
    {
        audio->SetSoundOn("Stomp");
        break;
    }
    case EntityMessage::GetFlower:
    {
        audio->SetSoundOn("Flower");
        break;
    }
	case EntityMessage::ShieldBlocking:
	{
		audio->SetSoundOn("Shield");
		break;
	}
	case EntityMessage::Wall_Break:
	{
		audio->SetSoundOn("WallBreak");
		break;
	}
    }
}