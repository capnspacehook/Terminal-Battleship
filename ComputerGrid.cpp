#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <functional>
#include "Utils.h"
#include "GameGrid.h"
#include "ComputerGrid.h"
using namespace std;

void ComputerGrid::buildParityGrid()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> polarity{ 0, 1 };
	int diffSize = getDifficultySize();
	int smallestShip = SHIP_SIZES[0];

	for (int y = 0; y < diffSize; y++)
		for (int x = y % smallestShip; x < diffSize; x += smallestShip)
			parityGrid[y][x] = 1;

	//randomly choose between parities
	if (polarity(mt) == 1)
	{
		for (int y = 0; y < diffSize; y++)
			for (int x = 0; x < diffSize; x++)
			{
				if (parityGrid[y][x] == 0)
					parityGrid[y][x] = 1;
				else
					parityGrid[y][x] = 0;
			}
	}
}

void ComputerGrid::chooseRandomShips()
{
	int shipPool = getShipPool();
	int destroyNum = 0;
	int subNum = 0;
	int frigateNum = 0;
	int battleNum = 0;
	int carrierNum = 0;
	reference_wrapper<int> playerFleet[5] = { destroyNum, subNum, frigateNum,
		battleNum, carrierNum };
	int currentFleet = 0;
	bool poolEmpty = false;
	int shipChoice;
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<unsigned int> randomInt{ 0, 4 };

	while (!poolEmpty)
	{
		shipChoice = randomInt(mt);
		if (shipPool - currentFleet >= 2)
		{
			if (shipPool - (currentFleet + SHIP_SIZES[shipChoice]) >= 0)
			{
				playerFleet[shipChoice]++;
				currentFleet += SHIP_SIZES[shipChoice];
			}
			else
				shipCheck(poolEmpty, shipPool, true);
		}
		else
			poolEmpty = true;
	}
	addShips(destroyNum, subNum, frigateNum, battleNum, carrierNum);
}

