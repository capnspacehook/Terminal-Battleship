#include "Ship.h"
#pragma once 

class Destroyer : public Ship
{
public:
	Destroyer() : Ship()
	{
		setHealth(2);
	}
};