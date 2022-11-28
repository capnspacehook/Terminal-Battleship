#include <string>
#include <iostream>
#include <cstdlib>
#pragma once

namespace Utils
{
	inline void clearScreen()
	{
#ifdef _WIN32
		system("cls");
#else
		// Assume POSIX
		system("clear");
#endif
	}
	inline void printCenter(std::string s, int i)
	{
		std::string indent((i * 2 - s.length()) / 2, ' ');
		std::cout << indent << s;
	}
	inline void pause()
	{
		do {
    		std::cout << "Press the Enter key to continue." << '\n';
   		} while (std::cin.get() != '\n');
	}
	enum Difficulty { EASY, MEDIUM, HARD };
	enum ShipName { DESTROYER, SUBMARINE, FRIGATE, BATTLESHIP, CARRIER };
	enum Orientation { HORIZONTAL, VERTICAL };
};