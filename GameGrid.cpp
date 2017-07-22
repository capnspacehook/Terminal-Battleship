#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "Utils.h"
#include "GameGrid.h"
#include "Destroyer.h"
#include "Frigate.h"
#include "Battleship.h"
using namespace std;

void GameGrid::setDifficulty(int sDifficulty)
{
	if (sDifficulty == 1)
	{
		difficulty = Utils::EASY;
		difficultySize = EASY_SIZE;
		grid.resize(EASY_SIZE, vector<char>(EASY_SIZE));
		fleetPos.resize(EASY_SIZE, vector<char>(EASY_SIZE));
	}
	else if (sDifficulty == 2)
	{
		difficulty = Utils::MEDIUM;
		difficultySize = MEDIUM_SIZE;
		grid.resize(MEDIUM_SIZE, vector<char>(MEDIUM_SIZE));
		fleetPos.resize(MEDIUM_SIZE, vector<char>(MEDIUM_SIZE));
	}
	else if (sDifficulty == 3)
	{
		difficulty = Utils::HARD;
		difficultySize = HARD_SIZE;
		grid.resize(HARD_SIZE, vector<char>(HARD_SIZE));
		fleetPos.resize(HARD_SIZE, vector<char>(HARD_SIZE));
	}
}

void GameGrid::addShips(int destoyNum, int frigateNum, int battleNum)
{
	if (destoyNum > 0)
	{
		setDestroyerNum(destoyNum);
		for (int i = 0; i < destoyNum; i++)
		{
			fleet.push_back(new Destroyer());
			shipLookup.push_back(Utils::DESTROYER);
		}
	}
	if (frigateNum > 0)
	{
		setFrigateNum(frigateNum);
		for (int i = 0; i < frigateNum; i++)
		{
			fleet.push_back(new Frigate());
			shipLookup.push_back(Utils::FRIGATE);
		}
	}
	if (battleNum > 0)
	{
		setBattleNum(battleNum);
		for (int i = 0; i < battleNum; i++)
		{
			fleet.push_back(new Battleship());
			shipLookup.push_back(Utils::BATTLESHIP);
		}
	}
}

void GameGrid::fillGrid()
{
	int diffSize = getDifficultySize();
	for (int y = 0; y < diffSize; y++)
		for (int x = 0; x < diffSize; x++)
		{
			grid[y][x] = '~';
			fleetPos[y][x] = '~';
		}
}

bool GameGrid::checkPlacement(Utils::ShipName ship, Utils::Orientation orientation, int x, int y) const
{
	int diffSize = getDifficultySize();
	bool validSpot = true;
	
	if (ship == Utils::DESTROYER)
	{
		if (orientation == Utils::HORIZONTAL)
		{
			if (x > (diffSize - DESTROYER_SIZE))
				validSpot = false;
			if (validSpot)
			{
				for (int i = x; i < x + DESTROYER_SIZE; i++)
				{
					if (grid[y][i] != '~')
						validSpot = false;
				}
			}
		}
		else if (orientation == Utils::VERTICAL)
		{
			if (y >(diffSize - DESTROYER_SIZE))
				validSpot = false;
			if (validSpot)
			{
				for (int i = y; i < y + DESTROYER_SIZE; i++)
				{
					if (grid[i][x] != '~')
						validSpot = false;
				}
			}
		}
	}
	else if (ship == Utils::FRIGATE)
	{
		if (orientation == Utils::HORIZONTAL)
		{
			if (x > (diffSize - FRIGATE_SIZE))
				validSpot = false;
			if (validSpot)
			{
				for (int i = x; i < x + FRIGATE_SIZE; i++)
				{
					if (grid[y][i] != '~')
						validSpot = false;
				}
			}
		}
		else if (orientation = Utils::VERTICAL)
		{
			if (y > (diffSize - FRIGATE_SIZE))
				validSpot = false;
			if (validSpot)
			{
				for (int i = y; i < y + FRIGATE_SIZE; i++)
				{
					if (grid[i][x] != '~')
						validSpot = false;
				}
			}
		}
	}
	else if (ship == Utils::BATTLESHIP)
	{
		if (orientation == Utils::HORIZONTAL)
		{
			if (x > (diffSize - HOR_BATTLESHIP_SIZE) || y > (diffSize - VER_BATTLESHIP_SIZE) || y == 9)
				validSpot = false;
			if (validSpot)
			{
				for (int i = x; i < x + HOR_BATTLESHIP_SIZE - 1; i++)
				{
					if (grid[y][i] != '~')
						validSpot = false;
				}
				for (int i = x; i < x + HOR_BATTLESHIP_SIZE; i++)
				{
					if (grid[y + 1][i] != '~')
						validSpot = false;
				}
			}
		}
		else if (orientation = Utils::VERTICAL)
		{
			if (y > (diffSize - HOR_BATTLESHIP_SIZE) || x > (diffSize - VER_BATTLESHIP_SIZE) || x == 9)
				validSpot = false;
			if (validSpot)
			{
				for (int i = y; i < y + HOR_BATTLESHIP_SIZE - 1; i++)
				{
					if (grid[i][x] != '~')
						validSpot = false;
				}
				for (int i = y; i < y + HOR_BATTLESHIP_SIZE; i++)
				{
					if (grid[i][x + 1] != '~')
						validSpot = false;
				}
			}
		}
	}
	return validSpot;
}

