#include "Tile.h"

void Tile::Create(irr::scene::ISceneManager *manager, irr::core::vector3df tilePosition, GAME_TILE_STATE tileState)
{
	//Save pointer to the scene manager so we are able to change the 3D-Model and the state of tile at any run time
	_manager = manager;
	
	//Set the position of the tile
	_tilePosition = tilePosition;

	//Save tile state
	_currentGameTileState = tileState;

	//Load tile model
	_tileModel = _manager->addAnimatedMeshSceneNode(_manager->getMesh("Assets\\Models\\Tile\\Tile.obj")); //Load model
	_tileModel->setPosition(_tilePosition);	//Set position

	//Load the model according to the TileState
	this->SetTileState(tileState);
}

void Tile::SetTileState(GAME_TILE_STATE tileState)
{
	switch (tileState)
	{
		case GAME_TILE_STATE::BLOCKED: {

			//Set the model visible and change the tile state
			_tileModel->setVisible(true);
			_currentGameTileState = tileState;		//Save updated tile state
		}
			break;
		case GAME_TILE_STATE::EMPTY: {

			//Set the model to invisible and change the tile state
			_tileModel->setVisible(false);
			_currentGameTileState = tileState;
			
		}
			break;
		case GAME_TILE_STATE::PLAYER1_STANDS: {
			
			_currentGameTileState = tileState;
		}
			break;
		case GAME_TILE_STATE::PLAYER2_STANDS: {

			_currentGameTileState = tileState;
		}
			break;
	}
}

GAME_TILE_STATE Tile::GetTileState()
{
	return _currentGameTileState;
}