void ComputerGrid::randomizePlacement()
{
	int x;
	int y;
	bool hardPlacement = false;
	char shipCounter = 64;
	Utils::Orientation shipOrientation;
	int shipsToAdd[5] = { getDestroyerNum(), getSubmarineNum(), getFrigateNum(),
	getBattleNum(), getCarrierNum() };

	if (getDifficulty() != Utils::EASY)
		hardPlacement = true;

	for (int i = 0; i < NUM_OF_SHIP_TYPES; i++)
	{
		for (int j = 0; j < shipsToAdd[i]; j++)
		{
			do {
				generatePosition(x, y, shipOrientation);
			} while (!checkPlacement(SHIP_TYPES[i], shipOrientation, x, y, hardPlacement));
			placeShip(SHIP_TYPES[i], shipOrientation, x, y, shipCounter);
		}
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

void ComputerGrid::fire(int x, int y, bool& playerHit)
{
	playerHit = false;
	int diffSize = getDifficultySize();
	Utils::clearScreen();
	cout << "Player's Turn\n";

	if (getGridCell(x, y) == 'X' || getGridCell(x, y) == 'H') 
	{
		playerHit = true;
	}

	if (getGridCell(x, y) == 'S')
	{
		playerHit = true;
		return;
	}

	if (getGridCell(x, y) == '~')
	{
		setGridCell(x, y, 'X');
		Utils::printCenter("MISS", diffSize);
	}

	else if (getGridCell(x, y) == 'X')
		Utils::printCenter("Missed again...", diffSize);

	else if (isShip(x, y))
	{
		playerHit = true;
		int damagedShip = (int)(getFleetPosCell(x, y) - 65);
		getFleetCell(damagedShip)->shipHit();
		if (getFleetCell(damagedShip)->shipSunk())
		{
			Utils::ShipName sunkShip = getShipLookupCell(damagedShip);
			damagedShip = (int)(getFleetPosCell(x, y));
			setGridCell(x, y, 'S');
			sinkShip(sunkShip, damagedShip);
			Utils::printCenter("Ship Sunk!", diffSize);
		}
		else
		{
			setGridCell(x, y, 'H');
			Utils::printCenter("HIT!", diffSize);
		}
		setLastHit(x, y);
	}

	else if (getGridCell(x, y) == 'H' || getGridCell(x, y) == '#')
		Utils::printCenter("Already hit...", diffSize);

	else if (getGridCell(x, y) == 'S')
		Utils::printCenter("That ship's already sunk!", diffSize);

	else if (getGridCell(x, y) == '*')
	{
		barrelDetonation(x, y, playerHit);
		Utils::printCenter("kaBOOOM!!", diffSize);
	}

	printGrid();
	if (!playerHit)
		system("pause");
}

void ComputerGrid::updateTurnGrid(ComputerGrid& compGrid)
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

void ComputerGrid::computerFire(int& x, int& y, bool& computerHit)
{
	static int randomFire = 5 + getDifficulty();
	int previousHit = getLastHitSize();
	int diffSize = getDifficultySize();
	int squaresKnown = enumerateTurnGrid();
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> probability{ 1, 100 };
	uniform_int_distribution<int> coodinate{ 0, diffSize - 1 };
	
	if (!hitYet)
		hitYet = computerHit;

	//fire randomly first 3 turns
	if (squaresKnown < randomFire && !hitYet)
	{
		do {
			x = coodinate(mt);
			y = coodinate(mt);
		} while (parityGrid[y][x] != 1 || turnGrid[y][x] != '~');
	}
	else
	{
		updateProbs();

		//get most probable spot to fire upon
		int maxProb = 0;
		int numOfMaxes = 0;
		vector<pair<int, int>> maxes;
		for (int y = 0; y < diffSize; y++)
			for (int x = 0; x < diffSize; x++)
			{
				if (probGrid[y][x] > maxProb)
				{
					numOfMaxes = 0;
					maxes.clear();
					maxProb = probGrid[y][x];
				}
				if (probGrid[y][x] == maxProb)
				{
					maxes.push_back(make_pair(x, y));
					numOfMaxes++;
				}
			}

		//if more than one square with max probability, randomlly pick between them
		if (numOfMaxes > 1)
		{
			int probChoice;
			uniform_int_distribution<int> randProb{ 0, (int)maxes.size() - 1 };
			probChoice = randProb(mt);
			x = get<0>(maxes[probChoice]);
			y = get<1>(maxes[probChoice]);
		}
		else
		{
			x = get<0>(maxes[0]);
			y = get<1>(maxes[0]);
		}
	}
}

void ComputerGrid::updateProbs()
{
	bool canFit;
	int hits;
	int probWeight;
	int shipTypes = NUM_OF_SHIP_TYPES;
	int oppPoolLeft = POOL_SIZE[getDifficulty()] - successfulHits;
	fillGrid(probGrid, 0);

	//don't compute probability of ships we know can't exist
	if (oppPoolLeft < 5)
	{
		if (oppPoolLeft > 2)
			shipTypes = oppPoolLeft;
		else
			shipTypes = 1;
	}

	//loop over ship types
	for (int i = 0; i < shipTypes; i++)
	{
		//try placing ships horizontally
		for (int y = 0; y < getDifficultySize(); y++)
		{
			for (int x = 0; x < getDifficultySize() - SHIP_SIZES[i] + 1; x++)
			{
				canFit = true;
				hits = 0;
				probWeight = 1;
				for (int j = x; j < SHIP_SIZES[i] + x; j++)
				{
					if (turnGrid[y][j] != '~' && turnGrid[y][j] != 'H')
					{
						canFit = false;
						break;
					}
					else if (turnGrid[y][j] == 'H')
						hits++;
				}
				if (hits > 0)
				{
					if (hits == 1)
						probWeight = 20;
					else
						probWeight = 200 * hits;
				}
				//increment probability matrix if ship fits
				if (canFit)
				{
					for (int j = x; j < SHIP_SIZES[i] + x; j++)
					{
						if (hits > 0)
						{
							if (turnGrid[y][j] == 'H')
								probGrid[y][j] = 0;
							else
								probGrid[y][j] += probWeight;
						}
						else
							probGrid[y][j] += probWeight;
					}
				}
				/*if (hits > 0)
				{
					Utils::clearScreen();
					printProbGrid();
					system("pause");
				}*/
			}
		}

		//try placing ships vertically
		for (int x = 0; x < getDifficultySize(); x++)
		{
			for (int y = 0; y < getDifficultySize() - SHIP_SIZES[i] + 1; y++)
			{
				canFit = true;
				hits = 0;
				probWeight = 1;
				for (int j = y; j < SHIP_SIZES[i] + y; j++)
				{
					if (turnGrid[j][x] != '~' && turnGrid[j][x] != 'H')
					{
						canFit = false;
						break;
					}
					else if (turnGrid[j][x] == 'H')
						hits++;
				}
				if (hits > 0)
				{
					if (hits == 1)
						probWeight = 20;
					else
						probWeight = 200 * hits;
				}
				//increment probability matrix if ship fits
				if (canFit)
				{
					for (int j = y; j < SHIP_SIZES[i] + y; j++)
					{
						if (hits > 0)
						{
							if (turnGrid[j][x] == 'H')
								probGrid[j][x] = 0;
							else
								probGrid[j][x] += probWeight;
						}
						else
							probGrid[j][x] += probWeight;
					}
				}
				/*if (hits > 0)
				{
					Utils::clearScreen();
					printProbGrid();
					system("pause");
				}*/
			}
		}
	}
}

void ComputerGrid::printProbGrid() const
{
	int diffSize = getDifficultySize();

	cout << "\n\n";
	for (int y = diffSize - 1; y >= 0; y--)
	{
		cout << "   ";
		for (int x = 0; x < diffSize; x++)
		{
			if (probGrid[y][x] < 10)
				cout << probGrid[y][x] << "     ";
			else if (probGrid[y][x] < 100)
				cout << probGrid[y][x] << "    ";
			else if (probGrid[y][x] < 1000)
				cout << probGrid[y][x] << "   ";
			else
				cout << probGrid[y][x] << "  ";
		}
		cout << endl;
	}
}
