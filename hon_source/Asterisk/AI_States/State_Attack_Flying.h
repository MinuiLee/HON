#ifndef STATE_ATTACK_FLYNG_H
#define STATE_ATTACK_FLYNG_H
/*!*******************************************************************
\headerfile   State_Attack_Flying.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/26
\brief
			  Header file for Attack AI state class for flying enemy.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <unordered_map>

#include "State.h"

#include "..\Vector.h"

using EntityId = unsigned int;

/*!*******************************************************************
\class State_Attack_Flying
\brief
	   Define state where enemy attacks player.
********************************************************************/
class State_Attack_Flying : public State
{
private: // Constructors and assginment operator should be private
	/*!*******************************************************************
	\brief Priavte default constructor.
	********************************************************************/
	State_Attack_Flying() = default;
	/*!*******************************************************************
	\brief
		   Private copy constructor.

	\param
		   Reference of this class.
	********************************************************************/
	State_Attack_Flying(const State_Attack_Flying &);
	/*!*******************************************************************
	\brief
		   Private assignment operator.

	\param
		   Reference of this class.
	********************************************************************/
	State_Attack_Flying & operator=(const State_Attack_Flying &);

	std::unordered_map<EntityId, Vector3f>
		m_information; //!< Container for storing pointers to delete.

public:
	/*!*******************************************************************
	\brief
		   Singleton pattern method.

	\return State_Attack_Flying*
			Returns pointer to instance of State_Attack_Flying.
	********************************************************************/
	static State_Attack_Flying * GetInstance();

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

#endif //STATE_ATTACK_FLYNG_H