#ifndef MOVECHARACTER
#define MOVECHARACTER
/******************************************************************************/
/*!
\headerfile   MoveCharacter.h
\author Kim Hyungseob
\par    email: hn02415@gmail.com
\date   2019/06/12
\brief
		Functions and constants for calculating to play starting and ending animation.
		It uses hard coded values because it can reduce amount of calculation and
		it was very last work, so I could sure that there will be no change.

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "glm.hpp"

namespace BestFit
{
	constexpr float TimerFactor = 0.8f; //!< Parameter to decide the speed of animation.
	constexpr float StartInMain = -0.08997995292586626f; //!< Start position on starting animation.
	constexpr float StartInCredit = 4.522f; //!< Start position on ending animation.
	constexpr float FinishInMain = 1.f; //!< Last position on starting animation.
	constexpr float FinishInCredit = 5.f; //!< Last position on ending animation.

	/*!*******************************************************************
	\brief
		   Calculate y-coordinate of sprite depends on time.

	\param t
		   Elapsed time, so can play animation depends on time.

	\param FinishiTime
		   Time to finish. It is necessary to define the range.

	\return float
			y-coordinate where sprite should be.
	********************************************************************/
	float GetY(float t, float FinishTime);
	/*!*******************************************************************
	\brief
		   Calculate x-coordinate of sprite depends on y.

	\param y
		   Because it is simple quadratic equation, x-coordinate can be calculated with y.

	\return float
			x-coordinate where sprite should be.
	********************************************************************/
	float GetX(float y);
	/*!*******************************************************************
	\brief
		   Calculate scale of sprite depends on time.

	\param t
		   Elapsed time, so can play animation depends on time.

	\param FinishiTime
		   Time to finish. It is necessary to define the range.

	\return float
			Scale of sprite should be.
	********************************************************************/
	float GetScale(float t, float StartTime, float FinishTime);
}

#endif