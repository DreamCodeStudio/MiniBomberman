#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Defintions for game states, tile states
#include "..\Definitions.h"

//Tile.h for modifieng the tiles, placing bombs, collecting items
#include "..\World\Tile\Tile.h"

//Windows.h for reveiving key input events
#include <Windows.h>

//For a smooth walking animation we need threads
#include <thread>

///<summary>
///The player class handles all the user input and the player figure in game.
///</summary>

class Player
{
	public:

		///<summary>The constructor loads the 3D-models e.g. player model, bombs...</summary>
		Player(irr::scene::ISceneManager *manager, Tile **gameMatrix);
		Player();

		///<summary>The Update method receives the user input and handles animations</summary>
		void Update();

	private:

		enum WalkingDirection {
			UP,
			LEFT,
			DOWN,
			RIGHT,
		};

		//Collision detection function 
		bool IsColliding();
		void WalkingThread(WalkingDirection direction);

		//Pointer to the game matrix, so the player can collect items, destroy tiles and more 
		Tile** _gameMatrix;

		//3D-Player model
		irr::scene::IAnimatedMeshSceneNode *_playerModel; //The 3D-mesh data
		irr::core::vector3df _playerSpawnPosition;		  //Spawn position (depends on the _instanceCounter)
		
		//If the player is still walking the user should not start a new animation
		bool _isWalking;

		//Every player instance has it's own corner to spawn
		static int _instanceCounter;
		int _currentInstance;

};
