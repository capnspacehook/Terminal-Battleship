#include "GameGrid.h"
#include "Utils.h"
#pragma once

class ComputerGrid : public GameGrid
{
public:
	ComputerGrid(int sDifficulty, int shipPool) : GameGrid()
	{
		setDifficulty(sDifficulty);
		chooseRandomShips(shipPool);
		randomizePlacement();
		placeBarrels();
	}
	void chooseRandomShips(int);
	void randomizePlacement();
	void generatePosition(int&, int&, Utils::Orientation&);
	void randomOrientation(Utils::Orientation&);
	virtual void fire() override;
	virtual void printGrid() const override;
};