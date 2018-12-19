#pragma once
#include <iostream>

//3D-Engine
#include <irrlicht.h>

//Inerhitance
#include "..\Item.h"

//The delay before exploding
#include <Windows.h>

///<summary>
///The Bomb class is handling the bomb item. So the player can use bombs to interact and change the ingame world
///</summary>

class Bomb : public Item
{
	public:

		void Create(irr::scene::ISceneManager *manager, irr::core::vector3df playerPos, Tile **gameMatrix) override;
		void InteractWithWorld(Tile **gameMatrix) override;

	private:

		irr::scene::IAnimatedMeshSceneNode *_bombMesh;
};
