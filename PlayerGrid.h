#include <vector>
#include <utility>
#include "GameGrid.h"
#pragma once

class ComputerGrid;

class PlayerGrid : public GameGrid
{
public:
	PlayerGrid(int sDifficulty, bool displGrid)
		: GameGrid(sDifficulty, displGrid)
	{
		playerChooseShips();
		playerPlaceShips();
		placeBarrels();
	}
	void playerChooseShips();
	void playerPlaceShips();
	void playerFire(int&, int&);
	void fire(int, int, bool&);
	void updateTurnGrid(ComputerGrid&);
};