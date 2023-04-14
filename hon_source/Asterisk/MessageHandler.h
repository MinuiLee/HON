#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
/******************************************************************************/
/*!
\headerfile   MessageHandler.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <unordered_map>
#include "Communicator.h"

using CommunicatorContainer =
    std::unordered_map<EntityMessage, Communicator>;

class MessageHandler
{

public:
    MessageHandler() {}
    ~MessageHandler() { m_communicators.clear(); }

    void Subscribe(const EntityMessage & type, Observer * observer)
    {
	m_communicators[type].AddObserver(observer);
    }

    void Unsubscribe(const EntityMessage & type, Observer * observer)
    {
	m_communicators[type].RemoveObserver(observer);
    }

    void Dispatch(Message & message)
    {
	auto itr = m_communicators.find((EntityMessage)message.m_type);
	if (itr == m_communicators.end())
	    return;
	m_communicators[(EntityMessage)message.m_type].Broadcast(message);
    }

private:
    CommunicatorContainer m_communicators;
};

#endif