#include "Player.h"

//The static variable to start value
int Player::_instanceCounter = 0;

Player::Player(irr::scene::ISceneManager *manager, Tile **gameMatrix, irr::gui::IGUIEnvironment *gui)
{
	//Save pointer to the game matrix, so the player can interact with the world
	_gameMatrix = gameMatrix;

	//Save pointer to the scene manager, so items can load 3D-meshes later
	_manager = manager;

	//Count the current instance
	_currentInstance = ++_instanceCounter;
	std::cout << "Created Player instance " << _instanceCounter << std::endl;

	//Create inventory
	_inventory = Inventory(manager, gui, _currentInstance);

	//!!!Note there are 4 spawn positions implemented, but player controll and inventory class will only support
	//!!!2 player instances.

	//Select a spawn location for the player, according to the number of instances
	switch (_currentInstance)	//The spawn locations are the corners of our game field
	{							
		case 1: {
			_playerSpawnPosition = irr::core::vector3df(-7.5f, 0, -7.5f);
		}
			break;
		case 2: {
			_playerSpawnPosition = irr::core::vector3df(6, 0, 6);
		}
				break;
		case 3: {
			_playerSpawnPosition = irr::core::vector3df(-7.5f, 0, 6);
		}
				break;
		case 4: {
			_playerSpawnPosition = irr::core::vector3df(6, 0, -7.5f);
		}
				break;
	}

	//Load Player model 
	_playerModel = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\Player\\Player.obj"), 0, -1,
													_playerSpawnPosition, irr::core::vector3df(0, 0, 0), 
													irr::core::vector3df(0.3f, 0.3f, 0.3f));	//Scale the model so it matches in size

	//After the scaling of the player model we need to normalize it again, otherwise it will be reflecting to much light
	_playerModel->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);	

	//Set start values
	_isWalking = false;
}

Player::Player()
{

}

void Player::Update()
{
	if (_isWalking)
	{
		return;
	}

	//Depending on the instance different key for the player control should be used
	switch (_currentInstance)
	{
		case 1: {

			//For moving the player
			if (GetAsyncKeyState(VK_UP))
			{
				this->Walk(WalkingDirection::UP);
			}
			if (GetAsyncKeyState(VK_LEFT))
			{
				this->Walk(WalkingDirection::LEFT);
			}
			if (GetAsyncKeyState(VK_DOWN))
			{
				this->Walk(WalkingDirection::DOWN);
			}
			if (GetAsyncKeyState(VK_RIGHT))
			{
				this->Walk(WalkingDirection::RIGHT);
			}

			//For using items
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				_bomb.Create(_manager, _playerModel->getAbsolutePosition(), _gameMatrix);
				Sleep(500);
			}
		}
				break;
		case 2: {

			if (GetAsyncKeyState('W'))
			{
				this->Walk(WalkingDirection::UP);

			}
			if (GetAsyncKeyState('A'))
			{
				this->Walk(WalkingDirection::LEFT);
			}
			if (GetAsyncKeyState('S'))
			{
				this->Walk(WalkingDirection::DOWN);
			}
			if (GetAsyncKeyState('D'))
			{
				this->Walk(WalkingDirection::RIGHT);
			}
		}
				break;
	}
}

/* ##### Private ###### */

