#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
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
		std::cout << "Press ENTER to continue... " << std::flush;
		std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
		std::cin.get();
	}
	enum Difficulty { EASY, MEDIUM, HARD };
	enum ShipName { DESTROYER, SUBMARINE, FRIGATE, BATTLESHIP, CARRIER };
	enum Orientation { HORIZONTAL, VERTICAL };
};
