/******************************************************************************/
/*!
\file   C_AI.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/01/28

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_AI.h"

#include "AI_States\State_Wandering.h"
#include "AI_States\State_Wandering_Flying.h"
#include "AI_States\GlobalState_Normal.h"
#include "AI_States/State_Bouncing.h"

#include "Boss/Goals/Goal_Think.h"

C_AI::C_AI() : C_Base(Component::AI), m_stateMachine(new StateMachine)
{
    m_stateMachine->SetGlobalState(GlobalState_Normal::GetInstance());
}

C_AI::~C_AI()
{
	if (m_type == EnemyType::Boss)
		delete m_brain;

	delete m_stateMachine;
}

void C_AI::ReadIn(std::stringstream & sstream)
{
    float type;
    sstream >> type;

    switch (static_cast<int>(type))
    {
    case 0:
        m_type = EnemyType::Normal;
        m_stateMachine->SetCurrentState(State_Wandering::GetInstance());
        sstream >> m_chase_range;
        break;

    case 1:
        m_type = EnemyType::Attacking;
        break;

    case 2:
        m_type = EnemyType::Jumping;
        break;
    case 3:
        m_type = EnemyType::Flying;
        m_stateMachine->SetCurrentState(State_Wandering_Flying::GetInstance());
        sstream >> m_chase_range;
        break;

	case 4:
		m_type = EnemyType::Defense;
		m_stateMachine->SetCurrentState(State_Wandering::GetInstance());
		sstream >> m_chase_range;
		break;
	case 5:
		m_type = EnemyType::Steam;
		m_stateMachine->SetCurrentState(State_Wandering::GetInstance());
		break;
	case 6:
		m_type = EnemyType::Bouncing;
		m_stateMachine->SetCurrentState(State_Bouncing::GetInstance());
		sstream >> m_chase_range;
		break;

	case 7:
		m_type = EnemyType::Boss;
		m_brain = new Goal_Think();
		break;

    default:
        m_type = EnemyType::Last;
    };
}