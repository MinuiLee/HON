/*!*******************************************************************
\headerfile C_AI.h <>
\author Primary: Minui Lee
	    Second: Kim Hyungseob
\par    email: alsdml0727 \@ gmail.com
\brief
	    Header file for AI Component Class.
\date   2018/12/14
\copyright
All content  2018 DigiPen (USA) Corporation, all rights reserved
********************************************************************/
#ifndef C_AI_H
#define C_AI_H

#include <cassert>

#include "C_Base.h"
#include "Vector.h"

#include "AI_States\StateMachine.h"


enum class EnemyType
{
    Normal,
    Attacking,
    Jumping,
    Flying,
	Defense,
	Steam,
	Bouncing,
	Boss,
    Last
};

class Goal_Think;

/*!*******************************************************************
\class C_AI
\brief
       Holds the ai target and event happening range.
********************************************************************/
class C_AI : public C_Base
{
public:
	/*!*******************************************************************
	\brief Default Constructor with base class constructor.
	********************************************************************/
    C_AI();

	/*!*******************************************************************
    \brief Destructor that removes allocated pointer to state machine.
    ********************************************************************/
    ~C_AI();

	/*!*******************************************************************
	\brief
		   Overrided method from C_Base class that reads configuration
		   file for AI component.

	\param sstream
		   stringstream that has information of ai component
	********************************************************************/
    void ReadIn(std::stringstream & sstream) override;

	/*!*******************************************************************
    \brief
	       Getter method for enemy type.

    \return EnemyType
	        type of enemy
    ********************************************************************/
	EnemyType GetType() const { return m_type; }

	/*!*******************************************************************
	\brief
		   Getter method for attack range.

	\return float
		    attack range
	********************************************************************/
	float GetAttackRange() const { return m_attack_range; }

	/*!*******************************************************************
	\brief
		   Getter method for chase range.

	\return float
		    chase range
	********************************************************************/
	float GetChaseRange() const { return m_chase_range; }

	/*!*******************************************************************
	\brief
	       Getter method for state machine.

	\return StateMachine*
		    Pointer to state machine.
	********************************************************************/
	StateMachine * GetStateMachine() const { return m_stateMachine; }

    /*!*******************************************************************
	\brief
	       Setter method for timer.

	\param timer
	       New start point for timer.
	********************************************************************/
    void SetTimer(float timer) { m_timer = timer; }
    /*!*******************************************************************
	\brief
	       Getter method for timer.

	\return float
		    Passed time before set timer.
	********************************************************************/
    float GetTimer() { return m_timer; }

    /*!*******************************************************************
	\brief
	       Setter method for condition.

	\param condition
	       New value will be used for condition.
	********************************************************************/
    void SetCondition(float condition) { m_condition = condition; }
    /*!*******************************************************************
	\brief
	       Getter method for condition.

	\return float
		    The value of current condition.
	********************************************************************/
    float GetCondition() { return m_condition; }

    /*!*******************************************************************
	\brief
	       Setter method for direction.

	\param direction
	       New direction to set.
	********************************************************************/
    void SetDirection(Vector2i direction) { m_direction = direction; }
    /*!*******************************************************************
	\brief
	       Getter method for direction.

	\return Vector2i
		    Direction that this entity is moving.
	********************************************************************/
    Vector2i GetDirection() const { return m_direction; }

	/*!*******************************************************************
	\brief
		   Getter method for think goal taht is brain of goal-driven agent

	\return Goal_Think*
			Brain of AI.
	********************************************************************/
	Goal_Think* GetBrain() { return m_brain; }

	void AddElapsedTime(float dt){m_elapsedTime += dt;}
	float GetElapsedTime() { return m_elapsedTime; }
	void SetElapsedTime(float time) { m_elapsedTime = time; }

private:
    // Inputs from entity file
	EnemyType m_type = EnemyType::Normal;   //!< type of enemy AI
	float m_attack_range = 0.f; //!< needed distance to attack between entity and player
	float m_chase_range  = 0.f; //!< needed distance to chase between entity and player

    // Variables that will be used in S_AI
	StateMachine * m_stateMachine = nullptr; //!< State machine that this entity uses. It will be allocated in S_AI.

    float m_condition = 0.f;      //!< Any kind of condition for AI.
    float m_timer = 0.f;          //!< To check conditions based on time.
	float m_elapsedTime = 0.f;    //!< Total time passed in certain state
    Vector2i m_direction{ 0, 0 }; //!< (is moving right, is moving up), 1: yes, 0: Stand, -1: opposite or {x value, y value} or target position

	Goal_Think* m_brain = nullptr; //!< Main logic for goal-driven agent.
};

#endif
