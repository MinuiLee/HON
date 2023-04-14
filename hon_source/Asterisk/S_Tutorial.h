#pragma once
#include "S_Base.h"
#include "../Game/State_Game.h"

class S_Tutorial : public S_Base
{
public:
    S_Tutorial(SystemManager * sysMgr);
    void Initialize(Tutorial * tutorial, Map * map);

    //S_Base methods
    void Update(Window * window, float dt);

    //Observer method
    void Notify(Message & message);

private:
    bool FirstEnemyOnSight();
    bool FirstDoorOnSight();
    bool FirstEnemyBelowOnSight();
    bool FirstHillOnSight();

    Tutorial * m_tutorial = nullptr;
    Map * m_map = nullptr;
};