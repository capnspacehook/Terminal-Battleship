#include "Ship.h"
#pragma once 

class Battleship : public Ship
{
public:
	Battleship() : Ship()
	{
		setHealth(9);
	}
};