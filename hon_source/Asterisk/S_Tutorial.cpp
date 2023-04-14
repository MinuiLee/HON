#include "S_Tutorial.h"
#include "C_Position.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Tile.h"

namespace
{
    constexpr float DISTANCE = 200;
    constexpr float TEXT_OFFSET_X = -400;
    constexpr float TEXT_OFFSET_Y = -10;
}

float GetDistance(C_Position * entity1, C_Position * entity2)
{
    Vector3f pos1 = entity1->GetPosition();
    Vector3f pos2 = entity2->GetPosition();

    return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + 
        (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

S_Tutorial::S_Tutorial(SystemManager * sysMgr)
    : S_Base(System::Tutorial, sysMgr)
{
}

void S_Tutorial::Initialize(Tutorial * tutorial, Map * map)
{
    m_tutorial = tutorial;
    m_map = map;
}

void S_Tutorial::Update(Window * window, float /*dt*/)
{
  /*  View * view = window->GetBaseWindow()->GetView();*/
    //const Vector3f & view_center = view->GetCenter();
	m_tutorial->m_text.SetPosition(
		Vector3f(-(window->GetWindowSize().x / 2.0f) + 200.0f, window->GetWindowSize().y / 2.0f, 0));

    auto beginning = m_tutorial->m_triggers.find("GameBegin");
   if (!beginning->second)
    {
       m_tutorial->m_triggers.find("GameBegin")->second = true;
       m_tutorial->m_timer.emplace_back(std::make_pair("GameBegin", 0.f));
       m_tutorial->m_text.SetString(
           "Movement \n -> or <-");

        return;
    }

   auto first_hill = m_tutorial->m_triggers.find("FirstHill");
   if (!first_hill->second && FirstHillOnSight())
   {
       m_tutorial->m_triggers.find("FirstHill")->second = true;
       m_tutorial->m_timer.emplace_back(std::make_pair("FirstHill", 0.f));
       if (m_tutorial->m_timer.size() == 1)
           m_tutorial->m_text.SetString(
               "Jump : Up Arrow");
       return;
   }

   auto first_enemy = m_tutorial->m_triggers.find("FirstEnemy");
   if (!first_enemy->second && FirstEnemyOnSight())
   {
       m_tutorial->m_triggers.find("FirstEnemy")->second = true;
       m_tutorial->m_timer.emplace_back(std::make_pair("FirstEnemy", 0.f));
       if (m_tutorial->m_timer.size() == 1)
           m_tutorial->m_text.SetString(
               "Dash Attack : A \nUppercut : W");
       return;
   }

   auto first_enemy_below = m_tutorial->m_triggers.find("FirstEnemyBelow");
    if (!first_enemy_below->second && FirstEnemyBelowOnSight())
    {
        m_tutorial->m_triggers.find("FirstEnemyBelow")->second = true;
        m_tutorial->m_timer.emplace_back(std::make_pair("FirstEnemyBelow", 0.f));
        if (m_tutorial->m_timer.size() == 1)
            m_tutorial->m_text.SetString(
                "Down attack : S on air"); 
        return;
    }


    auto first_gate = m_tutorial->m_triggers.find("FirstGate");
    if (!first_gate->second && FirstDoorOnSight())
    {
        m_tutorial->m_triggers.find("FirstGate")->second = true;
        m_tutorial->m_timer.emplace_back(std::make_pair("FirstGate", 0.f));
        if(m_tutorial->m_timer.size() == 1)
            m_tutorial->m_text.SetString(
                "The gate requires your soul\nPress 'A' to spend purple soul\nOr press 'S' to spend green soul");
        return;
    }
}

void S_Tutorial::Notify(Message & /*message*/) {}

bool S_Tutorial::FirstEnemyOnSight()
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    auto entities = entityMgr->GetEntityContainer();
    C_Position * player_pos = entityMgr->GetC_Position(m_playerID);

    for (auto & entity : entities)
    {
        if (entityMgr->GetEntityType(entity.first) != "Statue_entity") continue;

        C_Position * pos = entityMgr->GetC_Position(entity.first);
        if (GetDistance(player_pos, pos) < DISTANCE)
            return true;
    }

    return false;
}

bool S_Tutorial::FirstDoorOnSight()
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    auto entities = entityMgr->GetEntityContainer();
    C_Position * player_pos = entityMgr->GetC_Position(m_playerID);

    for (auto & entity : entities)
    {
        if (entityMgr->GetEntityType(entity.first) != "Door_entity") continue;

        C_Position * pos = entityMgr->GetC_Position(entity.first);
        if (GetDistance(player_pos, pos) < DISTANCE)
            return true;
    }

    return false;
}

bool S_Tutorial::FirstEnemyBelowOnSight()
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    auto entities = entityMgr->GetEntityContainer();
    C_Position * player_pos = entityMgr->GetC_Position(m_playerID);

    for (auto & entity : entities)
    {
        if (entityMgr->GetEntityType(entity.first) != "Statue_entity") continue;

        C_Position * pos = entityMgr->GetC_Position(entity.first);
        if (player_pos->GetPosition().y > pos->GetPosition().y && 
            GetDistance(player_pos, pos)< DISTANCE)
            return true;
    }

    return false;
}

bool S_Tutorial::FirstHillOnSight()
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    C_Position * player_pos = entityMgr->GetC_Position(m_playerID);

    int tile_on_player_x = static_cast<int>(floor(player_pos->GetPosition().x / Tile_Size));
    int tile_on_player_y = static_cast<int>(floor(player_pos->GetPosition().y / Tile_Size));

    if (m_map->GetTile(tile_on_player_x + 4, tile_on_player_y))
        return true;

    return false;
}
