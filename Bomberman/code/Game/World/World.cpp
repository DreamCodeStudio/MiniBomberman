#include "World.h"

World::World(irr::scene::ISceneManager *manager)
{
	//Load all 3D-Models here

	//First load the ground 
	_ground = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\Ground.obj"), 0, -1,
												irr::core::vector3df(0, -1, 0));
												
	//Create Game Matrix and set all elements to the default "Blocked" status
	_gameMatrix = new Tile*[10];
	for (int i = 0; i < 10; i++)
	{
		_gameMatrix[i] = new Tile[10];
	}
	//Set default values in the matrix
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			_gameMatrix[y][x].Create(manager, irr::core::vector3df(x * 3.0f, 0, y * 3.0f), GAME_TILE_STATE::BLOCKED);

		}
	}
}

World::World()
{

}