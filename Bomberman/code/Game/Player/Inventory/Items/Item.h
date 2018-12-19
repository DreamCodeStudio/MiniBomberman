#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Include tile class for working with the game matrix 
#include "..\..\..\World\Tile\Tile.h"

//For calling the InteractWithWorld method as a thread
#include <thread>

///<summary>
///The Item Base class defines all methods and members every item class should have
///</summary>

class Item
{
	public:

		virtual void Create(irr::scene::ISceneManager *manager, irr::core::vector3df playerPos, Tile **gameMatrix) = 0;

	protected:

		///<summary>The InteractWithWorld function will be called as a thread. This has the advantage, that items
		///			which take some time before having an effect on the world (e.g. bombs) can get implemented 
		///			very easily. Without calling this function as a thread the player class would need to call this
		///			function ones every frame, so it can check if it's time to manipulate the world matrix. In this
		///			case the function gets called as a thread from the Create method and the player class does not need
		///			to use it.</summary>
		virtual void InteractWithWorld(Tile **gameMatrix) = 0;

};