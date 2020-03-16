#include <iostream>
#include "GameGrid.h"
#include "PlayerGrid.h"
#include "ComputerGrid.h"
#include <ctime>
using namespace std;

int test_barrel_detonate(int, int, int);
void test_barrel();
void test_fire();



int main() {

  srand(time(0));

  //test_barrel();
  test_fire();
 
}



void test_barrel() {

  int s_total = 0;
  int m_total = 0;
  int l_total = 0;
  bool temp = true;

  for (int i=0; i<20; i++) {
    int rand_x = (rand() % 8);
    int rand_y = (rand() % 8);
    s_total += test_barrel_detonate(rand_x, rand_y, 1);
  }


  for (int i=0; i<20; i++) {
    int rand_x = (rand() % 10);
    int rand_y = (rand() % 10);
    m_total += test_barrel_detonate(rand_x, rand_y, 2);
  }


  for (int i=0; i<20; i++) {
    int rand_x = (rand() % 12);
    int rand_y = (rand() % 12);
    l_total += test_barrel_detonate(rand_x, rand_y, 3);
  }


  if (s_total == 0) {
    cout << endl << "All small boards passed the barrel detonation test" << endl;
  }
  else {
    cout << endl << s_total << " small board(s) failed the barrel detonation test" << endl;
  }


  if (m_total == 0) {
    cout << endl << "All medium boards passed the barrel detonation test" << endl;
  }
  else {
    cout << endl << m_total << " medium board(s) failed the barrel detonation test" << endl;
  }


  if (l_total == 0) {
    cout << endl << "All large boards passed the barrel detonation test" << endl;
  }
  else {
    cout << endl << l_total << " large board(s) failed the barrel detonation test" << endl;
  }



  if (!test_barrel_detonate(0, 0, 3)) {
    cout << endl << "Unit has passed first edge case" << endl;
  }

  else {
    cout << endl << "Unit has failed first edge case" << endl;
  }

  if (!test_barrel_detonate(10, 10, 3)) {
    cout << endl << "Unit has passed second edge case" << endl;
  }

  else {
    cout << endl << "Unit has failed second edge case" << endl;
  }

  GameGrid grid(3, true);
  grid.barrelDetonation(5, 5, temp);
  cout << endl << "Example board:" << endl;
  grid.printGrid();

}



int test_barrel_detonate(int x, int y, int diff) {
  bool temp = true;
  GameGrid grid(diff, true);
  grid.barrelDetonation(x, y, temp);
  for (int i=-2; i<3; i++) {
    for (int j=-2; j<3; j++) {
      if ((i == 0) && (j == 0)) {
	if (grid.getGridCell(x,y) != '#') {
	cout << endl << "this function failed with this board" << endl;
	grid.printGrid();
	return 1;
	}
      }
      else if ((x+i < grid.getDifficultySize()) && (x+i >= 0) && (y+j < grid.getDifficultySize()) && (y+j >= 0)) {
        if (grid.getGridCell(x+i,y+j) != 'X') {
          cout << endl << "this function failed with this board" << endl;
          grid.printGrid();
          return 1;
        }
      }
    }
  }

  return 0;
}



void test_fire() {

  int diffChoice = 1;
  int x;
  int y;
  bool temp = true;

  ComputerGrid computerGrid(diffChoice, true);
  computerGrid.printGrid();
  cout << computerGrid.getFleetPosCell(4,4) << endl;

  for (int i=0; i<5; i++)  {
    cout << "sink a ship\nx-coordinate: ";
    cin >> x;
    cout << "\nycoordinate: ";
    cin >> y;
    computerGrid.fire(x-1, y-1, temp);
    cout << "Additional turn (0 is false, 1 is true): " << temp << endl;
  }

}


