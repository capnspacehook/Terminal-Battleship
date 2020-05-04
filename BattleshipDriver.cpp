#include <iostream>
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "ComputerGrid.h"

using namespace std;

int main()
{
	int diffChoice = 0;
	int shotX;
	int shotY;
	bool turnResult = false;
	bool playerWon = false;
	bool computerWon = false;

	while (diffChoice < 1 || diffChoice > 3)
	{
		Utils::clearScreen();
		cout << "Welcome to Battleship!" << endl
		<< "Please choose your difficulty:" << endl
		<< "(1) Easy" << endl
		<< "(2) Normal" << endl
		<< "(3) Hard" << endl;
		if (diffChoice != 0) {
			cout << "That is not a valid difficulty setting." << endl
			<< "Please enter a valid difficulty setting: " << endl;
		}
		std::cout << "Difficulty: ";
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
			system("pause");
			computerGrid.computerFire(shotX, shotY, turnResult);
			playerGrid.fire(shotX, shotY, turnResult);
			playerGrid.updateTurnGrid(computerGrid);
			//computerGrid.printProbGrid();
			system("pause");
		} while (turnResult && !playerGrid.checkWin(computerWon));
	}

	Utils::clearScreen();

	if (playerWon)
		cout << "You Win!\n\n";
	else
		cout << "You Lose...\n\n";

	cout << "Thanks for playing!\nCreated by Andrew LeFevre\n";
	system("pause");
}