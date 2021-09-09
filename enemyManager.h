#pragma once
#include "gameNode.h"

#include "enemy.h"
#include "Bats.h"
#include "mush.h"
#include "boss.h"
#include "HP_Bar.h"

#include "bullets.h"
#include <vector>

//��ȣ������ ���� Ŭ�� ���漱��
class Player;

class enemyManager : public gameNode
{
private:
	typedef vector<enemy*>				Enemy_pool;
	typedef vector<enemy*>::iterator	pool_iter;

private:
	Enemy_pool _enemy_pool;
	pool_iter _pool_iter;
	bullet* _bullet;
	hyunMoo* _boss_bumerang;
	hyunMoo* _boss_ult;
	HP_bar* boss_HPBar;
	image* enemy_effect;

	int effect_frameX;
	int _count;
	float effect_x, effect_y, effect_dir;
	bool is_run;
	//��� �� �����Ϳ� �Ұ��ϴ� ������
	Player* _player;

public:
	enemyManager() {};
	~enemyManager() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void set_bats(int pos_x, int pos_y);
	void set_mush(int pos_x, int pos_y);
	void set_boss(int pos_x, int pos_y);
	void minionBulletFire();
	void find_player();
	void collision_player();
	void remove_unit();
	void run_effect();

	//���� �������ε� �Լ��̸��� �׷����� ����
	void Link_PlayerClass(Player* player) { _player = player; }

	vector<enemy*> getVMinion() { return _enemy_pool; }
	vector<enemy*>::iterator getVIMinion() { return _pool_iter; }
	bullet* get_bullet() { return _bullet; }
	POINT PointMake(LONG x, LONG y);
};

