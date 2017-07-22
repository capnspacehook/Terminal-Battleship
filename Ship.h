#pragma once

class Ship
{
public:
	void setHealth(int h)
	{
		health = h;
	}
	int getHealth() const
	{
		return health;
	}
	//decreases hit ship's health by one
	void shipHit()
	{
		setHealth(this->getHealth() - 1);
	}
	//checks if ship is sunk
	bool shipSunk()
	{
		bool sunk = false;
		if (this->getHealth() == 0)
			sunk = true;
		return sunk;
	}
private:
	int health;
};