#ifndef S_CONTROL_H
#define S_CONTROL_H
/******************************************************************************/
/*!
\headerfile   S_Control.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for control system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"

class S_Control : public S_Base
{
public:
    //Ctor
    S_Control(SystemManager * systemMgr);

    //S_Base method
    void Update(Window * /*window*/, float, bool /*editor_mode*/) {} //Do nothing

    //Observer method
    void Notify(Message &);
};

#endif