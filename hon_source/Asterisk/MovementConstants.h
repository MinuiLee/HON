#ifndef MOVEMENTCONSTANTS_H
#define MOVEMENTCONSTANTS_H
/*!*******************************************************************
\headerfile   MovementConstants.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/04/30
\brief
			  Header file for constants for movement.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/

namespace {
	constexpr float GRAVITY = -512.f;
	constexpr float DASH_SPEED = 600.f;

	constexpr float STOMP_IMPACT_VELOCITY = 300.f;
	constexpr float HURT_DELAY = 0.5f;
	constexpr float STOMP_DELAY = 0.3f;
	constexpr float AIR_STOMP_DELAY = 0.3f;
	constexpr float AIR_STOMP_VELOCITY = -1200.f;
	constexpr float HIGH_AIR_STOMP_FALLING_TIME = 0.4f;

	constexpr float UPPERCUT_DELAY = 0.15f;
	constexpr float UPPERCUT_DELAY_FREEZE = 0.05f;
	constexpr float UPPERCUT_VELOCITY_X = 200.f;
	constexpr float UPPERCUT_VELOCITY_Y = 1000.f;
	constexpr float ATTACK_IMPACT = 300.f;

	constexpr float DASH_ENDVELOCITY = 200.f;
	constexpr float FRICTION = 0.8f;
	constexpr float FORCE = 1024.f;
}

#endif