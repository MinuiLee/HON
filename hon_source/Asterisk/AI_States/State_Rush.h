#ifndef STATE_RUSH_H
#define STATE_RUSH_H
/*!*******************************************************************
\headerfile   State_Rush.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/04/10
\brief
			  Header file for rushing enemy AI state class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class State_Rush
\brief
	   Define state when enemy is rushing.
********************************************************************/
class State_Rush : public State
{
private: // Constructors and assginment operator should be private
	/*!*******************************************************************
	\brief Priavte default constructor.
	********************************************************************/
	State_Rush() = default;
	/*!*******************************************************************
	\brief
		   Private copy constructor.

	\param
		   Reference of this class.
	********************************************************************/
	State_Rush(const State_Rush&);
	/*!*******************************************************************
	\brief
		   Private assignment operator.

	\param
		   Reference of this class.
	********************************************************************/
	State_Rush& operator=(const State_Rush&);

public:
	/*!*******************************************************************
	\brief
		   Singleton pattern method.

	\return State_Rush*
			Returns pointer to instance of State_Rush.
	********************************************************************/
	static State_Rush * GetInstance();

	/*!*******************************************************************
	\brief
		   Make entity rush.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Enter(SystemManager* systemMgr, EntityId entity) override;

	/*!*******************************************************************
	\brief
		   If Dash state is finished, then change to wandering state.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param msg
		   Message to handle.

	\return bool
			If message processed properly, return true. If not, false.
	********************************************************************/
	bool HandleMessage(SystemManager* systemMgr, const Message& msg) override;
};

#endif //STATE_DEFENSE_H