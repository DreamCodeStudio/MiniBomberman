#include "Bomb.h"

void Bomb::Create(irr::scene::ISceneManager *manager, irr::core::vector3df playerPos, Tile **gameMatrix)
{
	//Load 3D-mesh for the bomb model and place it on the position of the player
	_bombMesh = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\Bomb\\Bomb.obj"), 0, -1, playerPos);		

	std::thread thread(&Bomb::InteractWithWorld, this, gameMatrix);
	thread.detach();
	//this->InteractWithWorld(gameMatrix);
}

void Bomb::InteractWithWorld(Tile **gameMatrix)
{
	std::cout << "Started item interaction thread..." << std::endl;

	for (unsigned int i = 0; i < 3; i++)
	{
		//Maybe do some animation here...

		//Wait 3 seconds before explode
		Sleep(1000);
	}

	//If tiles around the bomb exists - remove them 

	//First: Get the coords of the bomb in the game matrix
	int xPosition = static_cast<int>(round(_bombMesh->getAbsolutePosition().X / 1.5f)) + 5;
	int zPosition = static_cast<int>(round(_bombMesh->getAbsolutePosition().Z / 1.5f)) + 5;

	int setter = 0;
	//Second: Remove every Tile around the bomb
	if (zPosition > 0)
	{
		gameMatrix[zPosition - 1][xPosition].SetTileState(GAME_TILE_STATE::EMPTY);
		setter++;
	}
	if (zPosition < 9)
	{
		gameMatrix[zPosition + 1][xPosition].SetTileState(GAME_TILE_STATE::EMPTY);
		setter++;
	}
	if (xPosition > 0)
	{
		gameMatrix[zPosition][xPosition - 1].SetTileState(GAME_TILE_STATE::EMPTY);
		setter++;
	}
	if (xPosition < 9)
	{
		gameMatrix[zPosition][xPosition + 1].SetTileState(GAME_TILE_STATE::EMPTY);
		setter++;
	}

	//Third: Remove the bomb mesh (because it exploded and does not exist anymore)
	_bombMesh->setVisible(false);

	std::cout << "Exited interaction thread!" << std::endl;
	std::cout << "Set " << setter << " Tiles to empty!" << std::endl;
}