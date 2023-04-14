#ifndef S_ATTACKABLE_H
#define S_ATTACKBALE_H
/******************************************************************************/
/*!
\headerfile   S_Attackable.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for attackable system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"

class S_Attackable : public S_Base
{
public:
	//Ctor
	S_Attackable(SystemManager * sysMgr);

	//S_Base method
	void Update(Window * window, float, bool editor_mode);

	//Observer method
	void Notify(Message & message);

	void SetStateManager(StateManager * stateMgr) { m_stateMgr = stateMgr; }
private:
	//S_Attackable method
	void Damage(const EntityId & attacker, const EntityId target);
	void SendGetAttackedMessage(int attacker, int target);
    void GenerateSoul(const EntityId & dead_enemy);
    bool HasStateAnimation(const EntityId & entity, EntityState state);

	StateManager * m_stateMgr;
};

#endif