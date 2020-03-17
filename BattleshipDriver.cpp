#include <iostream>
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "ComputerGrid.h"
using namespace std;

int main()
{
	int diffChoice;
	int shotX;
	int shotY;
	bool turnResult = false;
	bool playerWon = false;
	bool computerWon = false;

	cout << "Welcome to Battleship 2.0!" << endl
		<< "Please choose your difficulty:" << endl
		<< "(1) Easy" << endl
		<< "(2) Medium" << endl
		<< "(3) Hard\n: ";
	cin >> diffChoice;
	while (cin.fail() || diffChoice < 1 || diffChoice > 3)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "That is not a valid difficulty setting." << endl
			<< "Please enter a valid difficulty setting: ";
		cin >> diffChoice;
	}

	PlayerGrid playerGrid(diffChoice, true);
	ComputerGrid computerGrid(diffChoice, false);

	while (!playerGrid.checkWin(computerWon) && !computerGrid.checkWin(playerWon))
	{
		do
		{
			Utils::clearScreen();
			cout << "Player's Turn\n";
			computerGrid.printGrid();
			playerGrid.playerFire(shotX, shotY);
			computerGrid.fire(shotX, shotY, turnResult);
			Utils::pause();
			Utils::clearScreen();
		} while (turnResult && !computerGrid.checkWin(playerWon));

		if (playerGrid.checkWin(computerWon) || computerGrid.checkWin(playerWon))
			break;

		do
		{
			Utils::clearScreen();
			cout << "Computer's Turn\n";
			playerGrid.printGrid();
			//computerGrid.updateProbs();
			//computerGrid.printProbGrid();
			//system("pause");
			Utils::pause();
			computerGrid.computerFire(shotX, shotY, turnResult);
			playerGrid.fire(shotX, shotY, turnResult);
			playerGrid.updateTurnGrid(computerGrid);
			//computerGrid.printProbGrid();
			//system("pause");
			Utils::pause();
		} while (turnResult && !playerGrid.checkWin(computerWon));
	}

	//Utils::clearScreen();

	if (playerWon)
		cout << "You Win!\n\n";
	else
		cout << "You Lose...\n\n";

	cout << "Thanks for playing!\nCreated by Andrew LeFevre\n";
	//system("pause");
	Utils::pause();
}