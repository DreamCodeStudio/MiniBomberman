#include "Game.h"

Game::Game()
{
	//Create game window
	_device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_DIRECT3D9, irr::core::dimension2d<irr::u32>(1920, 1080),
								16U, false, false, true, 0);

	//Get video driver and scene manager
	_driver = _device->getVideoDriver();
	_manager = _device->getSceneManager();

	//Create first person camera. The player can controll the camera via the LeapMotion controller
	//Because this is kind of an ego-shooter we use a first person camera here. 
	_camera = _manager->addCameraSceneNodeFPS(0, 100, 0.05);

	//Set start values
	_currentGameState = GAME_STATE::RUN;

	//Create Game Elements here
	_gameWorld = World(_manager);
}

void Game::Run()
{
	this->Update();		//Update Game Elements
	this->Render();		//Render 
}

bool Game::IsOpen()
{
	//Returns if the window is still open, so the main loop knows when to exit 
	return _device->run();
}

// ###### Private methods ######

void Game::Update()
{
	//Update all game elements here
	switch (_currentGameState)
	{
		case GAME_STATE::MENU: {

		}
			break;
		case GAME_STATE::RUN: {

		}
			break;
	}
}

void Game::Render()
{
	//Draw scene here

	_driver->beginScene(true, true, irr::video::SColor(150, 150, 150, 150));

	_manager->drawAll();	//Draw all objects which were added to the scene to the window

	_driver->endScene();
}