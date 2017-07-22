#include "GameGrid.h"
#pragma once

class PlayerGrid : public GameGrid
{
public:
	PlayerGrid(int sDifficulty, int destoyNum, int frigateNum, int battleNum)
		: GameGrid()
	{
		setDifficulty(sDifficulty);
		addShips(destoyNum, frigateNum, battleNum);
		playerPlaceShips();
		placeBarrels();
	}
	void playerPlaceShips();
	virtual void fire() override;
	//returns true if coordinates don't hit or miss again
	bool checkHit(int x, int y);
	void computerFire(int x, int y);
	virtual void printGrid() const override;
private:
	bool hitLastTurn = false;
	int consecutiveHits = 0;
	int totalHits = 0;
};