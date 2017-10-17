#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "Utils.h"
#include "GameGrid.h"
#include "Destroyer.h"
#include "Submarine.h"
#include "Frigate.h"
#include "Battleship.h"
#include "Carrier.h"
using namespace std;

void GameGrid::addShips(int destoyNum, int subNum, int frigateNum, int battleNum, int carrierNum)
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
	if (subNum > 0)
	{
		setSubmarineNum(subNum);
		for (int i = 0; i < subNum; i++)
		{
			fleet.push_back(new Submarine());
			shipLookup.push_back(Utils::SUBMARINE);
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
	if (carrierNum > 0)
	{
		setCarrierNum(carrierNum);
		for (int i = 0; i < carrierNum; i++)
		{
			fleet.push_back(new Carrier());
			shipLookup.push_back(Utils::CARRIER);
		}
	}
}

void GameGrid::shipCheck(bool& empty, int pool, bool isComputer)
{
	int counter = 0;
	for (int i = 0; i < NUM_OF_SHIP_TYPES; i++)
	{
		if (pool - SHIP_SIZES[i] <= 0)
			counter++;
	}
	if (counter == NUM_OF_SHIP_TYPES)
		empty = true;
	if (empty && !isComputer)
		cout << "Ship cannot be added. Please choose a smaller ship.\n";
}

bool GameGrid::isShip(int x, int y) const
{
	bool cellIsShip = false;
	for (int i = 0; i < NUM_OF_SHIP_TYPES; i++)
	{
		if (getGridCell(x, y) == SHIP_LETTERS[i])
		{
			cellIsShip = true;
			break;
		}
	}
	return cellIsShip;
}

bool GameGrid::checkPlacement(Utils::ShipName ship, Utils::Orientation orientation, int x, int y, bool hardPlace) const
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> adjPlace{ 1, 100 };
	int ajdPlaceChance = adjPlace(mt);
	int diffSize = getDifficultySize();
	bool validSpot = true;
	int shipSize = SHIP_SIZES[(int)ship];

	if (orientation == Utils::HORIZONTAL)
	{
		if (x > (diffSize - shipSize))
			validSpot = false;
		if (validSpot)
		{
			for (int i = x; i < x + shipSize; i++)
			{
				if (grid[y][i] != '~')
					validSpot = false;
			}
		}
	}
	else if (orientation == Utils::VERTICAL)
	{
		if (y >(diffSize - shipSize))
			validSpot = false;
		if (validSpot)
		{
			for (int i = y; i < y + shipSize; i++)
			{
				if (grid[i][x] != '~')
					validSpot = false;
			}
		}
	}
	 
	if (validSpot && hardPlace && ajdPlaceChance < 75)
	{
		if (orientation == Utils::HORIZONTAL)
		{
			for (int i = x; i < x + shipSize; i++)
			{
				if (i + 1 < diffSize)
				{
					if (isShip(i + 1, y))
					{
						validSpot = false;
						break;
					}
				}
				if (y + 1 < diffSize)
				{
					if (isShip(i, y + 1))
					{
						validSpot = false;
						break;
					}
				}
				if (i - 1 > 0)
				{
					if (isShip(i - 1, y))
					{
						validSpot = false;
						break;
					}
				}
				if (y - 1 > 0)
				{
					if (isShip(i, y - 1))
					{
						validSpot = false;
						break;
					}
				}
			}
		}
		else if (orientation == Utils::VERTICAL)
		{
			for (int i = y; i < y + shipSize; i++)
			{
				if (i + 1 < diffSize)
				{
					if (isShip(x, i + 1))
					{
						validSpot = false;
						break;
					}
				}
				if (x + 1 < diffSize)
				{
					if (isShip(x + 1, i))
					{
						validSpot = false;
						break;
					}
				}
				if (i - 1 > 0)
				{
					if (isShip(x, i - 1))
					{
						validSpot = false;
						break;
					}
				}
				if (x - 1 > 0)
				{
					if (isShip(x - 1, i))
					{
						validSpot = false;
						break;
					}
				}
			}
		}	
	}
	return validSpot;
}

