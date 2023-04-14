#ifndef STATE_DEFENSE_H
#define STATE_DEFENSE_H
/*!*******************************************************************
\headerfile   State_Defense.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/03/20
\brief
			  Header file for defensing enemy AI state class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class State_Defense
\brief
	   Define state when enemy is defensing.
********************************************************************/
class State_Defense : public State
{
private: // Constructors and assginment operator should be private
	/*!*******************************************************************
	\brief Priavte default constructor.
	********************************************************************/
	State_Defense() = default;
	/*!*******************************************************************
	\brief
	       Private copy constructor.

	\param
	       Reference of this class.
	********************************************************************/
	State_Defense(const State_Defense&);
	/*!*******************************************************************
	\brief
	       Private assignment operator.

	\param
	       Reference of this class.
	********************************************************************/
	State_Defense& operator=(const State_Defense&);

public:
	/*!*******************************************************************
	\brief
	       Singleton pattern method.

	\return State_Defense*
		    Returns pointer to instance of State_Defense.
	********************************************************************/
	static State_Defense * GetInstance();

	/*!*******************************************************************
	\brief
	       Make entity stop.

	\param systemMgr
	       Pointer to system manager to use engine interface.

	\param entity
	       Id of entity in this state.
	********************************************************************/
	void Enter(SystemManager* systemMgr, EntityId entity) override;

	/*!*******************************************************************
	\brief
	       Make enemy defense direction of player.

	\param systemMgr
	       Pointer to system manager to use engine interface.

	\param entity
	       Id of entity in this state.

	\param dt
	       Elapsed time for working base on the time.
	********************************************************************/
	void Update(SystemManager * systemMgr, EntityId entity, float) override;
};

#endif STATE_DEFENSE_H