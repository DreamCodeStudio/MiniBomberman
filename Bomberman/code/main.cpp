#include <iostream>

#include "Game/Game.h"

int main()
{
	Game game = Game();

	while (game.IsOpen())
	{
		game.Run();
	}

	return 0;
}