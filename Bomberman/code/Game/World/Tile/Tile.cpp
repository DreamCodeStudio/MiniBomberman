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
	if (tileState == GAME_TILE_STATE::UNDESTRUCTABLE)
	{
		_tileModel = _manager->addAnimatedMeshSceneNode(_manager->getMesh("Assets\\Models\\UndestructableTile\\Tile.obj")); //Load model
	}
	else
	{
		_tileModel = _manager->addAnimatedMeshSceneNode(_manager->getMesh("Assets\\Models\\Tile\\Tile.obj")); //Load model
	}
	_tileModel->setPosition(_tilePosition);	//Set position

	//Load the model according to the TileState
	this->SetTileState(tileState);
}

void Tile::SetTileState(GAME_TILE_STATE tileState)
{
	//If the current tile state is undestructable -> the tile state can not be changed or influenced in any way
	if (_currentGameTileState == GAME_TILE_STATE::UNDESTRUCTABLE)
	{
		return;
	}

	switch (tileState)
	{
		case GAME_TILE_STATE::BLOCKED: {

			//Set the model visible and change the tile state
			_tileModel->setVisible(true);
			_currentGameTileState = tileState;		//Save updated tile state
		}
			break;
		case GAME_TILE_STATE::EMPTY: {

			_currentGameTileState = tileState;	
		}
			break;
		case GAME_TILE_STATE::DESTROY: {
			//!!!!!!!!!!
			//Note that DESTROY does the same as EMPTY: It sets the TILE_STATE to EMPTY
			//The only difference is, that DESTROY also removes the mesh graphically
			//We need this because tiles often change the state between PLAYER_STANDS and empty (if a player walks over them)
			//But we do not want the setVisible() method called everytime. We only want it to be called when the Bomb class actually
			//removes anything. So the bomb class calls the DESTROY state, which in the end leads to the same result: an EMPTY tile

			//Set the model to invisible and change the tile state
			_tileModel->setVisible(false);
			_currentGameTileState = GAME_TILE_STATE::EMPTY;
		}
			break;
		case GAME_TILE_STATE::MARKED: {
			//!!!!!!!!!!!!!!!!!!!!!!!!!//
			//!!!!! Note  that the MARKED tile state is not a "real" tile state. It does not overwrite
			//!!!!! the _currentGameTileState member. It only changes the graphical appereance of the 
			//!!!!! tile Model. (After the bomb exploded all marked tiles will be set to empty anyways)
			//!!!!!!!!!!!!!!!!!!!!!!!!!//

			//Change the color of the block so the blocked is marked, and the player knows that this block is effected by a following 
			//explosion

			//If a tile with a solid block is under explosion -> make the block transparent
			if (_tileModel->isVisible() && _tileModel->getMesh() == _manager->getMesh("Assets\\Models\\Tile\\Tile.obj"))
			{
				_tileModel->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
			}
			else //A empty Tile can also be under explosion 
			{
				//Load a tile model which indicates that this tile is under explosion
				_tileModel->remove();
				_tileModel = _manager->addAnimatedMeshSceneNode(_manager->getMesh("Assets\\Models\\MarkedTile\\MarkedTile.obj"), 0, -1,
																_tilePosition);
			}
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