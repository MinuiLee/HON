#ifndef GLOBALSTATE_NORMAL_H
#define GLOBALSTATE_NORMAL_H
/*!*******************************************************************
\headerfile   GlobalState_Normal.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/25
\brief
	      Header file for global AI state class for Normal enemy.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class GlobalState_Normal
\brief
       Define global state for common enemies.
********************************************************************/
class GlobalState_Normal : public State
{
private: // Constructors and assginment operator should be private
    /*!*******************************************************************
    \brief Priavte default constructor.
    ********************************************************************/
    GlobalState_Normal() = default;
    /*!*******************************************************************
    \brief
	   Private copy constructor.

    \param 
	   Reference of this class.
    ********************************************************************/
    GlobalState_Normal(const GlobalState_Normal&);
    /*!*******************************************************************
    \brief
	   Private assignment operator.

    \param
	   Reference of this class.
    ********************************************************************/
    GlobalState_Normal & operator=(const GlobalState_Normal&);

public:
    /*!*******************************************************************
    \brief
	   Singleton pattern method.

    \return GlobalState_Normal*
	    Returns pointer to instance of GlobalState_Normal.
    ********************************************************************/
    static GlobalState_Normal* GetInstance();

    /*!*******************************************************************
    \brief
	   Increase timer by dt.

    \param systemMgr
	   Pointer to system manager to use engine interface.

    \param entity
	   Id of entity in this state.

    \param dt
	   Elapsed time for working base on the time.
    ********************************************************************/
    void Update(SystemManager* systemMgr, EntityId entity, float dt) override;
    /*!*******************************************************************
    \brief
	   Process the message sent to this entity.

    \param systemMgr
	   Pointer to system manager to use engine interface.

    \param msg
	   Message to handle.

    \return bool
            If message processed properly, return true. If not, false.
    ********************************************************************/
    bool HandleMessage(SystemManager* systemMgr, const Message & msg) override;
};

#endif //GLOBALSTATE_NORMAL_H