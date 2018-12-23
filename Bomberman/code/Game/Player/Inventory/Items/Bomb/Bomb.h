#pragma once

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

	protected:

		void InteractWithWorld(Tile **gameMatrix) override;

		//Only implemented in the bomb class
		void SetBombEffectedTiles(Tile **gameMatrix, GAME_TILE_STATE state);
		void CheckForPlayerHit(Tile **gameMatrix);

		irr::scene::IAnimatedMeshSceneNode *_bombMesh;
		irr::scene::IParticleSystemSceneNode *_particleSystem;
		irr::scene::IParticleEmitter *_particleEmitter;



};
