#pragma once
#include "enemy.h"

class Bats : public enemy
{
public:
	Bats();
	~Bats() {};

	virtual void move();
};

