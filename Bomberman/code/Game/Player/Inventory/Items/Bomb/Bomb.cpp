#include "Bomb.h"

void Bomb::Create(irr::scene::ISceneManager *manager, irr::core::vector3df playerPos, Tile **gameMatrix)
{
	//Set item type
	_itemType = GAME_ITEM::BOMB;

	//Load 3D-mesh for the bomb model and place it on the position of the player
	_bombMesh = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\Bomb\\Bomb.obj"), 0, -1, playerPos);		

	std::thread thread(&Bomb::InteractWithWorld, this, gameMatrix);
	thread.detach();
}

void Bomb::InteractWithWorld(Tile **gameMatrix)
{
	//Mark all tiles which are effected by the following explosion
	this->SetBombEffectedTiles(gameMatrix, GAME_TILE_STATE::MARKED);

	for (unsigned int i = 0; i < 3; i++)
	{
		//Maybe do some animation here...

		//Wait 3 seconds before explode
		Sleep(1000);
	}

	//Set default return value if nobody is blown up
	_threadExitStatus = 0;
	this->CheckForPlayerHit(gameMatrix);
	this->SetBombEffectedTiles(gameMatrix, GAME_TILE_STATE::DESTROY);

	//Third: Remove the bomb mesh (because it exploded and does not exist anymore)
	_bombMesh->setVisible(false);
}

void Bomb::SetBombEffectedTiles(Tile **gameMatrix, GAME_TILE_STATE state)
{
	//First: Get the coords of the bomb in the game matrix
	int xPosition = static_cast<int>(round(_bombMesh->getAbsolutePosition().X / 1.5f)) + 5;
	int zPosition = static_cast<int>(round(_bombMesh->getAbsolutePosition().Z / 1.5f)) + 5;

	//Second: Remove every Tile around the bomb
	if (zPosition > 0)
	{
		gameMatrix[zPosition - 1][xPosition].SetTileState(state);
		if (zPosition > 1)
		{
			gameMatrix[zPosition - 2][xPosition].SetTileState(state);
		}
	}
	if (zPosition < 9)
	{
		gameMatrix[zPosition + 1][xPosition].SetTileState(state);
		if (zPosition < 8)
		{
			gameMatrix[zPosition + 2][xPosition].SetTileState(state);
		}
	}
	if (xPosition > 0)
	{
		gameMatrix[zPosition][xPosition - 1].SetTileState(state);
		if (xPosition > 1)
		{
			gameMatrix[zPosition][xPosition - 2].SetTileState(state);
		}
	}
	if (xPosition < 9)
	{
		gameMatrix[zPosition][xPosition + 1].SetTileState(state);
		if (xPosition < 8)
		{
			gameMatrix[zPosition][xPosition + 2].SetTileState(state);
		}
	}
}

void Bomb::CheckForPlayerHit(Tile **gameMatrix)
{
	//First: Get the coords of the bomb in the game matrix
	int xPosition = static_cast<int>(round(_bombMesh->getAbsolutePosition().X / 1.5f)) + 5;
	int zPosition = static_cast<int>(round(_bombMesh->getAbsolutePosition().Z / 1.5f)) + 5;

	if (zPosition > 0)
	{
		if (gameMatrix[zPosition - 1][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
		{
			std::cout << "Player 1 was blown up!" << std::endl;
			_threadExitStatus = 1;
		}
		if (gameMatrix[zPosition - 1][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
		{
			std::cout << "Player 2 was blown up!" << std::endl;
			_threadExitStatus = 2;
		}

		if (zPosition > 1)
		{
			if (gameMatrix[zPosition - 2][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
			{
				std::cout << "Player 1 was blown up!" << std::endl;
				_threadExitStatus = 1;
			}
			if (gameMatrix[zPosition - 2][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
			{
				std::cout << "Player 2 was blown up!" << std::endl;
				_threadExitStatus = 2;
			}
		}
	}
	if (zPosition < 9)
	{
		if (gameMatrix[zPosition + 1][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
		{
			std::cout << "Player 1 was blown up!" << std::endl;
			_threadExitStatus = 1;
		}
		if (gameMatrix[zPosition + 1][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
		{
			std::cout << "Player 2 was blown up!" << std::endl;
			_threadExitStatus = 2;
		}
		if (zPosition < 8)
		{
			if (gameMatrix[zPosition + 2][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
			{
				std::cout << "Player 1 was blown up!" << std::endl;
				_threadExitStatus = 1;
			}
			if (gameMatrix[zPosition + 2][xPosition].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
			{
				std::cout << "Player 2 was blown up!" << std::endl;
				_threadExitStatus = 2;
			}
		}
	}
	if (xPosition > 0)
	{
		if (gameMatrix[zPosition][xPosition - 1].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
		{
			std::cout << "Player 1 was blown up!" << std::endl;
			_threadExitStatus = 1;
		}
		if (gameMatrix[zPosition][xPosition - 1].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
		{
			std::cout << "Player 2 was blown up!" << std::endl;
			_threadExitStatus = 2;
		}
		if (xPosition > 1)
		{
			if (gameMatrix[zPosition][xPosition - 2].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
			{
				std::cout << "Player 1 was blown up!" << std::endl;
				_threadExitStatus = 1;
			}
			if (gameMatrix[zPosition][xPosition - 2].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
			{
				std::cout << "Player 2 was blown up!" << std::endl;
				_threadExitStatus = 2;
			}
		}
	}
	if (xPosition < 9)
	{
		if (gameMatrix[zPosition][xPosition + 1].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
		{
			std::cout << "Player 1 was blown up!" << std::endl;
			_threadExitStatus = 1;
		}
		if (gameMatrix[zPosition][xPosition + 1].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
		{
			std::cout << "Player 2 was blown up!" << std::endl;
			_threadExitStatus = 2;
		}
		if (xPosition < 8)
		{
			if (gameMatrix[zPosition][xPosition + 2].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS)
			{
				std::cout << "Player 1 was blown up!" << std::endl;
				_threadExitStatus = 1;
			}
			if (gameMatrix[zPosition][xPosition + 2].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS)
			{
				std::cout << "Player 2 was blown up!" << std::endl;
				_threadExitStatus = 2;
			}
		}
	}
}