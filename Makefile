CC = g++ -std=c++11
exe = RunBattleship.exe

$(exe): BattleshipDriver.o ComputerGrid.o GameGrid.o PlayerGrid.o
	$(CC): BattleshipDriver.o ComputerGrid.o GameGrid.o PlayerGrid.o -o $(exe)

BattleshipDriver.o: BattleshipDriver.cpp
	$(CC) -c BattleshipDriver.cpp
ComputerGrid.o: ComputerGrid.cpp
	$(CC) -c ComputerGrid.cpp
GameGrid.o: GameGrid.cpp
	$(CC) -c GameGrid.cpp
PlayerGrid.o: PlayerGrid.cpp
	$(CC) -c PlayerGrid.cpp

clean:
	rm -f *.out *.o $(exe)
