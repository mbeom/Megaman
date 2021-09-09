#pragma once
#include "gameNode.h"
#include "Player.h"
#include "Object.h"
#include "enemyManager.h"

struct Camera
{
	float x, y;
};
class playGround : public gameNode
{
private:
	Player* _player;
	Obj* _obj;
	enemyManager* _enemy;

	Camera _camera;
	image* _background;
	int _boss_num;
	int restart_count;
	bool _start, end;
	bool _intro;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

