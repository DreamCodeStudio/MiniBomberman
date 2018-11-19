#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Defintions for game states, tile states
#include "..\..\Definitions.h"

///<summary>
///An object of the Tile class represents a single tile on the 
///game field. The tile can be destroyed by the players. There
///is also a chance for items to spawn, after a block was destroyed
///</summary>

class Tile
{
	public:

		///<summary>The create method loads according to the tile state it's current model 
		///e.g. blocks, items and positions them</summary>
		void Create(irr::scene::ISceneManager *manager, irr::core::vector3df tilePosition, GAME_TILE_STATE tileState);

		///<summary>Sets the state of a tile an automatically changes it's 3D-Model</summary>
		void SetTileState(GAME_TILE_STATE tileState);
		GAME_TILE_STATE GetTileState();

	private:

		//Save a pointer to the SceneManager because the tile state can get changed a few times so we need to be able to 
		//load different model at any run time
		irr::scene::ISceneManager *_manager;

		//The current tile model
		irr::scene::IAnimatedMeshSceneNode *_tileModel;
		irr::core::vector3df _tilePosition;
		GAME_TILE_STATE _currentGameTileState;
		
		int test;
};
