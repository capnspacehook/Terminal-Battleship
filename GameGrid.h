#include <vector>
#include <utility>
#include "Ship.h"
#include "Utils.h"
#pragma once

class GameGrid
{
public:
	GameGrid(int sDifficulty, bool displGrid)
	{
		showGrid = displGrid;
		sDifficulty--;
		difficulty = DIFF_TYPES[sDifficulty];
		shipPool = POOL_SIZE[sDifficulty];
		difficultySize = DIFF_SIZE[sDifficulty];
		grid.resize(difficultySize, std::vector<char>(difficultySize));
		fleetPos.resize(difficultySize, std::vector<char>(difficultySize));
		fillGrid(grid, '~');
		fillGrid(fleetPos, '~');
	}
	~GameGrid()
	{
		for (auto &fleetRef : fleet)
			delete fleetRef;
	}
	Utils::Difficulty getDifficulty() const
	{
		return difficulty;
	}
	int getDifficultySize() const
	{
		return difficultySize;
	}
	int getShipPool() const
	{
		return shipPool;
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
	void setSubmarineNum(int subNum)
	{
		submarineNumber = subNum;
	}
	int getSubmarineNum() const
	{
		return submarineNumber;
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
	void setCarrierNum(int carrierNum)
	{
		carrierNumber = carrierNum;
	}
	int getCarrierNum() const
	{
		return carrierNumber;
	}
	void setLastHit(int x, int y)
	{
		lastHits.push_back(std::make_pair(x, y));
	}
	int getLastHitX(int i) const
	{
		return std::get<0>(lastHits[i]);
	}
	int getLastHitY(int i) const
	{
		return std::get<1>(lastHits[i]);
	}
	int getLastHitSize() const
	{
		return lastHits.size();
	}
	void eraseLastHit(int hitNum)
	{
		lastHits.erase(lastHits.begin() + --hitNum);
	}
	void clearLastHits()
	{
		lastHits.clear();
	}
	void addShips(int, int, int, int, int);
	template <typename T> void fillGrid(std::vector<std::vector<T>> &vec, T fillVal)
	{
		int diffSize = getDifficultySize();
		for (int y = 0; y < diffSize; y++)
			for (int x = 0; x < diffSize; x++)
				vec[y][x] = fillVal;
	}
	bool checkPlacement(Utils::ShipName, Utils::Orientation, int, int, bool) const;
	void placeShip(Utils::ShipName, Utils::Orientation, int, int, char&);
	void shipCheck(bool&, int, bool);
	bool isShip(int, int) const;
	void placeBarrels();
	void barrelDetonation(int x, int y, bool&);
	void barrelFire(int x, int y, bool&);
	virtual void printGrid() const;
	void sinkShip(Utils::ShipName, int);
	bool checkWin(bool& win) const
	{
		if (getDestroyerNum() == 0 && getSubmarineNum() == 0
			&& getFrigateNum() == 0 && getBattleNum() == 0 && getCarrierNum() == 0)
			win = true;
		return win;
	}

	//ship sizes
	static const int DESTROYER_SIZE = 2;
	static const int SUBMARINE_SIZE = 3;
	static const int FRIGATE_SIZE = 3;
	static const int BATTLESHIP_SIZE = 4;
	static const int CARRIER_SIZE = 5;
	const int SHIP_SIZES[5] = { DESTROYER_SIZE, SUBMARINE_SIZE,
		FRIGATE_SIZE, BATTLESHIP_SIZE, CARRIER_SIZE };
	static const int NUM_OF_SHIP_TYPES = 5;
	//chars that represent ships
	const char SHIP_LETTERS[5] = { 'D', 'U', 'F', 'B', 'C' };
	const Utils::ShipName SHIP_TYPES[5] = { Utils::DESTROYER,
	Utils::SUBMARINE, Utils::FRIGATE, Utils::BATTLESHIP, Utils::CARRIER };
	//grid sizes
	static const int EASY_SIZE = 8;
	static const int MEDIUM_SIZE = 10;
	static const int HARD_SIZE = 12;
	const int DIFF_SIZE[3] =
	{ EASY_SIZE, MEDIUM_SIZE, HARD_SIZE };
	//ship pool sizes
	static const int EASY_POOL = 12;
	static const int MEDIUM_POOL = 18;
	static const int HARD_POOL = 26;
	const int POOL_SIZE[3] =
	{ EASY_POOL, MEDIUM_POOL, HARD_POOL };
	//difficulty types
	const Utils::Difficulty DIFF_TYPES[3] =
	{ Utils::EASY, Utils::MEDIUM, Utils::HARD };
private:
	Utils::Difficulty difficulty;
	bool showGrid;
	int shipPool;
	int difficultySize;
	int destroyerNumber;
	int submarineNumber;
	int frigateNumber;
	int battleshipNumber;
	int carrierNumber;
	//stores coodinates of hits in order
	std::vector<std::pair<int, int>> lastHits;
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
