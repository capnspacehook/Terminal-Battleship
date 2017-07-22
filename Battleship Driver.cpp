#include <iostream>
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "ComputerGrid.h"
//#include "Ship.h"
//#include "Destroyer.h"
//#include "Frigate.h"
//#include "Battleship.h"
using namespace std;

//controls how many ships can be added
const int EASY_POOL = 30;
const int MEDIUM_POOL = 45;
const int HARD_POOL = 60;
const int DESTROYER_SIZE = 3;
const int FRIGATE_SIZE = 6;
const int BATTLESHIP_SIZE = 9;
//checks if ship user is trying to add can be added 
void shipCheck(bool&, int);
int main()
{
	bool poolEmpty = false;
	int diffChoice;
	int shipChoice;
	int shipPool;
	int destoyNum = 0;
	int frigateNum = 0;
	int battleNum = 0;
	int currentFleet = 0;
	bool playerWon = false;
	bool computerWon = false;

	cout << "Welcome to Battleship 2.0!" << endl
		<< "Please choose your difficulty:" << endl
		<< "(1) Easy\t 15x15 grid" << endl
		<< "(2) Medium\t 20x20 grid" << endl
		<< "(3) Hard\t 25x25 grid\n: ";
	cin >> diffChoice;
	while (cin.fail() || diffChoice < 1 || diffChoice > 3)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "That is not a valid difficulty setting." << endl
			<< "Please enter a valid difficulty setting: ";
		cin >> diffChoice;
	}

	if (diffChoice == 1)
		shipPool = EASY_POOL;
	else if (diffChoice == 2)
		shipPool = MEDIUM_POOL;
	else if (diffChoice == 3)
		shipPool = HARD_POOL;
	
	cout << "Choose your fleet:" << endl;
	while (!poolEmpty)
	{
		if (shipPool - currentFleet >= 3)
		{
			cout << "\n(1) Destroyers: 3 ship parts" << endl
				<< "(2) Frigates: 6 ship parts" << endl
				<< "(3) Battleships: 9 ship parts"
				<< "\nYou have " << shipPool - currentFleet << " ship parts left." << "\n:";
			cin >> shipChoice;

			while (cin.fail() || shipChoice < 1 || shipChoice > 3)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "That is not a valid option." << endl
					<< "Please choose a ship for your fleet: ";
				cin >> shipChoice;
			}

			if (shipChoice == 1)
			{
				destoyNum++;
				currentFleet += DESTROYER_SIZE;
			}
			else if (shipChoice == 2)
			{
				if (shipPool - (currentFleet + FRIGATE_SIZE) >= 0)
				{
					frigateNum++;
					currentFleet += FRIGATE_SIZE;
				}
				else
					shipCheck(poolEmpty, shipPool);
			}
			else if (shipChoice == 3)
				if (shipPool - (currentFleet + BATTLESHIP_SIZE) >= 0)
				{
					battleNum++;
					currentFleet += BATTLESHIP_SIZE;
				}
				else
					shipCheck(poolEmpty, shipPool);
		}
		else
			poolEmpty = true;
	}

	//PlayerGrid playerGrid(diffChoice, 1, 1, 1);
	PlayerGrid playerGrid(diffChoice, destoyNum, frigateNum, battleNum);
	ComputerGrid computerGrid(diffChoice, shipPool);

	while (!playerGrid.checkWin(computerWon) && !computerGrid.checkWin(playerWon))
	{
		Utils::clearScreen();
		cout << "Player's Turn\n";
		computerGrid.printGrid();
		computerGrid.fire();
		computerGrid.printGrid();
		system("pause");
		Utils::clearScreen();
		
		if (playerGrid.checkWin(computerWon) || computerGrid.checkWin(playerWon))
			break;

		cout << "Computer's Turn\n";
		playerGrid.printGrid();
		system("pause");
		playerGrid.fire();
		playerGrid.printGrid();
		system("pause");

		if (playerGrid.checkWin(computerWon) || computerGrid.checkWin(playerWon))
			break;
	}

	if (playerWon)
		cout << "You Win!\n\n";
	else
		cout << "You Lose...\n\n";
	system("pause");
}

void shipCheck(bool& empty, int pool)
{
	if (pool - DESTROYER_SIZE <= 0 && pool - FRIGATE_SIZE <= 0 && pool - BATTLESHIP_SIZE <= 0)
		empty = true;
	if (!empty)
		cout << "\nShip cannot be added. Please choose a smaller ship.\n";

}