#pragma once
#include "gameNode.h"

enum class enemy_state
{
	LEFT_IDLE,
	RIGHT_IDLE,
	IDLE,
	LEFT_RUN,
	RIGHT_RUN,
	ATTACK,
	DEFENSE,
	HIDE,
	HURT,
	DEAD,
	START,
	THROW,
	GET,
	ULT,

	enum_max
};

enum enemy_type
{
	FLYING,
	GROUND
};

class enemy : public gameNode
{
protected:
	image* enemy_action;		//에너미 이미지
	enemy_type _type;
	enemy_state _state;
	Player* _player;
	RECT _rc, _boss_hitbox;				//렉트

	int _currentFrameX;		//프레임번호로 사용할 변수
	int _rightIndex;
	int _currentFrameY;

	int _count;				//프레임렌더링 용
	int _fireCount;			//발사 카운트 용
	int _hitCount;			//데미지 받는 인터벌용 카운트
	int _Fire_interval;			//발사 주기
	int _direction;
	int _width, _height;
	int _current_stage;
	int action_interval[static_cast<int>(enemy_state::enum_max)];

	float dist_count;		// 일정거리 이동하기위한 카운트
	float angle;
	float _gravity;
	float _friction;
	float _current_hp, _max_hp;

	bool is_ground;
	bool is_Fground;
	bool is_find;
	bool is_alert;
	bool enemy_doing;
	bool is_hit;
	bool is_boss;
	bool boss_wait;
	bool boss_second_action;
	bool end_game;

public:
	enemy() {};
	~enemy() {};

	virtual HRESULT init();
	virtual HRESULT init(const char* imageName, POINT position);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void move();
	virtual bool bullet_Fire();

	void draw();

	void set_top(int len);
	void set_direction(int dir) { _direction = dir; }
	void setOff_doing() { enemy_doing = false; }
	void setOff_Fground() { is_Fground = false; }
	void setOn_Fground() { is_Fground = true; }
	void set_damage(float dmg) { _current_hp -= dmg; }

	void find_player() { is_find = true; }
	void Nfind_player() { is_find = false; }

	void alert_player() { is_alert = true; }
	void Nalert_player() { is_alert = false; }

	void enemy_checkGround(int len);
	void enemy_checkWall(int len, int _direction);
	void enemy_getGravity();
	void enemy_state();
	virtual void enemy_isHit();

	int get_stage() { return _current_stage; }

	inline RECT getRect() { return _rc; }
	inline RECT get_hitbox() { return _boss_hitbox; }
	inline enemy_type get_type() { return _type; }
	inline auto get_state() { return _state; }
	inline bool on_ground() { return is_ground; }
	inline bool on_Fground() { return is_Fground; }
	inline int get_direction() { return _direction; }
	inline int get_height() { return _height; }
	inline float get_hp() { return _current_hp; }
	inline bool get_ishit() { return is_hit; }
	inline bool get_isalert() { return is_alert; }
	inline bool check_boss() { return is_boss; }
	inline bool waiting_boss() { return boss_wait; }
	void Nwaiting_boss() { boss_wait = false; }
	void do_action() { boss_second_action = true; }
	bool Ndo_action() { return boss_second_action; }
	bool get_endsignal() { return end_game; }
};

