#ifndef ENTITY_STATE_H
#define ENTITY_STATE_H
/******************************************************************************/
/*!
\headerfile   EntityState.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2019/02/01
\brief
		Header file for lists of entity state.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

enum class EntityState
{
    Idle = 0,
    Walk,
    Jump,
    Dash,
    Hurt,
    Knockdown,
    Uppercut,
    Stomp,
    Air_Stomp,
    Dead,
    Other
};

#endif