#pragma once

//Define the different GAME_STATES
enum GAME_STATE {
	RUN,
	MENU,
};

//Define different game tile states
enum GAME_TILE_STATE {
	BLOCKED,
	EMPTY,
	DESTROY,
	MARKED,
	PLAYER1_STANDS,
	PLAYER2_STANDS
};

//Define items
enum GAME_ITEM{
	BOMB,
};
