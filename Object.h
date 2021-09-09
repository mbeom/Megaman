#pragma once
#include "gameNode.h"

#define stage_num 11

class Player;
class enemyManager;

struct Ground
{
	RECT rc;
	image* img;
	int height;
};

struct Wall
{
	RECT rc;
	image* img;
};

class Obj : public gameNode
{
private:

	Player* _player_info;	
	enemyManager* _enemy_info;
	image* stage[stage_num];
	image* _door;

	int x_left, x_right, y;
	int current_stage;
	bool on_ground, on_Fground;
	bool is_pass;
	bool show_map;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void render();
	virtual void update();

	void pixel_collision();
	void enemy_pixel_collision();
	void Link_PlayerInfo(Player* player) { _player_info = player; }
	void Link_EnemyInfo(enemyManager* em) { _enemy_info = em; }

	int get_currentStage() { return current_stage; }

};

