#include <vector>
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "Utils.h"
#pragma once

class ComputerGrid : public GameGrid
{
public:
	ComputerGrid(int sDifficulty, bool displGrid)
		: GameGrid(sDifficulty, displGrid)
	{
		chooseRandomShips();
		randomizePlacement();
		placeBarrels();
		probGrid.resize(getDifficultySize(), std::vector<int>(getDifficultySize()));
		turnGrid.resize(getDifficultySize(), std::vector<char>(getDifficultySize()));
		parityGrid.resize(getDifficultySize(), std::vector<int>(getDifficultySize()));
		fillGrid(probGrid, 0);
		fillGrid(turnGrid, '~');
		fillGrid(parityGrid, 0);
		buildParityGrid();
	}
	void buildParityGrid();
	void chooseRandomShips();
	void randomizePlacement();
	void generatePosition(int&, int&, Utils::Orientation&);
	void randomOrientation(Utils::Orientation&);
	void fire(int, int, bool&);
	void computerFire(int&, int&, bool&);
	void updateTurnGrid(ComputerGrid&);
	int enumerateTurnGrid()
	{
		//return how many squares of opponent's grid we know the contents of
		int counter = 0;
		successfulHits = 0;
		for (int y = 0; y < getDifficultySize(); y++)
			for (int x = 0; x < getDifficultySize(); x++)
			{
				if (turnGrid[y][x] != '~')
					counter++;

				if (turnGrid[y][x] == 'S')
					successfulHits++;
			}
		return counter;
	}
	void updateProbs();
	void printProbGrid() const;
private:
	std::vector<std::vector<int>> probGrid;
	std::vector<std::vector<char>> turnGrid;
	std::vector<std::vector<int>> parityGrid;
	//true once computer hits player
	bool hitYet = false;
	int successfulHits = 0;

	friend void PlayerGrid::updateTurnGrid(ComputerGrid&);
	// friend void ComputerGrid::updateTurnGrid(ComputerGrid&);
};
