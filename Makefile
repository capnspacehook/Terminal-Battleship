CC = g++ -std=c++11
exe = RunBattleship

$(exe): BattleshipDriver.o Test.o ComputerGrid.o GameGrid.o PlayerGrid.o
	$(CC) -o RunBattleship BattleshipDriver.o ComputerGrid.o GameGrid.o PlayerGrid.o
	$(CC) -o UnitTesting Test.o ComputerGrid.o GameGrid.o PlayerGrid.o

BattleshipDriver.o: BattleshipDriver.cpp
	$(CC) -c BattleshipDriver.cpp

Test.o: Test.cpp
	$(CC) -c Test.cpp

ComputerGrid.o: ComputerGrid.cpp
	$(CC) -c ComputerGrid.cpp

GameGrid.o: GameGrid.cpp
	$(CC) -c GameGrid.cpp

PlayerGrid.o: PlayerGrid.cpp
	$(CC) -c PlayerGrid.cpp

clean:
	rm -f *.out *.o $(exe) $(exe_2)
