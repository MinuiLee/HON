#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
/******************************************************************************/
/*!
\headerfile   Communicator.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <vector>
#include "Observer.h"
#include "S_Base.h"

using ObserverContainer = std::vector<Observer *>;

class Communicator
{
public:
    Communicator() {}
    ~Communicator() { m_observers.clear(); }

    bool AddObserver(Observer * observer)
    {
	for (auto itr = m_observers.begin(); itr != m_observers.end(); ++itr)
	{
	    if (*itr == observer)
		return false;
	}
	m_observers.emplace_back(observer);
	return true;
    }

    bool RemoveObserver(Observer * observer)
    {
	for (auto itr = m_observers.begin(); itr != m_observers.end(); ++itr)
	{
	    if (*itr == observer)
	    {
		m_observers.erase(itr);
		return true;
	    }
	}
	return false;
    }

    void Broadcast(Message & message)
    {
	for (auto itr : m_observers)
	{
	    if (static_cast<S_Base *>(itr)->HasEntity(message.m_receiver))
		itr->Notify(message);
	}
    }

private:
    ObserverContainer m_observers;
};

#endif