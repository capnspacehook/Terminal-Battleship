#include "Ship.h"
#pragma once

class Submarine : public Ship
{
public:
	Submarine() : Ship()
	{
		setHealth(3);
	}
};