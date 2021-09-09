#pragma once
#include "gameNode.h"
#include "Object.h"
#include "bullets.h"
#include "HP_Bar.h"

class Object;
class enemyManager;

enum MYSTATE
{
	RIGHT_IDLE,
	LEFT_IDLE,
	RIGHT_RUN,
	LEFT_RUN,
	ATTACK,
	ATTACK2,
	ATTACK3,
	ATTACK4,
	DASH,
	JUMP,
	WALL,
	JUMP_DOWN,
	LADDER,
	WALL_ATTACK,
	SHOOT,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	START,
	JUMP_ATTACK,
	HURT,
	DEAD,

	enum_Max
};
struct Player_INFO
{
	image* action;
	image* effect;
	RECT body, hit_box;
	int width;
	int height;
	int direction;

	bool on_ground;
	bool ladder;
	bool on_flying_wall;
	bool doing;
	bool is_charged;
	bool is_hit;
};

class Player : public gameNode
{
private:
	Player_INFO _player;
	MYSTATE _state;
	hyunMoo* _bullet;
	hyunMoo* _Special_bullet;
	HP_bar* _HP;
	image* _shadow_img[4];
	POINT _shadow_pos[4];
	Obj* _obj;
	enemyManager* _em;

	int _count, _index, _left_index;
	int _count_effect, temp_interval;
	int _edge_left, _edge_right, _edge_bottom, _edge_top;
	int _Ledge_left, _Ledge_right, _Ledge_bottom, _Ledge_top;
	int each_interval[enum_Max];
	int stage;
	int _bullet_dist;

	float effect_x, effect_y;
	float hit_width, hit_height, _current_hp, _maxHP;

	float _gravity;
	float _jump_power;
	float _dash_speed;
	float _wall_friction;

	bool _shadow_action;
	bool _second_action;
	bool effect_set;
	bool end;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void render();
	virtual void update();

	void key_setting();
	void key_pressed();
	void move_img();
	void get_damage(float dmg);
	void define_status();
	void based_on_status();
	void land();

	void Link_ObjectClass(Obj* obj) { _obj = obj; }
	void Link_EnemyClass(enemyManager* em) { _em = em; }
	void make_rect(LONG x, LONG y);
	void set_onGround() { _player.on_ground = true; }
	void setoff_onGround() { _player.on_ground = false; }
	void set_FonGround() { _player.on_flying_wall = true; }
	void setoff_FonGround() { _player.on_flying_wall = false; }
	void set_top(int len) { _edge_top = len; 
	_player.body.bottom = len;
	_player.body.top = _player.body.bottom - _player.height;}
	void check_ground(LONG top);
	void check_wall(LONG len, int direction);
	void check_Fground(LONG top);
	void collision_enemy();
	void get_gravity();
	void set_isHit();
	void run_effect();
	int get_direction() { return _player.direction; }

	Player_INFO get_playerInfo() { return _player; };
	HP_bar* get_hpPointer() { return _HP; }
	MYSTATE get_state() { return _state; }
	hyunMoo* get_bullet() { return _bullet; }
	bool on_ground() { return _player.on_ground; }
	bool Fon_ground() { return _player.on_flying_wall; }
	bool is_hit() { return _player.is_hit; }
	bool end_player() { return end; }
	void Nend_player() { end = false; }
};

