#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Defintions for game states, tile states
#include "..\Definitions.h"

//The world class handles all tiles which together form the game field
#include "Tile/Tile.h"

///<summary>
///The world class handles all objects and interactions
///in the virtual world, which are not items or the 
///player.
///</summary>

class World
{
	public:
		
		///<summary>The World constructor loads all 3D-Models and positions them</summary>
		World(irr::scene::ISceneManager *manager);
		World();

		///<summary>The update method handles all interactions between the player and world objects</summary>
		void Update();

		Tile **GetGameMatrix();

	private:

		//3D-Models
		irr::scene::IAnimatedMeshSceneNode *_ground;

		//Matrix to know which Game tiles are blocked, available, or currently under fire
		Tile **_gameMatrix;
};