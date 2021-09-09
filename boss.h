#pragma once
#include "enemy.h"
#include "HP_Bar.h"

class boss : public enemy
{
private:
	int boss_pattern;
	int hitbox_width, hitbox_height;
	float distance;
	bool stop;

public:
	boss();
	~boss() {};

	virtual HRESULT init(const char* imageName, POINT position);
	virtual void update();
	virtual void move();
	virtual void enemy_isHit();
	virtual void get_pattern();
	virtual void draw();
	void make_idle();
};

