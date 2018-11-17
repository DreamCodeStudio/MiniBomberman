#include "World.h"

World::World(irr::scene::ISceneManager *manager)
{
	//Load all 3D-Models here

	//First load the ground 
	_ground = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\Ground\\Ground.obj"), 0, -1,
												irr::core::vector3df(0, -1, 0));

	//Create Game Matrix and set all elements to the default "Blocked" status
	_gameMatrix = new Tile*[10];
	for (int i = 0; i < 10; i++)
	{
		_gameMatrix[i] = new Tile[10];
	}
	//Set default values in the matrix
	for (int y = -5; y < 5; y++)
	{
		for (int x = -5; x < 5; x++)
		{
			_gameMatrix[y + 5][x + 5].Create(manager, irr::core::vector3df(x * 1.5f, 0, y * 1.5f), GAME_TILE_STATE::BLOCKED);
		}
	}

	//Create some light in the scene
	//For testing create a simple cube here
	irr::scene::ILightSceneNode *light = manager->addLightSceneNode(0, irr::core::vector3df(0, 80, 0), 
																	 irr::video::SColor(255, 255, 255, 255), 100);

}

World::World()
{

}