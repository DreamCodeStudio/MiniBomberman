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
	//_inventory = Inventory(manager, gui, _currentInstance);

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

	//For a random screen shake we need random numbers
	srand(static_cast<unsigned int>(time(NULL)));

	//Load Player model 
	_playerModel = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\Player\\Player.obj"), 0, -1,
													_playerSpawnPosition, irr::core::vector3df(0, 0, 0), 
													irr::core::vector3df(0.3f, 0.3f, 0.3f));	//Scale the model so it matches in size

	//After the scaling of the player model we need to normalize it again, otherwise it will be reflecting to much light
	_playerModel->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);	

	//Load the game over mesh which can be displayed a player won. The text is depending on the instance number
	if (_currentInstance == 1)
	{
		_gameOverNode = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\GameOver\\Player2Won\\Player2Won.obj"), 0, -1,
														  irr::core::vector3df(0, 5, 0));
	}
	if (_currentInstance == 2)
	{
		_gameOverNode = manager->addAnimatedMeshSceneNode(manager->getMesh("Assets\\Models\\GameOver\\Player1Won\\Player1Won.obj"), 0, -1, 
														  irr::core::vector3df(0, 5, 0));
	}
	_gameOverNode->setVisible(false);		//Set visibility to false, because the game just started
	_gameOverNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);	//The node should not be affected by light, so it is easier to read
	_gameOverNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);

	//Set start values
	_isWalking = false;
	_gameOver = false;	//Is set to true if the game class calls the "GameOver" method
	_winLoseStatus = 0; //Is changed if this player class won or killed itself
	_itemCooldown = 0;
}

Player::Player()
{

}

void Player::Update()
{
	//Update and handle all the user input
	this->PlayerControl();

	//This method checks the return value of the bomb threads. If the return value is the number of the other player instance 
	//we have won
	this->UpdateEnemyGameOver();
}

int Player::GetWinLoseStatus()
{
	return _winLoseStatus;
}

void Player::GameOver()
{
	_gameOver = true; //Set game over to true, so the player does not react on inputs
	_playerModel->setVisible(false); //If the player died -> set model invisible
	_gameOverNode->setVisible(true); //If the player died -> set game over text visibility to true
	_gameOverNode->setRotation(_manager->getActiveCamera()->getRotation());
}

/* ##### Private ###### */


/* ######## Alle methods for the player control (using items, walking) are implemented here ######## */
void Player::PlayerControl()
{
	//Lower the cooldown every frame
	_itemCooldown++;

	//While the player is still walking - do not take any commands
	if (_isWalking || _gameOver)
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
			if (GetAsyncKeyState(VK_RCONTROL) && _itemCooldown > 50)
			{
				std::cout << "Running in instance 1! Dropping bomb!" << std::endl;
				//Create a new bomb object and store it
				_itemStorage.push_back(new Bomb);
				_itemStorage[_itemStorage.size() - 1]->Create(_manager, _playerModel->getAbsolutePosition(), _gameMatrix);
				//Delete method needs to get implemented, so the memory gets freed again

				//Set cooldown 
				_itemCooldown = 0;
			}
		}
				break;
		case 2: {

			//For moving the player
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

			//For using items
			if (GetAsyncKeyState(VK_SPACE) && _itemCooldown > 50)
			{
				std::cout << "Running in instance 2! Dropping bomb!" << std::endl;
				//Create a new bomb object and store it
				_itemStorage.push_back(new Bomb);
				_itemStorage[_itemStorage.size() - 1]->Create(_manager, _playerModel->getAbsolutePosition(), _gameMatrix);

				//Set item cooldown
				_itemCooldown = 0;
			}
		}
				break;
	}
}

bool Player::IsColliding()
{
	_playerModel->updateAbsolutePosition();
	int xPosition = static_cast<int>(round(_playerModel->getAbsolutePosition().X / 1.5)) + 5;
	int zPosition = static_cast<int>(round(_playerModel->getAbsolutePosition().Z / 1.5)) + 5;

	if (zPosition < 0 || zPosition > 9 || xPosition < 0 || xPosition > 9)
	{
		return true;
	}
	else if (_gameMatrix[zPosition][xPosition].GetTileState() == GAME_TILE_STATE::BLOCKED ||
			 _gameMatrix[zPosition][xPosition].GetTileState() == GAME_TILE_STATE::UNDESTRUCTABLE)
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

	//Now move the player every couple of ms a little bit in the direction
	for (unsigned int i = 0; i < 50; i++)
	{
		switch (direction)
		{
			case UP: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X + 0.03f,
				_playerModel->getAbsolutePosition().Y,
				_playerModel->getAbsolutePosition().Z));
			}
				break;
			case DOWN: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X - 0.03f,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z));
			}
					 break;
			case LEFT: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z + 0.03f));
			}
					 break;
			case RIGHT: {
				_playerModel->updateAbsolutePosition();
				_playerModel->setPosition(irr::core::vector3df(_playerModel->getAbsolutePosition().X,
					_playerModel->getAbsolutePosition().Y,
					_playerModel->getAbsolutePosition().Z - 0.03f));
			}
					 break;
		}
		Sleep(3);
	}

	_isWalking = false;

	//Set the tile on which the player is standing to a specific state, so the bomb class knows if a player was blown up
	this->UpdatePlayerStandTile();
}

/* ###### End of player control methods ######## */

