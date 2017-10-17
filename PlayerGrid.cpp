#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <functional>
#include <vector>
#include "Utils.h"
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "ComputerGrid.h"
using namespace std;

void PlayerGrid::playerChooseShips()
{
	int shipPool = getShipPool();
	bool poolEmpty = false;
	int shipChoice;
	int destroyNum = 0;
	int subNum = 0;
	int frigateNum = 0;
	int battleNum = 0;
	int carrierNum = 0;
	reference_wrapper<int> playerFleet[5] = { destroyNum, subNum, frigateNum,
		battleNum, carrierNum };
	int currentFleet = 0;

	Utils::clearScreen();
	while (!poolEmpty)
	{
		if (shipPool - currentFleet >= 2)
		{
			cout << "Choose your fleet:\n"
				<< "(1) Destroyers: " << DESTROYER_SIZE << " ship parts" << endl
				<< "(2) Submarines: " << SUBMARINE_SIZE << " ship parts" << endl
				<< "(3) Frigates: " << FRIGATE_SIZE << " ship parts" << endl
				<< "(4) Battleships: " << BATTLESHIP_SIZE << " ship parts" << endl
				<< "(5) Carriers: " << CARRIER_SIZE << " ship parts" << endl
				<< "\nYou have " << shipPool - currentFleet << " ship parts left." << "\n:";
			cin >> shipChoice;

			while (cin.fail() || shipChoice < 1 || shipChoice > 5)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "That is not a valid option." << endl
					<< "Please choose a ship for your fleet: ";
				cin >> shipChoice;
			}
			shipChoice--;

			if (shipPool - (currentFleet + SHIP_SIZES[shipChoice]) >= 0)
			{
				playerFleet[shipChoice]++;
				currentFleet += SHIP_SIZES[shipChoice];
				Utils::clearScreen();
			}
			else
			{
				Utils::clearScreen();
				shipCheck(poolEmpty, shipPool, false);
			}
		}
		else
			poolEmpty = true;
	}
	addShips(destroyNum, subNum, frigateNum, battleNum, carrierNum);
}

void PlayerGrid::playerPlaceShips()
{
	int x;
	int y;
	int usrOri;
	int diffSize = getDifficultySize();
	string shipType;
	bool badPlacement = false;
	Utils::Orientation orientation;
	char shipCounter = 64;

	for (int i = 0; i < getShipLookupSize(); i++)
	{
		do {
			if (!badPlacement)
				Utils::clearScreen();

			badPlacement = false;
			Utils::printCenter("Place your ships:\n", diffSize);
			printGrid();
			if (getShipLookupCell(i) == Utils::DESTROYER)
				shipType = "destroyer";
			else if (getShipLookupCell(i) == Utils::SUBMARINE)
				shipType = "submarine";
			else if (getShipLookupCell(i) == Utils::FRIGATE)
				shipType = "frigate";
			else if (getShipLookupCell(i) == Utils::BATTLESHIP)
				shipType = "battleship";
			else
				shipType = "carrier";

			cout << "What orientation would you like your " << shipType << " of size " 
				<< SHIP_SIZES[getShipLookupCell(i)] << " to be in?" << endl
				<< "(1)\t Horizontal\n(2)\t Vertical\n: ";
			cin >> usrOri;
			if ((usrOri != 1 && usrOri != 2) || cin.fail())
			{
				while ((usrOri != 1 && usrOri != 2) || cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Invalid orientation entered. Please enter again.\n";
					cin >> usrOri;
				}
			}
			if (usrOri == 1)
				orientation = Utils::HORIZONTAL;
			else if (usrOri == 2)
				orientation = Utils::VERTICAL;

			cout << "Where would you like to place your " << shipType << "?\nX Coordinate: ";
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

			if (checkPlacement(getShipLookupCell(i), orientation, x, y, false))
			{
				placeShip(getShipLookupCell(i), orientation, x, y, shipCounter);
				printGrid();
			}
			else
			{
				badPlacement = true;
				Utils::clearScreen();
				cout << "Invalid ship position entered. Please pick a new one.\n";

			}
		} while (badPlacement);
	}
}

void PlayerGrid::playerFire(int& x, int& y)
{
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
}

void PlayerGrid::fire(int x, int y, bool& hit)
{
	hit = false;
	int diffSize = getDifficultySize();
	Utils::clearScreen();
	cout << "Computer's Turn\n";

	if (getGridCell(x, y) == '~')
	{
		setGridCell(x, y, 'X');
		Utils::printCenter("MISS", diffSize);
	}

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
			Utils::printCenter("HIT!", diffSize);
		}
		setLastHit(x, y);
	}

	else if (getGridCell(x, y) == '*')
	{
		barrelDetonation(x, y, hit);
		Utils::printCenter("kaBOOOM!!", diffSize);
	}

	printGrid();
}

void PlayerGrid::updateTurnGrid(ComputerGrid& compGrid)
{
	for (int y = 0; y < getDifficultySize(); y++)
		for (int x = 0; x < getDifficultySize(); x++)
		{
			if (isShip(x, y) || getGridCell(x, y) == '*')
				compGrid.turnGrid[y][x] = '~';
			else
				compGrid.turnGrid[y][x] = getGridCell(x, y);
		}
}