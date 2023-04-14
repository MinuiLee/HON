/*!*******************************************************************
\headerfile   S_AI.h
\author Minui Lee
\par    email: alsdml0727 \@ gmail.com
\date   2018/12/14
\brief
		Header file for AI System Class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#ifndef S_AI_H
#define S_AI_H

#include "S_Base.h"
/*!*******************************************************************
\class S_AI
\brief
	   Simple AI following technique.
********************************************************************/
class S_AI : public S_Base
{
public:
	/*!*******************************************************************
	\brief
	       Constructor for AI system class
	\param systemMgr
	       SystemManager where register this system.
	********************************************************************/
	S_AI(SystemManager * sysMgr);

	/*!*******************************************************************
	\brief
		   Observer method that takes message
	\param message
		   nothing
    ********************************************************************/
	void Notify(Message & message) override;
	/*!*******************************************************************
	\brief
		   Updtae AI event using the AI component
	\param dt
		   update AI event by dt
	********************************************************************/
	void Update(Window * window, float dt, bool editor_mode);

private:
	/*!*******************************************************************
	\brief
		   send AI message
	\param entity
		   which entity to send message to
	********************************************************************/
	void SendMessages(const EntityId & entity);
	/*!*******************************************************************
	\brief
		   setting random value which makes look more like AI
	\param entityid
		   which entity to set random value
	********************************************************************/
	void Patrol(int entityid);
};

#endif
