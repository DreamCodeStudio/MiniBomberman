#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Game Elements
#include "World/World.h"	//World class which creates the game field and the tiles
#include "Player/Player.h"  //Player class which handles all the input from the user

//Defintions for game states, tile states
#include "Definitions.h"

///<summary>
///The Game class is the core. It handles all different Game States, Game Elements
///and window events
///</summary>

class Game
{
	public:

		///<summary>The constructor creates a window and initializes all Game Elements</summary>
		Game();

		///<summary>Updates all Game Elements, handles window events and changes in Game States</summary>
		void Run();

		///<summary><returns>Returns true if the window is still open</returns></summary>
		bool IsOpen();

	private:

		//Update methods which are called by the Run() function
		void Update();
		void Render();
	
		int _currentGameState;

		irr::IrrlichtDevice *_device;			//The irrlicht device for creating the window
		irr::video::IVideoDriver *_driver;		//Video driver 
		irr::scene::ISceneManager *_manager;	//Scene manager for creating the game scene

		irr::scene::ICameraSceneNode *_camera;	//Camera 

		//Game elements
		World _gameWorld;
		Player _player1, _player2;
};