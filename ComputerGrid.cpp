#include <iostream>
#include <random>
#include "Utils.h"
#include "GameGrid.h"
#include "ComputerGrid.h"
using namespace std;

void ComputerGrid::chooseRandomShips(int shipPool)
{
	int destoyNum = 0;
	int frigateNum = 0;
	int battleNum = 0;
	int currentFleet = 0;
	bool poolEmpty = false;
	int shipChoice;
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<unsigned int> randomInt{ 0, 2 };

	while (!poolEmpty)
	{
		shipChoice = randomInt(mt);

		if (shipPool - currentFleet >= 3)
		{
			if (shipChoice == Utils::DESTROYER)
			{
				destoyNum++;
				currentFleet += DESTROYER_SIZE;
			}
			else if (shipChoice == Utils::FRIGATE)
			{
				if (shipPool - (currentFleet + FRIGATE_SIZE) >= 0)
				{
					frigateNum++;
					currentFleet += FRIGATE_SIZE;
				}
				else
					if (shipPool - DESTROYER_SIZE <= 0 && shipPool - FRIGATE_SIZE <= 0 && shipPool - BATTLESHIP_SIZE <= 0)
						poolEmpty = true;
			}
			else if (shipChoice == Utils::BATTLESHIP)
				if (shipPool - (currentFleet + BATTLESHIP_SIZE) >= 0)
				{
					battleNum++;
					currentFleet += BATTLESHIP_SIZE;
				}
				else
					if (shipPool - DESTROYER_SIZE <= 0 && shipPool - FRIGATE_SIZE <= 0 && shipPool - BATTLESHIP_SIZE <= 0)
						poolEmpty = true;
		}
		else
			poolEmpty = true;
	}
	addShips(destoyNum, frigateNum, battleNum);
}

void ComputerGrid::randomizePlacement()
{
	int x;
	int y;
	char shipCounter = 64;
	Utils::Orientation shipOrientation;

	fillGrid();

	for (int i = 0; i < getDestroyerNum(); i++)
	{
		do {
			generatePosition(x, y, shipOrientation);
		} while (!checkPlacement(Utils::DESTROYER, shipOrientation, x, y));
		placeShip(Utils::DESTROYER, shipOrientation, x, y, shipCounter);
	}

	for (int i = 0; i < getFrigateNum(); i++)
	{
		do {
			generatePosition(x, y, shipOrientation);
		} while (!checkPlacement(Utils::FRIGATE, shipOrientation, x, y));
		placeShip(Utils::FRIGATE, shipOrientation, x, y, shipCounter);
	}

	for (int i = 0; i < getBattleNum(); i++)
	{
		do {
			generatePosition(x, y, shipOrientation);
		} while (!checkPlacement(Utils::BATTLESHIP, shipOrientation, x, y));
		placeShip(Utils::BATTLESHIP, shipOrientation, x, y, shipCounter);
	}
}

void ComputerGrid::generatePosition(int& x, int& y, Utils::Orientation& ori)
{
	random_device rd;
	mt19937 mt(rd());

	uniform_int_distribution<int> randomInt{ 1, getDifficultySize() - 1 };
	x = randomInt(mt);
	y = randomInt(mt);

	randomOrientation(ori);
}

void ComputerGrid::randomOrientation(Utils::Orientation& shipOrientation)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<unsigned int> orientation{ 1, 2 };
	int o = orientation(mt);
	if (o == 1)
		shipOrientation = Utils::HORIZONTAL;
	else
		shipOrientation = Utils::VERTICAL;
}

void ComputerGrid::fire()
{
	int x, y;
	int centerW = (getDifficultySize() * 2) - (getDifficultySize() / 4) - 1;

	cout << "Enter coordinates to fire upon\nX Coordinate: ";
	cin >> x;
	if (x > getDifficultySize() || x < 1 || cin.fail())
	{
		while (x > getDifficultySize() || x < 1 || cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid coordinate entered. Please enter again.\n";
			cin >> x;
		}
	}
	cout << "Y Coordinate: ";
	cin >> y;
	if (y > getDifficultySize() || y < 1 || cin.fail())
	{
		while (y > getDifficultySize() || y < 1 || cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid coordinate entered. Please enter again.\n";
			cin >> y;
		}
	}

	x--;
	y--;
	Utils::clearScreen();
	cout << "Player's Turn\n";

	if (getGridCell(x, y) == '~')
	{
		setGridCell(x, y, 'X');
		Utils::printCenter("MISS");
	}

	else if (getGridCell(x, y) == 'X')
		Utils::printCenter("Missed again...");

	else if (getGridCell(x, y) != '~' && getGridCell(x, y) != 'X' 
		&& getGridCell(x, y) != 'H' && getGridCell(x, y) != '*' && getGridCell(x, y) != '#')
	{
		int damagedShip = (int)(getFleetPosCell(x, y) - 65);
		if (getFleetCell(damagedShip)->shipSunk())
		{
			Utils::ShipName sunkShip = getShipLookupCell(damagedShip);
			sinkShip(sunkShip);
		}
		else
			Utils::printCenter("HIT!");

		setGridCell(x, y, 'H');
		setFleetPosCell(x, y, 'H');
		setLastHit(x, y);
	}

	else if (getGridCell(x, y) == 'H')
		Utils::printCenter("Already hit...");

	else if (getGridCell(x, y) == '*')
	{
		barrelDetonation(x, y);
		Utils::printCenter("kaBOOOM!!");
	}
}

void ComputerGrid::printGrid() const
{
	int diffSize = getDifficultySize();
	int vertNumber = diffSize;
	int counter = 0;

	cout << endl;
	for (int y = diffSize - 1; y >= 0; y--)
	{
		for (int x = 0; x < diffSize; x++)
		{
			if (counter == 0 && vertNumber < 10)
				cout << " " << vertNumber;
			else if (counter == 0 && vertNumber >= 10)
				cout << vertNumber;

			if (getGridCell(x, y) != 'H' && getGridCell(x, y) != 'X' && getGridCell(x, y) != '#')
				cout << "  " << '~';
			else 
				cout << "  " << getGridCell(x, y);
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