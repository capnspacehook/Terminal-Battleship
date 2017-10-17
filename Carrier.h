#include "Ship.h"
#pragma once

class Carrier : public Ship
{
public:
	Carrier() : Ship()
	{
		setHealth(5);
	}
};
