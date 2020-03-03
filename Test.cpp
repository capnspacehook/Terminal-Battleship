#include <iostream>
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "ComputerGrid.h"
using namespace std;

int main() {
  int diffChoice;
  bool turnResult;
  cout << "Welcome to the unit testing area!\n" << endl
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


    ComputerGrid computerGrid(diffChoice, true);
    computerGrid.printGrid();
    computerGrid.setGridCell(0,0,'*');
    computerGrid.fire(0,0,turnResult);


}
