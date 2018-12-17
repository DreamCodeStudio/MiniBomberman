#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//For converting integers (item counters) to strings (display text)
#include <sstream>

//Defintions for game states, tile states
#include "..\..\Definitions.h"

///<summary>
///The Inventory class handles all items the player collects during the game.
///e.g. powerups, bombs 
///</summary>

class Inventory
{
	public:

		///<summary>The Inventory constructor creates the gui which shows the player the selected items</summary>
		Inventory(irr::scene::ISceneManager *manager, irr::gui::IGUIEnvironment *gui, int playerInstance);
		Inventory();

		void AddItem(GAME_ITEM item);
		bool hasItem(GAME_ITEM item);

	private:

		std::string IntToString(int number);

		irr::gui::IGUIEnvironment *_gui; //The GUI Environment for displaying an item list for every player

		//Inventory images, and counters
		irr::gui::IGUIStaticText *_label;

		//Bombs
		irr::gui::IGUIImage *_bombImage;
		irr::gui::IGUIStaticText *_bombCounterText;
		int _bombCounter;
};