void Player::UpdatePlayerStandTile()
{
	//Get the current position of the player in the matrix...
	int xPosition = static_cast<int>(round(_playerModel->getAbsolutePosition().X / 1.5)) + 5;
	int zPosition = static_cast<int>(round(_playerModel->getAbsolutePosition().Z / 1.5)) + 5;

	//According to the player instance number, set the tile on which the player is currently standing to a specific value
	if (_currentInstance == 1)
	{
		_gameMatrix[zPosition][xPosition].SetTileState(GAME_TILE_STATE::PLAYER1_STANDS);
	}
	if (_currentInstance == 2)
	{
		_gameMatrix[zPosition][xPosition].SetTileState(GAME_TILE_STATE::PLAYER2_STANDS);
	}

	//Go through the whole tile field and set the old standing position to empty
	for (unsigned int y = 0; y < 10; y++)
	{
		for (unsigned int x = 0; x < 10; x++)
		{
			if (_currentInstance == 1)
			{
				if (_gameMatrix[y][x].GetTileState() == GAME_TILE_STATE::PLAYER1_STANDS && (y != zPosition || x != xPosition))
				{
					_gameMatrix[y][x].SetTileState(GAME_TILE_STATE::EMPTY);
				}
			}
			if (_currentInstance == 2)
			{
				if (_gameMatrix[y][x].GetTileState() == GAME_TILE_STATE::PLAYER2_STANDS && (y != zPosition || x != xPosition))
				{
					_gameMatrix[y][x].SetTileState(GAME_TILE_STATE::EMPTY);
				}
			}
		}
	}
}

void Player::UpdateEnemyGameOver()
{
	//Go through all exit codes 
	for (unsigned int i = 0; i < _itemStorage.size(); i++)
	{
		if (_itemStorage[i]->GetItemType() == GAME_ITEM::BOMB)
		{
			//If the player blew up himself
			if (_currentInstance == 1 && _itemStorage[i]->GetThreadExitStatus() == 1)
			{
				_winLoseStatus = -1;

				delete _itemStorage[i];
				_itemStorage.erase(_itemStorage.begin() + i, _itemStorage.begin() + i + 1);
			}
			//If the player killed the enemy player
			else if (_currentInstance == 1 && _itemStorage[i]->GetThreadExitStatus() == 2)
			{
				_winLoseStatus = 1;
				
				delete _itemStorage[i];
				_itemStorage.erase(_itemStorage.begin() + i, _itemStorage.begin() + i + 1);
			}
			//The same as above
			else if (_currentInstance == 2 && _itemStorage[i]->GetThreadExitStatus() == 2)
			{
				_winLoseStatus = -1;
				
				delete _itemStorage[i];
				_itemStorage.erase(_itemStorage.begin() + i, _itemStorage.begin() + i + 1);
			}
			else if (_currentInstance == 2 && _itemStorage[i]->GetThreadExitStatus() == 1)
			{
				_winLoseStatus = 1;
				
				delete _itemStorage[i];
				_itemStorage.erase(_itemStorage.begin() + i, _itemStorage.begin() + i + 1);
			}
			//If the bomb exploded and did not hit a player
			else if (_itemStorage[i]->GetThreadExitStatus() == 3)
			{
				//Start a screen shake so we get a visual feedback of the explosion
				std::thread Thread(&Player::ScreenShake, this);
				Thread.detach();

				delete _itemStorage[i];
				_itemStorage.erase(_itemStorage.begin() + i, _itemStorage.begin() + i + 1);
			}
			//If the thread is still running...
			else if (_itemStorage[i]->GetThreadExitStatus() == 0)
			{

			}
		}
	}
}

void Player::ScreenShake()
{
	std::cout << "Starting screen shake!" << std::endl;
	for (unsigned int i = 0; i < 20; i++)
	{
		int direction = rand() % 6;
		std::cout << direction << std::endl;
		for (unsigned int k = 0; k < 10; k++)
		{
			_manager->getActiveCamera()->updateAbsolutePosition();
			if (direction == 0)
			{
				_manager->getActiveCamera()->setTarget(irr::core::vector3df(_manager->getActiveCamera()->getTarget().X,
																			_manager->getActiveCamera()->getTarget().Y + 0.02f,
																			_manager->getActiveCamera()->getTarget().Z));

			}
			else if (direction == 1)
			{
				_manager->getActiveCamera()->setTarget(irr::core::vector3df(_manager->getActiveCamera()->getTarget().X,
																			_manager->getActiveCamera()->getTarget().Y - 0.02f,
																			_manager->getActiveCamera()->getTarget().Z));

			}
			else if (direction == 2)
			{
				_manager->getActiveCamera()->setTarget(irr::core::vector3df(_manager->getActiveCamera()->getTarget().X + 0.02f,
																			_manager->getActiveCamera()->getTarget().Y,
																			_manager->getActiveCamera()->getTarget().Z));

			}
			else if (direction == 3)
			{
				_manager->getActiveCamera()->setTarget(irr::core::vector3df(_manager->getActiveCamera()->getTarget().X - 0.02f,
																			_manager->getActiveCamera()->getTarget().Y,
																			_manager->getActiveCamera()->getTarget().Z));

			}
			else if (direction == 4)
			{
				_manager->getActiveCamera()->setTarget(irr::core::vector3df(_manager->getActiveCamera()->getTarget().X,
																			_manager->getActiveCamera()->getTarget().Y,
																			_manager->getActiveCamera()->getTarget().Z + 0.02f));

			}
			else if (direction == 5)
			{
				_manager->getActiveCamera()->setTarget(irr::core::vector3df(_manager->getActiveCamera()->getTarget().X,
																			_manager->getActiveCamera()->getTarget().Y,
																			_manager->getActiveCamera()->getTarget().Z - 0.02f));

			}
			Sleep(3);
		}

		Sleep(20);
	}
	_manager->getActiveCamera()->setTarget(irr::core::vector3df(0, 0, 0));
}