void GameGrid::placeShip(Utils::ShipName ship, Utils::Orientation ori, int x, int y, char& shipCounter)
{
	int diffSize = getDifficultySize();

	if (ship == Utils::DESTROYER)
	{
		shipCounter++;
		if (ori == Utils::HORIZONTAL)
			for (int i = x; i < x + DESTROYER_SIZE; i++)
			{
				grid[y][i] = 'D';
				fleetPos[y][i] = shipCounter;
			}
		else if (ori == Utils::VERTICAL)
			for (int i = y; i < y + DESTROYER_SIZE; i++)
			{
				grid[i][x] = 'D';
				fleetPos[i][x] = shipCounter;
			}
	}
	else if (ship == Utils::FRIGATE)
	{
		shipCounter++;
		if (ori == Utils::HORIZONTAL)
			for (int i = x; i < x + FRIGATE_SIZE; i++)
			{
				grid[y][i] = 'F';
				fleetPos[y][i] = shipCounter;
			}
		else if (ori == Utils::VERTICAL)
			for (int i = y; i < y + FRIGATE_SIZE; i++)
			{
				grid[i][x] = 'F';
				fleetPos[i][x] = shipCounter;
			}
	}
	else if (ship == Utils::BATTLESHIP)
	{
		if (ori == Utils::HORIZONTAL)
		{
			shipCounter++;
			for (int i = x; i < x + HOR_BATTLESHIP_SIZE - 1; i++)
			{
				grid[y][i] = 'B';
				fleetPos[y][i] = shipCounter;
			}
			for (int i = x; i < x + HOR_BATTLESHIP_SIZE; i++)
			{
				grid[y + 1][i] = 'B';
				fleetPos[y + 1][i] = shipCounter;
			}
		}
		else if (ori == Utils::VERTICAL)
		{
			shipCounter++;
			for (int i = y; i < y + HOR_BATTLESHIP_SIZE - 1; i++)
			{
				grid[i][x] = 'B';
				fleetPos[i][x] = shipCounter;
			}
			for (int i = y; i < y + HOR_BATTLESHIP_SIZE; i++)
			{
				grid[i][x + 1] = 'B';
				fleetPos[i][x + 1] = shipCounter;
			}
		}
	}
}	

void GameGrid::sinkShip(Utils::ShipName sunkShip)
{
	if (sunkShip == Utils::DESTROYER)
		setDestroyerNum(getDestroyerNum() - 1);
	else if (sunkShip == Utils::FRIGATE)
		setFrigateNum(getFrigateNum() - 1);
	else if (sunkShip == Utils::BATTLESHIP)
		setBattleNum(getBattleNum() - 1);

	Utils::printCenter("Ship Sunk!");
}

void GameGrid::placeBarrels()
{
	int x;
	int y;
	int barrlesToPlace;
	int maxBarrels;
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> coordinate{ 0, getDifficultySize() - 1 };

	if (getDifficulty() == Utils::EASY)
		maxBarrels = 2;
	else if (getDifficulty() == Utils::MEDIUM)
		maxBarrels = 3;
	else if (getDifficulty() == Utils::HARD)
		maxBarrels = 5;

	uniform_int_distribution<int> numbrOfBarrels{ 1, maxBarrels };
	barrlesToPlace = numbrOfBarrels(mt);

	for (int i = 0; i < barrlesToPlace; i++)
	{
		do {
			x = coordinate(mt);
			y = coordinate(mt);
		} while (getGridCell(x, y) != '~');
		setGridCell(x, y, '*');
	}
}

void GameGrid::barrelDetonation(int x, int y)
{
	setGridCell(x, y, '#');
	for (int i = y - 2; i < y + 3; i++)
	{
		for (int j = x - 2; j < x + 3; j++)
		{
			if (i < 0 || j < 0 || i > getDifficultySize() - 1 || j > getDifficultySize() - 1)
				continue;
			barrelFire(j, i);
		}
	}
}

void GameGrid::barrelFire(int x, int y)
{
	if (getGridCell(x, y) == '~')
		setGridCell(x, y, 'X');

	else if (getGridCell(x, y) != '~' && getGridCell(x, y) != 'X'
		&& getGridCell(x, y) != 'H' && getGridCell(x, y) != '*' && getGridCell(x, y) != '#')
	{
		int damagedShip = (int)(getFleetPosCell(x, y) - 65);
		if (getFleetCell(damagedShip)->shipSunk())
		{
			Utils::ShipName sunkShip = getShipLookupCell(damagedShip);
			sinkShip(sunkShip);
		}

		setGridCell(x, y, 'H');
		setFleetPosCell(x, y, 'H');
		setLastHit(x, y);
	}

	else if (getGridCell(x, y) == '*')
		barrelDetonation(x, y);
}