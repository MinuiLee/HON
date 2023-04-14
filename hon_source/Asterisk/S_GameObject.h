#ifndef S_GAMEOBJECT_H
#define S_GAMEOBJECT_H
/*!*******************************************************************
\headerfile   S_GameObject.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/30
\brief
			  Header file for GameObject system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "S_Base.h"
#include "Text.h"

#include "GameObject/DoorUI.h"

class Window;

class S_GameObject : public S_Base
{
public:
    //Ctor
    S_GameObject(SystemManager * sysMgr);

    //S_Base methods
    void Update(Window * window, float dt, bool editor_mode) override;

    //Observer method
    void Notify(Message & message) override;

    void Draw(Window * wind);

    void InitializeObject(EntityId entity, int condition = 0);

private:
    bool m_DrawText = false;
	bool m_is_shake   = false;
	DoorUI* m_doorUI = DoorUI::GetInstance();
};

#endif //S_GAMEOBJECT_H