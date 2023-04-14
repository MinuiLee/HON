#ifndef DOOR_H
#define DOOR_H
/*!*******************************************************************
\headerfile   Door.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/31
\brief
              Header file for Game object Door class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "DoorUI.h"
#include "../Text.h"

using EntityId = unsigned int;

class SystemManager;
class Draw;
struct Message;

/*!*******************************************************************
\class DoorUI
\brief
	   Define behavior of Game Object Door.
********************************************************************/
class Door
{
public:
	/*!*******************************************************************
	\brief Default constructor that initialize Text UI.
	********************************************************************/
	Door();

	/*!*******************************************************************
	\brief
		   Set condition of Door.

	\param condition
		   Condition to set.
	********************************************************************/
    void Initialize(int condition);

	/*!*******************************************************************
	\brief
		   Draw Text UI.

	\param window
		   Pointer to Window that provides drawing interface.
	********************************************************************/
	void DrawUI(Window * window);

	/*!*******************************************************************
	\brief
		   Checks the distance between player and door.
		   If player is close enough, then set information to UI.

	\param systemMgr
		   Pointer to SystemManager.

	\param entity
		   EntityId of this Door.

	\param dt
	       Elapsed time for working base on the time.

	\return bool
			If player is close enough, then return true.
			If not, return false.
	********************************************************************/
    bool IsPlayerCloseToDoor(SystemManager* systemMgr, EntityId entity, float dt);

	/*!*******************************************************************
	\brief
		   Handle paying soul message.
		   If player has more than 1 soul, then makes player pay the soul.

	\param systemMgr
		   Pointer to SystemManager.

	\param message
		   Pointer to Message to tell which soul to pay.

	\return bool
			If player pay soul completely, then return true.
			If not, return false.
	********************************************************************/
    bool IsSoulEnoughToOpenDoor(SystemManager* systemMgr, Message* message);

	/*!*******************************************************************
	\brief
		   Returns m_condition.

	\return int&
			Returns reference to m_condition to get or change.
	********************************************************************/
    int GetCondition() const { return m_condition; }

	/*!*******************************************************************
	\brief Increases the number of condition.
	********************************************************************/
	void IncreaseCondition();

	/*!*******************************************************************
	\brief Decreases the number of condition.
	********************************************************************/
	void DecreaseCondition();

private:
    int m_condition = 2; //!< Neede souls to open this Door.
	DoorUI* m_UI = DoorUI::GetInstance(); //!< Pointer to DoorUI.

	Text m_condition_UI; //!< Text represents the number of souls need to be paid.
};

#endif //DOOR_H