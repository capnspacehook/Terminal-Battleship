#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include "Utils.h"
#include "GameGrid.h"
#include "PlayerGrid.h"
using namespace std;

void PlayerGrid::playerPlaceShips()
{
	int x;
	int y;
	int usrOri;
	string shipType;
	bool badPlacement = false;
	Utils::Orientation orientation;
	char shipCounter = 64;

	fillGrid();

	for (int i = 0; i < getShipLookupSize(); i++)
	{
		do {
			if (!badPlacement)
				Utils::clearScreen();

			badPlacement = false;
			cout << endl;
			printGrid();
			if (getShipLookupCell(i) == Utils::DESTROYER)
				shipType = "destroyer";
			else if (getShipLookupCell(i) == Utils::FRIGATE)
				shipType = "frigate";
			else
				shipType = "battleship";

			cout << "What orientation would you like your " << shipType << " to be in?" << endl
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
			else if (usrOri == 1)
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

			if (checkPlacement(getShipLookupCell(i), orientation, x, y))
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

void PlayerGrid::fire()
{
	int x;
	int y;
	int chanceToHit;
	int previousHit = totalHits;
	//if computer is hitting consecutively, where the hit will be in reference to
	//the last hit
	static int smartHit;
	bool goodHit = false;
	bool canHitAgain = true;
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> probability{ 1, 100 };
	uniform_int_distribution<int> coordinate{ 0, getDifficultySize() - 1 };
	uniform_int_distribution<int> adjacentHit{ 1, 4 };
	int prob = probability(mt);

	if (getDifficulty() == Utils::EASY)
	{
		if (hitLastTurn)
			chanceToHit = 75;
		else
			chanceToHit = 15;
	}
	else if (getDifficulty() == Utils::MEDIUM)
	{
		if (hitLastTurn)
			chanceToHit = 85;
		else
			chanceToHit = 20;
	}
	else if (getDifficulty() == Utils::HARD)
	{
		if (hitLastTurn)
			chanceToHit = 95;
		else
			chanceToHit = 25;
	}

	if (prob <= chanceToHit)
	{
		if (hitLastTurn)
		{
			int hitSequentially = probability(mt);
			if (consecutiveHits == 0 || hitSequentially > pow((5 * consecutiveHits), 1.35) + 50)
				smartHit = adjacentHit(mt);

			//checks to see if it's possible to fire again on a coordinate adjecent to 
			//any of the previous hits
			while (!checkHit(1, previousHit) && !checkHit(2, previousHit)
				&& !checkHit(3, previousHit) && !checkHit(4, previousHit))
			{
				if (previousHit - 1 < 1)
				{
					canHitAgain = false;
					break;
				}
				else
					previousHit--;
			}

			if (canHitAgain)
			{
				while (!checkHit(smartHit, previousHit))
					smartHit = adjacentHit(mt);

				if (smartHit == 1)
				{
					x = getLastHitX(previousHit);
					y = getLastHitY(previousHit) + 1;
				}
				else if (smartHit == 2)
				{
					x = getLastHitX(previousHit) + 1;
					y = getLastHitY(previousHit);
				}
				else if (smartHit == 3)
				{
					x = getLastHitX(previousHit);
					y = getLastHitY(previousHit) - 1;
				}
				else if (smartHit == 4)
				{
					x = getLastHitX(previousHit) - 1;
					y = getLastHitY(previousHit);
				}
			}

			else
			{
				while (!goodHit)
				{
					x = coordinate(mt);
					y = coordinate(mt);
					if ((getGridCell(x, y) == 'D' || getGridCell(x, y) == 'F' || getGridCell(x, y) == 'B')
						&& (getGridCell(x, y) != 'H' && getGridCell(x, y) != 'X'))
						goodHit = true;
				}
			}
		}

		else
		{
			while (!goodHit)
			{
				x = coordinate(mt);
				y = coordinate(mt);
				if ((getGridCell(x, y) == 'D' || getGridCell(x, y) == 'F' || getGridCell(x, y) == 'B')
					&& (getGridCell(x, y) != 'H' && getGridCell(x, y) != 'X'))
					goodHit = true;
			}
		}
		computerFire(x, y);
	}
	else if (prob > chanceToHit)
	{
		while (!goodHit)
		{
			x = coordinate(mt);
			y = coordinate(mt);
			if ((getGridCell(x, y) != 'D' || getGridCell(x, y) != 'F' || getGridCell(x, y) != 'B')
				&& (getGridCell(x, y) != 'H' && getGridCell(x, y) != 'X'))
				goodHit = true;
		}
		computerFire(x, y);
	}
}

bool PlayerGrid::checkHit(int smartHit, int previousHit)
{
	int hitX = getLastHitX(previousHit);
	int hitY = getLastHitY(previousHit);
	bool validHit = false;

	if (smartHit == 1)
	{
		if (hitY + 1 <= getDifficultySize() - 1)
			if (getGridCell(hitX, hitY + 1) != 'X' && getGridCell(hitX, hitY + 1) != 'H')
				validHit = true;
	}

	else if (smartHit == 2)
	{
		if (hitX + 1 <= getDifficultySize() - 1)
			if (getGridCell(hitX + 1, hitY) != 'X' && getGridCell(hitX + 1, hitY) != 'H')
				validHit = true;
	}

	else if (smartHit == 3)
	{
		if (hitY - 1 >= 0)
			if (getGridCell(hitX, hitY - 1) != 'X' && getGridCell(hitX, hitY - 1) != 'H')
				validHit = true;
	}

	else if (smartHit == 4)
	{
		if (hitX - 1 >= 0)
			if (getGridCell(hitX - 1, hitY) != 'X' && getGridCell(hitX - 1, hitY) != 'H')
				validHit = true;
	}

	return validHit;
}

void PlayerGrid::computerFire(int x, int y)
{
	Utils::clearScreen();
	cout << "Computer's Turn\n";

	if (getGridCell(x, y) == '~')
	{
		setGridCell(x, y,'X');
		Utils::printCenter("MISS");
		//cout << setw(centerW) << "MISS";
		//hitLastTurn = false;
		consecutiveHits = 0;
	}

	else if (getGridCell(x, y) != '~' && getGridCell(x, y) != 'X' && getGridCell(x, y) != 'H')
	{
		int damagedShip = (int)(getFleetPosCell(x, y) - 65);
		getFleetCell(damagedShip)->shipHit();
		if (getFleetCell(damagedShip)->shipSunk())
		{
			Utils::ShipName sunkShip = getShipLookupCell(damagedShip);
			sinkShip(sunkShip);
			hitLastTurn = false;
		}
		else
		{
			Utils::printCenter("HIT!");
			hitLastTurn = true;
		}
		
		if (hitLastTurn)
			consecutiveHits++;

		setGridCell(x, y, 'H');
		setFleetPosCell(x, y, 'H');
		setLastHit(x, y);
		totalHits++;
	}

	else if (getGridCell(x, y) == '*')
	{
		barrelDetonation(x, y);
		Utils::printCenter("kaBOOOM!!");
	}

}

void PlayerGrid::printGrid() const
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