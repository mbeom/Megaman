#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init()
{
	return S_OK;
}

HRESULT enemy::init(const char* imageName, POINT position)
{
	_currentFrameX = _currentFrameY = 0;
	_count = _fireCount = 0;
	_current_hp = _max_hp = 50;

	_Fire_interval = RND->getFromIntTo(1, 500);

	enemy_action = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(position.x, position.y,
		enemy_action->getFrameWidth(), enemy_action->getFrameHeight());

	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	dist_count = WINSIZEX;
	angle = 0;
	_direction = 0;
	_gravity = 0;
	_hitCount = 0;
	_state = enemy_state::LEFT_IDLE;

	is_ground = false;
	is_Fground = false;
	is_find = false;
	is_hit = false;
	is_boss = false;
	enemy_doing = false;

	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	_count++;
	if (_count % 4 == 0)
	{
		if (_currentFrameX >= enemy_action->getMaxFrameX()) _currentFrameX = 0;
		enemy_action->setFrameX(_currentFrameX);
		_currentFrameX++;

		_count = 0;
	}

	if (is_hit)
	{
		_hitCount++;
	}

	if (_hitCount > 50)
	{
		is_hit = false;
		_hitCount = 0;
	}

	move();
}

void enemy::render()
{
	draw();
	/*
	char temp[128];
	sprintf_s(temp, " 플레이어 위치: %d, %d, %d ", end_game, _state, (int)_current_hp);
	TextOut(getMemDC(), _rc.left, _rc.top - 200, temp, strlen(temp));
	*/
}

void enemy::move()
{

}

void enemy::draw()
{
	int x = _rc.left + _width / 2 - enemy_action->getFrameWidth() / 2;
	int y = _rc.top + _height / 2 - enemy_action->getFrameHeight() / 2;

	if(!is_boss)
		enemy_action->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	
	else
		enemy_action->frameRender(getMemDC(), x, y, _currentFrameX, _currentFrameY);

	//Rectangle(getMemDC(), _boss_hitbox);
}

void enemy::set_top(int len)
{
	_rc.bottom = len;
	_rc.top = _rc.bottom - _height;
}

void enemy::enemy_checkGround(int len)
{
	// 보스는 START 모션이 있기때문에 상태변경을 마음대로 안함.
	if (!is_boss)
	{	// 일반 잡몹만 땅에 착지시 IDLE 상태로
		if (_direction == 0)
		{
			_state = enemy_state::LEFT_IDLE;
		}
		else
			_state = enemy_state::RIGHT_IDLE;
	}

	is_ground = true;

	_rc.bottom = len;
	_rc.top = _rc.bottom - _height;
}

void enemy::enemy_checkWall(int len, int _direction)
{
	if (_direction == 1)
	{
		_rc.right = len;
		_rc.left = _rc.right - _width;
	}

	// 측면에서 충돌시 (오른쪽)
	else
	{
		_rc.left = len;
		_rc.right = _rc.left + _width;
	}
}

void enemy::enemy_getGravity()
{
	is_ground = false;
}

void enemy::enemy_state()
{
	if (_type == GROUND)
	{
		if (!is_ground)
		{
			_gravity += 0.2f;
			_rc.bottom += _gravity;
			_rc.top += _gravity;
		}
		else
			_gravity = 0;
	}

}

void enemy::enemy_isHit()
{
	is_hit = true;
}

//에너미가 발사했다고 신호만 주는 함수
bool enemy::bullet_Fire()
{
	_fireCount++;

	if (_fireCount % _Fire_interval == 0)
	{

		_Fire_interval = RND->getFromIntTo(1, 500);
		_fireCount = 0;

		return true;
	}

	return false;
}
