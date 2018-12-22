#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Defintions for game states, tile states
#include "..\Definitions.h"

//Tile.h for modifieng the tiles, placing bombs, collecting items
#include "..\World\Tile\Tile.h"

//Inventory class for handling the collected items of the player during the game
#include "Inventory/Inventory.h"

//All the items which can get used by the player are organized in classes
#include "Inventory/Items/Bomb/Bomb.h"

//Windows.h for reveiving key input events
#include <Windows.h>

//For a smooth walking animation we need threads
#include <thread>

//For storing items which are currently in game
#include <vector>

//For some calculations like rounding
#include <cmath>


///<summary>
///The player class handles all the user input and the player figure in game.
///</summary>
class Player
{
	public:

		///<summary>The constructor loads the 3D-models e.g. player model, bombs...</summary>
		Player(irr::scene::ISceneManager *manager, Tile **gameMatrix, irr::gui::IGUIEnvironment *gui);
		Player();

		///<summary>The Update method receives the user input and handles animations</summary>
		void Update();

		///<summary>The game over method gets called automatically by the bomb class if the player was hit by an explosion</summary>
		int GetWinLoseStatus();

		///<summary>This method is called if the player died</summary>
		void GameOver();

	private:

		enum WalkingDirection {
			UP,
			LEFT,
			DOWN,
			RIGHT,
		};

		//Player control methods for using items, waling and so on...
		void PlayerControl();
		bool IsColliding();
		void Walk(WalkingDirection direction);
		void WalkingThread(WalkingDirection direction);


		//Other update methods
		void UpdatePlayerStandTile();
		void UpdateEnemyGameOver();

		//Pointer to the scene manager so it can get passed to the create method of items for loading item meshes
		irr::scene::ISceneManager *_manager;

		//Inventory class so the player can collect and use items during the game
		Inventory _inventory;

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

		//Every item which is used by the player an is currently existing in the ingame world should get stored here
		std::vector<Item*> _itemStorage;
		//Using an item has a cooldown so the player does not accidantly use a few of them
		int _itemCooldown;

		//If the player won the game
		int _winLoseStatus;
		bool _gameOver;
};
