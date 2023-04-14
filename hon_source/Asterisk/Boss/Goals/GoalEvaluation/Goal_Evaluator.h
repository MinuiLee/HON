#ifndef GOAL_EVALUATOR_H
#define GOAL_EVALUATOR_H
/*!*******************************************************************
\headerfile   Goal_Evaluator.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/05/18
\brief
			  Header file to define an interface for objects that
			  are able to evaluate the desirability of a specific
			  strategy level goal.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
using EntityId = unsigned int;
class SystemManager;
class Goal_Think;

/*!*******************************************************************
\class Goal_Evaluator
\brief
	   Define base goal evaluator class.
********************************************************************/
class Goal_Evaluator
{
protected:

	double m_CharacterBias; //!< Used to create bots with preferences based upon their personality.

public:

	/*!*******************************************************************
	\brief
		   Default constructor.

	\param characterBias
		   New value for AI personality.
	********************************************************************/
	Goal_Evaluator(double characterBias) : m_CharacterBias(characterBias) {}

	/*!*******************************************************************
	\brief Default virtual destructor.
	********************************************************************/
	virtual ~Goal_Evaluator() {}

	/*!*******************************************************************
	\brief
	       Calculates and returns desirability of the startegy
		   the concrete subclass represents.

	\param systemMgr
	       Pointer to system manager to use engine interface.

	\param entity
	       Id of entity in this state.

	\return double
			Score between 0 and 1 representing desirability of goal.
	********************************************************************/
	virtual double CalculateDesirability(SystemManager* systemMgr, EntityId entity) = 0;

	/*!*******************************************************************
	\brief
		   Adds the appropriate goal to the given entity's brain.

	\param brain
		   Pointer to Goal_Think.
	********************************************************************/
	virtual void SetGoal(Goal_Think* brain) = 0;
};

#endif