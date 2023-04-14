#ifndef S_BASE_H
#define S_BASE_H
/******************************************************************************/
/*!
\headerfile   S_Base.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for base system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <vector>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "ECS_Types.h"
#include "Observer.h"
#include "Window.h"

using EntityId			= unsigned int;
using EntityVector		= std::vector<EntityId>;
using SystemRequirement = std::vector<Bitset>;
using EntityTimer		= std::unordered_map<EntityId, float>;

class SystemManager;
class S_Base : public Observer
{
public:
    //Ctor & Dtor
    S_Base(const System & id, SystemManager * systemMgr)
	: m_type(id), m_systemMgr(systemMgr), m_playerID(0)
    {
	    srand(static_cast<unsigned int>(time(NULL)));
    }
    virtual ~S_Base() { Purge(); }

	// Entity managing
	bool AddEntity(const EntityId id);
	bool HasEntity(const EntityId & id);
	bool RemoveEntity(const EntityId & id);

	// Component checking
	bool FitsRequitrment(const Bitset & bits);

    void Purge();
    void PurgeEntities()
    {
	    m_entities.clear();
    }
    const System & GetType() const { return m_type; }
    unsigned int GetPlayerID() { return m_playerID; }
    void SetPlayerID(unsigned int ID) { m_playerID = ID; }
    std::pair<EntityState, bool> RequireState(const EntityId & entity) const;

    bool GetRandomBool(int seed);
	int GetRandomInt(int range);

	virtual void Initialize() { std::cout << "No initialize." << std::endl; }
	virtual void Update(Window*, float, bool) = 0;

protected:
	void ShieldBlockMessage(const EntityId & target, const EntityId & attacker);
    bool IsEntityOnAir(const EntityId & entity);
    bool  IsCollidingFragileWall(const EntityId & entity);
    System m_type;
    EntityVector m_entities;

    SystemRequirement m_requirement;
    unsigned int m_playerID;

	SystemManager * m_systemMgr;
};

#endif