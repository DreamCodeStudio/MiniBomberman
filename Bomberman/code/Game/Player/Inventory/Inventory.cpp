#include "Inventory.h"

Inventory::Inventory(irr::scene::ISceneManager *manager, irr::gui::IGUIEnvironment *gui, int playerInstance)
{
	//Create gui with bomb counter and collected powerups
	_gui = gui;

	//Create item counter. For every item e.g. bombs a counter which shows how many items of this type are in the inventory
	
	switch (playerInstance)
	{
		case 1: {
			
			//Add an label, so the players know which item list belongs to which player
			_label = _gui->addStaticText(irr::core::stringw("Player 1").c_str(), irr::core::recti(10, 10, 100, 30));

			//Add an Image and a counter for each item

			//##### Bombs ##### 
			_bombCounter = 5;
			
			//The counter which is displayed on the screen
			_bombCounterText = _gui->addStaticText(irr::core::stringw(this->IntToString(_bombCounter).c_str()).c_str(),
													irr::core::recti(80, 50, 112, 80));

			//The bomb icon
			_bombImage = _gui->addImage(irr::core::recti(10, 50, 74, 114), 0, -1, 0, true);
			_bombImage->setImage(manager->getVideoDriver()->getTexture("Assets\\Textures\\Bomb.jpg"));
			_bombImage->setScaleImage(true);
		}
		case 2: {

			//Add an label, so the players know which item list belongs to which player
			_label = _gui->addStaticText(irr::core::stringw("Player 2").c_str(), irr::core::recti(1800, 10, 1900, 30));

			//Add an Image and a counter for each item

			//##### Bombs ##### 
			_bombCounter = 5;

			//The counter which is displayed on the screen
			_bombCounterText = _gui->addStaticText(irr::core::stringw(this->IntToString(_bombCounter).c_str()).c_str(),
				irr::core::recti(1800, 50, 1832, 80));

			//The bomb icon
			_bombImage = _gui->addImage(irr::core::recti(1840, 50, 1904, 114), 0, -1, 0, true);
			_bombImage->setImage(manager->getVideoDriver()->getTexture("Assets\\Textures\\Bomb.jpg"));
			_bombImage->setScaleImage(true);
		}
	}
}

Inventory::Inventory()
{

}

void Inventory::AddItem(GAME_ITEM item)
{
	//Add an item to the item counter
	switch (item)
	{
		case GAME_ITEM::BOMB: {
			_bombCounter++;			//Add the item
			_bombCounterText->setText(irr::core::stringw(this->IntToString(_bombCounter).c_str()).c_str()); //Refresh the displayed text
		}
			break;
	}
}

bool Inventory::hasItem(GAME_ITEM item)
{
	//Search for an item
	switch (item)
	{
		case GAME_ITEM::BOMB: {	

			if (_bombCounter)	//Check if the counter is not null
			{
				return true;	//If the counter is not null -> the player has the item -> return true
			}
		}
			break;
	}
		
	return false;		//Otherwise return false
}

/* ############## Private ################ */

std::string Inventory::IntToString(int number)
{
	//Konvert int number into string
	std::string resultStr;
	std::stringstream str;
	str << number;
	str >> resultStr;

	return resultStr;	//Return the string
}