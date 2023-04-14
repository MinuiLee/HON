/******************************************************************************/
/*!
\file   Goal_Composite.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/11

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Goal_Composite.h"

void Goal_Composite::RemoveAllSubgoals(SystemManager* systemMgr, EntityId entity)
{
	for (auto& subgoal : m_subgoals)
	{
		subgoal->Terminate(systemMgr, entity);
		delete subgoal;
	}

	m_subgoals.clear();
}

// First removes any completed goals from the front of the list.
// It then processes the next goal in the list (if there is one).
int Goal_Composite::ProcessSubgoals(SystemManager* systemMgr, EntityId entity, float dt)
{
	// Remove all completed & failed goals from the front of the list.
	while (!m_subgoals.empty() && (m_subgoals.front()->IsComplete() || m_subgoals.front()->HasFailed()))
	{
		m_subgoals.front()->Terminate(systemMgr, entity);
		delete m_subgoals.front();
		m_subgoals.pop_front();
	}

	// If any subgoals remain, process the one at the fron of the list
	if (!m_subgoals.empty())
	{
		// Grab the status of the front-most subgoal.
		int status_of_subgoal = m_subgoals.front()->Process(systemMgr, entity, dt);

		// Test for speceial case
		// [front-most subgoal report completed & subgoal list contains additional goals]
		// to ensure the parent keeps processing its subgoal list, return the active status
		if (status_of_subgoal == completed && m_subgoals.size() > 1)
			return active;

		return status_of_subgoal;
	}

	// no more subgoals to process
	return completed;
}

void Goal_Composite::AddSubgoal(Goal* goal)
{
	// Add the new goal to the front of the list
	m_subgoals.push_front(goal);
}

// Passes the message to the goal at the front of the queue.
bool Goal_Composite::ForwardMessageToFrontMostSubgoal(SystemManager* systemMgr, const Message& msg)
{
	if (!m_subgoals.empty())
		return m_subgoals.front()->HandleMessage(systemMgr, msg);

	// messag has not been handled
	return false;
}