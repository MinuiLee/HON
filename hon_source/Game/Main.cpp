/******************************************************************************/
/*!
\file   Main.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Game.h"
#undef main
int main(int /*argc*/, int* /*argv[]*/)
{
#define main    SDL_main
	Game game;

	//Game Loop
	while (!game.GetWindow().IsDone())
	{
        game.Update();
        game.Draw();
        game.RestartClock();
        game.Clear();
	}

	return 0;
}