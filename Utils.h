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
	inline void printCenter(std::string s)
	{
		std::string indent( (50 - s.length()) / 2, ' ' );
		std::cout << indent << s;
	}
	enum Difficulty{ EASY, MEDIUM, HARD };
	enum ShipName{ DESTROYER, FRIGATE, BATTLESHIP };
	enum Orientation{ HORIZONTAL, VERTICAL };
};