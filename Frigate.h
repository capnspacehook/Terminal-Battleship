#include "Ship.h"
#pragma once 

class Frigate : public Ship
{
public:
	Frigate() : Ship()
	{
		setHealth(3);
	}
};