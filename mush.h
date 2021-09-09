#pragma once
#include "enemy.h"

class mush : public enemy
{
private:
	float dist;
	float origin_x;
	int _pattern;

public:
	mush();
	~mush() {};

	virtual HRESULT init(const char* imageName, POINT position);
	virtual void update();
	virtual void move();
	virtual bool bullet_Fire();
	virtual void enemy_isHit();
};

