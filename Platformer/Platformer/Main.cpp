#include "Game.h"


#undef main
int main()
{	
	Game* game = new Game();

	if (game)
	{
		while (game->GetProgramRunning())
		{
			game->Update();
		}

		delete game;
		game = nullptr;
	}

	return 0;
	}

	