#ifndef STATE_BOUNCING_H
#define STATE_BOUNCING_H
/*!*******************************************************************
\headerfile   State_Bouncing.h
\author       Minui Lee
\par          email: alsdml0727 \@ gmail.com
\date         2019/04/03
\brief
			  Header file for Attack AI state class for bouncing enemy.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <unordered_map>

#include "State.h"

#include "..\Vector.h"

using EntityId = unsigned int;

/*!*******************************************************************
\class State_Bouncing
\brief
	   Define state where enemy attacks player.
********************************************************************/
class State_Bouncing : public State
{
private: // Constructors and assginment operator should be private
	/*!*******************************************************************
	\brief Priavte default constructor.
	********************************************************************/
	State_Bouncing() = default;
	/*!*******************************************************************
	\brief
		   Private copy constructor.

	\param
		   Reference of this class.
	********************************************************************/
	State_Bouncing(const State_Bouncing &);
	/*!*******************************************************************
	\brief
		   Private assignment operator.

	\param
		   Reference of this class.
	********************************************************************/
	State_Bouncing & operator=(const State_Bouncing &);

	std::unordered_map<EntityId, Vector3f>
		m_information; //!< Container for storing pointers to delete.

public:
	/*!*******************************************************************
	\brief
		   Singleton pattern method.

	\return State_Bouncing*
			Returns pointer to instance of State_Attack_Flying.
	********************************************************************/
	static State_Bouncing * GetInstance();

	/*!*******************************************************************
	\brief
		   Store information that represents original position of entity.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.
	********************************************************************/
	void Enter(SystemManager * systemMgr, EntityId entity) override;
	/*!*******************************************************************
	\brief
		   Make entity fly around player.
		   Sometimes, it attacks player.

	\param systemMgr
		   Pointer to system manager to use engine interface.

	\param entity
		   Id of entity in this state.

	\param dt
		   Elapsed time for working base on the time.
	********************************************************************/
	void Update(SystemManager * systemMgr, EntityId entity, float) override;

private:
	Vector2f m_initPosition;
	bool m_bool = false;
};

#endif STATE_BOUNCING_H