void GameGrid::placeShip(Utils::ShipName ship, Utils::Orientation ori, int x, int y, char& shipCounter)
{
	int diffSize = getDifficultySize();
	int shipSize = SHIP_SIZES[(int)ship];
	char shipChar = SHIP_LETTERS[(int)ship];

	shipCounter++;
	if (ori == Utils::HORIZONTAL)
		for (int i = x; i < x + shipSize; i++)
		{
			grid[y][i] = shipChar;
			fleetPos[y][i] = shipCounter;
		}
	else if (ori == Utils::VERTICAL)
		for (int i = y; i < y + shipSize; i++)
		{
			grid[i][x] = shipChar;
			fleetPos[i][x] = shipCounter;
		}
}

void GameGrid::sinkShip(Utils::ShipName sunkShipName, int sunkShip)
{
	int x;
	int y;

	if (sunkShipName == Utils::DESTROYER)
		setDestroyerNum(getDestroyerNum() - 1);
	else if (sunkShipName == Utils::SUBMARINE)
		setSubmarineNum(getSubmarineNum() - 1);
	else if (sunkShipName == Utils::FRIGATE)
		setFrigateNum(getFrigateNum() - 1);
	else if (sunkShipName == Utils::BATTLESHIP)
		setBattleNum(getBattleNum() - 1);
	else
		setCarrierNum(getCarrierNum() - 1);

	for (int y = 0; y < fleetPos.size(); y++)
	{
		for (int x = 0; x < fleetPos.size(); x++)
		{
			if (getFleetPosCell(x, y) == (char)sunkShip)
			{
				setGridCell(x, y, 'S');
			}
		}
	}

	for (int i = 1; i < lastHits.size(); i++)
	{
		x = getLastHitX(i);
		y = getLastHitY(i);
		if (getFleetPosCell(x, y) == 'S')
			eraseLastHit(i);
	}
	Utils::printCenter("Ship Sunk!", getDifficultySize());
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
		maxBarrels = 1;
	else if (getDifficulty() == Utils::MEDIUM)
		maxBarrels = 2;
	else if (getDifficulty() == Utils::HARD)
		maxBarrels = 3;

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

void GameGrid::barrelDetonation(int x, int y, bool &hit)
{
	setGridCell(x, y, '#');
	for (int i = y - 2; i <= y + 2; i++)
	{
		for (int j = x - 2; j <= x + 2; j++)
		{
			if (i < 0 || j < 0 || i > getDifficultySize() - 1 || j > getDifficultySize() - 1)
				continue;
			barrelFire(j, i, hit);
		}
	}
}

void GameGrid::barrelFire(int x, int y, bool &hit)
{
	static bool explosionHit = false;
	hit = false;

	if (getGridCell(x, y) == '~')
		setGridCell(x, y, 'X');

	else if (isShip(x, y))
	{
		hit = true;
		int damagedShip = (int)(getFleetPosCell(x, y) - 65);
		getFleetCell(damagedShip)->shipHit();
		if (getFleetCell(damagedShip)->shipSunk())
		{
			Utils::ShipName sunkShip = getShipLookupCell(damagedShip);
			damagedShip = (int)(getFleetPosCell(x, y));
			setGridCell(x, y, 'S');
			sinkShip(sunkShip, damagedShip);
		}
		else
		{
			setGridCell(x, y, 'H');
			setLastHit(x, y);
		}
	}

	else if (getGridCell(x, y) == '*')
		barrelDetonation(x, y, hit);

	if (!explosionHit)
		explosionHit = hit;

	hit = explosionHit;
}	

void GameGrid::printGrid() const
{
	int diffSize = getDifficultySize();
	int vertNumber = diffSize;
	int counter = 0;

	cout << "\n";
	for (int y = diffSize - 1; y >= 0; y--)
	{
		for (int x = 0; x < diffSize; x++)
		{
			if (counter == 0 && vertNumber < 10)
				cout << " " << vertNumber;
			else if (counter == 0 && vertNumber >= 10)
				cout << vertNumber;

			if (showGrid)
				cout << "  " << getGridCell(x, y);
			else
			{
				if (isShip(x, y) || getGridCell(x, y) == '*')
					cout << "  " << '~';
				else
					cout << "  " << getGridCell(x, y);
			}
			counter++;
		}
		cout << endl;
		counter = 0;
		vertNumber--;
	}

	cout << "    ";
	for (int n = 1; n < diffSize + 1; n++)
	{
		if (n < 9)
			cout << n << "  ";
		else
			cout << n << " ";
	}
	cout << "\n\n";
}