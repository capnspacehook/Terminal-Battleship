#include <vector>
#include "Ship.h"
#include "Utils.h"
#pragma once

class GameGrid
{
public:
	~GameGrid()
	{
		for (auto &fleetRef : fleet)
			delete fleetRef;
	}
	void setDifficulty(int sDifficulty);
	Utils::Difficulty getDifficulty() const
	{
		return difficulty;
	}
	int getDifficultySize() const
	{
		return difficultySize;
	}
	void setGridCell(int x, int y, char c)
	{
		grid[y][x] = c;
	}
	char getGridCell(int x, int y) const
	{
		return grid[y][x];
	}
	char getFleetPosCell(int x, int y) const
	{
		return fleetPos[y][x];
	}
	void setFleetPosCell(int x, int y, char c)
	{
		fleetPos[y][x] = c;
	}
	Ship* getFleetCell(int ship) const
	{
		return fleet[ship];
	}
	Utils::ShipName getShipLookupCell(int ship) const
	{
		return shipLookup[ship];
	}
	int getShipLookupSize() const
	{
		return shipLookup.size();
	}
	void setDestroyerNum(int destroyNum)
	{
		destroyerNumber = destroyNum;
	}
	int getDestroyerNum() const
	{
		return destroyerNumber;
	}
	void setFrigateNum(int frigateNum)
	{
		frigateNumber = frigateNum;
	}
	int getFrigateNum() const
	{
		return frigateNumber;
	}
	void setBattleNum(int battleNum)
	{
		battleshipNumber = battleNum;
	}
	int getBattleNum() const
	{
		return battleshipNumber;
	}
	void setLastHit(int x, int y)
	{
		lastHitsX.push_back(x);
		lastHitsY.push_back(y);
	}
	int getLastHitX(int hitNum) const
	{
		return lastHitsX[hitNum - 1];
	}
	int getLastHitY(int hitNum) const
	{
		return lastHitsY[hitNum - 1];
	}
	void addShips(int, int, int);
	void fillGrid();
	bool checkPlacement(Utils::ShipName, Utils::Orientation, int, int) const;
	void placeShip(Utils::ShipName, Utils::Orientation, int, int, char&);
	void placeBarrels();
	void barrelDetonation(int x, int y);
	void barrelFire(int x, int y);
	virtual void printGrid() const = 0;
	virtual void fire() = 0;
	void sinkShip(Utils::ShipName);
	bool checkWin(bool& win) const
	{
		if (this->getDestroyerNum() == 0 && this->getFrigateNum() == 0 && this->getBattleNum() == 0)
			win = true;
		return win;
	}

	static const int DESTROYER_SIZE = 3;
	static const int FRIGATE_SIZE = 6;
	static const int BATTLESHIP_SIZE = 9;
	static const int HOR_BATTLESHIP_SIZE = 5;
	static const int VER_BATTLESHIP_SIZE = 2;
	//grid sizes
	static const int EASY_SIZE = 15;
	static const int MEDIUM_SIZE = 20;
	static const int HARD_SIZE = 25;
private:
	Utils::Difficulty difficulty;
	int difficultySize;
	int destroyerNumber;
	int frigateNumber;
	int battleshipNumber;
	//stores coodinates of hits in order 
	std::vector<int> lastHitsX;
	std::vector<int> lastHitsY;
	//stores pointers to ship objects
	std::vector<Ship*> fleet;
	//grid in which the ships, hits, and misses are marked
	std::vector<std::vector<char>> grid;
	//vector for storing ships locations. Allows program to track if 
	//specific ships are sunk
	std::vector<std::vector<char>> fleetPos;
	//works with fleetPos to detirmine what kind of ship is in a certain
	//spot in fleetPos
	std::vector<Utils::ShipName> shipLookup;
};