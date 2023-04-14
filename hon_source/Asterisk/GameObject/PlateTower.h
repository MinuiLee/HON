#ifndef PLATETOWER_H
#define PLATETOWER_H
/*!*******************************************************************
\headerfile   PlateTower.h
\author Jooho Jeong
\par    email: jooho556 \@ gmail.com
\date   2019/02/17
\brief
		Header file for plate tower enemy.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "../Asterisk/Observer.h"
#include "../Asterisk/Vector.h"

class EntityManager;
class PlateTower
{
public:
    PlateTower() {}
    void Initialize(unsigned int id, EntityManager * entityMgr);

    void Update(float dt);
    void HandleMessage(Message & msg);

private:
    void Shoot();

    EntityManager * m_entityMgr = nullptr;
    unsigned int m_id = 0;
    float m_reloading_time = 0.f;
};

#endif