bool Player::IsColliding()
{
	_playerModel->updateAbsolutePosition();
	int xPosition = static_cast<int>(_playerModel->getAbsolutePosition().X / 1.5f) + 5;
	int zPosition = static_cast<int>(_playerModel->getAbsolutePosition().Z / 1.5f) + 5;

	if (zPosition < 0 || zPosition > 9 || xPosition < 0 || xPosition > 9)
	{
		return true;
	}
	else if (_gameMatrix[zPosition][xPosition].GetTileState() == GAME_TILE_STATE::BLOCKED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::Walk(WalkingDirection direction)
{
	switch (direction)
	{
		case WalkingDirection::UP: {
			//Set rotation in the walking direction
			_playerModel->setRotation(irr::core::vector3df(0, 180, 0));

			//First of all check if a collision would occurr on the targeted location
			_playerModel->updateAbsolutePosition();
			_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X + 1.5f,
				_playerModel->getAbsolutePosition().Y,
				_playerModel->getAbsolutePosition().Z));

			//Now check if a collision occurs if so set the player model back and exit
			if (this->IsColliding())
			{
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X - 1.5f,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z));
			}
			else
			{
				//Set the model back and run the animation after it
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X - 1.5f,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z));
				//If no collision occured run the animation
				//For a smooth walking animation call a thread which runs parallel
				std::thread Thread(&Player::WalkingThread, this, WalkingDirection::UP);
				Thread.detach();
			}
		}
			break;
		case WalkingDirection::LEFT: {

			_playerModel->setRotation(irr::core::vector3df(0, 90, 0));

			//First of all check if a collision would occurr on the targeted location
			_playerModel->updateAbsolutePosition();
			_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
				_playerModel->getAbsolutePosition().Y,
				_playerModel->getAbsolutePosition().Z + 1.5f));

			//Now check if a collision occurs if so set the player model back and exit
			if (this->IsColliding())
			{
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z - 1.5f));
			}
			else
			{
				//Set the model back and run the animation after it
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z - 1.5f));
				//If no collision occured run the animation
				//For a smooth walking animation call a thread which runs parallel
				std::thread Thread(&Player::WalkingThread, this, WalkingDirection::LEFT);
				Thread.detach();
			}
		}
			break;
		case WalkingDirection::DOWN: {

			//Set rotation in the walking direction
			_playerModel->setRotation(irr::core::vector3df(0, 0, 0));

			//First of all check if a collision would occurr on the targeted location
			_playerModel->updateAbsolutePosition();
			_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X - 1.5f,
				_playerModel->getAbsolutePosition().Y,
				_playerModel->getAbsolutePosition().Z));

			//Now check if a collision occurs if so set the player model back and exit
			if (this->IsColliding())
			{
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X + 1.5f,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z));
			}
			else
			{
				//Set the model back and run the animation after it
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X + 1.5f,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z));
				//If no collision occured run the animation
				//For a smooth walking animation call a thread which runs parallel
				std::thread Thread(&Player::WalkingThread, this, WalkingDirection::DOWN);
				Thread.detach();
			}
		}
			break;
		case WalkingDirection::RIGHT: {
			_playerModel->setRotation(irr::core::vector3df(0, 270, 0));

			//First of all check if a collision would occurr on the targeted location
			_playerModel->updateAbsolutePosition();
			_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
				_playerModel->getAbsolutePosition().Y,
				_playerModel->getAbsolutePosition().Z - 1.5f));

			//Now check if a collision occurs if so set the player model back and exit
			if (this->IsColliding())
			{
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z + 1.5f));
			}
			else
			{
				//Set the model back and run the animation after it
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z + 1.5f));
				//If no collision occured run the animation
				//For a smooth walking animation call a thread which runs parallel
				std::thread Thread(&Player::WalkingThread, this, WalkingDirection::RIGHT);
				Thread.detach();
			}
		}
	}
}

void Player::WalkingThread(WalkingDirection direction)
{
	//As long as the player is walking the user should not change the walking direction
	_isWalking = true;
	std::cout << std::endl;

	//Now move the player every couple of ms a little bit in the direction
	for (unsigned int i = 0; i < 75; i++)
	{
		switch (direction)
		{
			case UP: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X + 0.02f,
				_playerModel->getAbsolutePosition().Y,
				_playerModel->getAbsolutePosition().Z));
			}
				break;
			case DOWN: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X - 0.02f,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z));
			}
					 break;
			case LEFT: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z + 0.02f));
			}
					 break;
			case RIGHT: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z - 0.02f));
			}
					 break;
		}
		Sleep(1);
	}

	_isWalking = false;
}