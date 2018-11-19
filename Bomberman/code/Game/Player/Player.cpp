#include "Player.h"

//The static variable to start value
int Player::_instanceCounter = 0;

Player::Player(irr::scene::ISceneManager *manager, Tile **gameMatrix)
{
	//Save pointer to the game matrix, so the player can interact with the world
	_gameMatrix = gameMatrix;
	//test
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (_gameMatrix[y][x].GetTileState() == GAME_TILE_STATE::BLOCKED)
			{
				std::cout << "+";
			}
			else
			{
				std::cout << "#";
			}
		}
		std::cout << std::endl;
	}

	//Count the current instance
	_currentInstance = ++_instanceCounter;
	std::cout << "Created Player instance " << _instanceCounter << std::endl;
	
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

			if (GetAsyncKeyState(VK_UP))
			{
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
			if (GetAsyncKeyState(VK_LEFT))
			{
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
			if (GetAsyncKeyState(VK_DOWN))
			{
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
			if (GetAsyncKeyState(VK_RIGHT))
			{
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
		std::cout << "Matrix empty!" << std::endl;
		return false;
	}
}

void Player::WalkingThread(WalkingDirection direction)
{
	//As long as the player is walking the user should not change the walking direction
	_isWalking = true;
	std::cout << "run " << std::endl;
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