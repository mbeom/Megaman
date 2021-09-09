#include "stdafx.h"
#include "mush.h"

mush::mush()
{
	IMAGEMANAGER->addFrameImage("mush_idle", "mush_idle.bmp", 75, 150, 1, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("mush_run", "mush_run.bmp", 600, 214, 8, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("mush_attack", "mush_attack.bmp", 375, 214, 5, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("mush_hide", "mush_hide.bmp", 450, 150, 6, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("mush_hurt", "mush_hurt.bmp", 386, 214, 4, 2, true, RGB(255, 0, 255), false);

	_type = GROUND;
}

HRESULT mush::init(const char* imageName, POINT position)
{
	_currentFrameX = _currentFrameY = 0;
	_count = _fireCount = 0;
	_current_hp = _max_hp = 100;

	_Fire_interval = 30;
	enemy_action = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(position.x, position.y,
		76, 76);
	origin_x = position.x;
	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	dist_count = WINSIZEX;
	angle = 0;
	_direction = 0;
	_gravity = 0;
	_hitCount = 0;
	_current_stage = (_rc.left + _width) / WINSIZEX;
	_state = enemy_state::LEFT_IDLE;

	dist = 0;
	is_ground = false;
	is_Fground = false;
	is_find = false;
	is_hit = false;
	is_boss = false;
	enemy_doing = false;
	boss_second_action = false;

	return S_OK;
}

void mush::update()
{
	_count++;
	if (_count % 10 == 0)
	{
		if (_state == enemy_state::HIDE)
		{
			action_interval[(int)enemy_state::HIDE]++;

			if (action_interval[(int)enemy_state::HIDE] - 10 > enemy_action->getMaxFrameX())
			{
				enemy_doing = false;
				is_alert = false;
				action_interval[(int)enemy_state::HIDE] = 0;
			}

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX++;

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					_currentFrameX = enemy_action->getMaxFrameX();
				}
			}

			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX--;

				if (_currentFrameX <= 0)
				{
					_currentFrameX = 0;
				}
			}

		}
		else if (_state == enemy_state::ATTACK)
		{
			action_interval[(int)enemy_state::ATTACK]++;

			if (action_interval[(int)enemy_state::ATTACK] - 10 > enemy_action->getMaxFrameX())
			{
				enemy_doing = false;
				action_interval[(int)enemy_state::ATTACK] = 0;
			}

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX++;

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					_currentFrameX = 0;
				}
			}

			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX--;

				if (_currentFrameX <= 0)
				{
					_currentFrameX = enemy_action->getMaxFrameX();
				}
			}
		}
		else if (_state == enemy_state::HURT)
		{
			action_interval[(int)enemy_state::HURT]++;

			if (action_interval[(int)enemy_state::HURT] - 1 > enemy_action->getMaxFrameX())
			{
				enemy_doing = false;
				is_hit = false;
				is_alert = false;
				boss_second_action = false;
				action_interval[(int)enemy_state::HURT] = 0;
			}

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX++;

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					_currentFrameX = enemy_action->getMaxFrameX();
				}
			}

			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX--;

				if (_currentFrameX <= 0)
				{
					_currentFrameX = 0;
				}
			}
		}
		else
		{
			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX++;
				if (_currentFrameX >= enemy_action->getMaxFrameX()) _currentFrameX = 0;
			}

			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);
				_currentFrameX--;
				if (_currentFrameX <= 0) _currentFrameX = enemy_action->getMaxFrameX();
			}
		}

		_count = 0;
	}
	// 현재 위치 갱신
	_current_stage = (_rc.left + _width) / WINSIZEX;

	move();
	enemy_state();
}

void mush::move()
{
	if (is_ground && _state == enemy_state::LEFT_IDLE)
	{
		_direction = 1;
		_currentFrameY = 1;
		_state = enemy_state::RIGHT_RUN;

		enemy_action = IMAGEMANAGER->findImage("mush_run");
		enemy_action->setFrameY(_direction);
	}
	if (_state == enemy_state::LEFT_RUN && !is_find && !enemy_doing)
	{
		enemy_doing = true;
		_rc.left -= 3;
		_rc.right -= 3;

		dist += 5;
		if (dist > 300 || origin_x - _rc.right > 300)
		{
			_direction = 1;
			_state = enemy_state::RIGHT_RUN;

			enemy_action = IMAGEMANAGER->findImage("mush_run");
			enemy_action->setFrameY(_direction);

			_currentFrameX = enemy_action->getMaxFrameX();
			_currentFrameY = 1;
			dist = 0;
			enemy_doing = false;
		}
	}

	if (_state == enemy_state::RIGHT_RUN && !is_find && !enemy_doing)
	{
		enemy_doing = true;
		_rc.left += 3;
		_rc.right += 3;

		dist += 5;
		if (dist > 300 || _rc.left - origin_x > 300)
		{
			_direction = 0;
			_state = enemy_state::LEFT_RUN;

			enemy_action = IMAGEMANAGER->findImage("mush_run");
			enemy_action->setFrameY(_direction);

			_currentFrameX = 0;
			_currentFrameY = 0;
			dist = 0;
			enemy_doing = false;
		}
	}

	if (is_find && !is_hit && !enemy_doing)
	{
		_pattern = RND->getFromIntTo(0, 2);
		enemy_doing = true;

		switch (_pattern)
		{
		case 0:
			_state = enemy_state::ATTACK;
			enemy_action = IMAGEMANAGER->findImage("mush_attack");
			enemy_action->setFrameY(_direction);
			_currentFrameY = _direction;

			if (_direction == 0)
			{
				_currentFrameX = 0;
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
			}

			break;
		case 1:
			_state = enemy_state::HIDE;
			enemy_action = IMAGEMANAGER->findImage("mush_hide");
			enemy_action->setFrameY(_direction);
			_currentFrameY = _direction;

			if (_direction == 0)
			{
				_currentFrameX = 0;
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
			}

			break;

		default:
			break;
		}
	}

	if(!is_find && !is_hit && !enemy_doing && !is_alert)
	{
		_pattern = RND->getFromIntTo(0, 2);

		switch (_pattern)
		{
		case 0:
			_direction = 0;
			_state = enemy_state::LEFT_RUN;
			enemy_action = IMAGEMANAGER->findImage("mush_run");
			enemy_action->setFrameY(_direction);
			_currentFrameY = 0;

			if (_direction == 0)
			{
				_currentFrameX = 0;
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
			}

			break;
		case 1:
			_direction = 1;
			_state = enemy_state::RIGHT_RUN;
			enemy_action = IMAGEMANAGER->findImage("mush_run");
			enemy_action->setFrameY(_direction);
			_currentFrameY = 1;

			if (_direction == 0)
			{
				_currentFrameX = 0;
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
			}

			break;

		default:
			_direction = 0;
			_state = enemy_state::LEFT_RUN;
			enemy_action = IMAGEMANAGER->findImage("mush_run");
			enemy_action->setFrameY(_direction);
			_currentFrameY = 0;

			if (_direction == 0)
			{
				_currentFrameX = 0;
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
			}

			break;
		}
	}
	
	if (!is_find && is_alert && !is_hit && enemy_doing)
	{
		enemy_doing = false;

		_state = enemy_state::HIDE;
		enemy_action = IMAGEMANAGER->findImage("mush_hide");
		enemy_action->setFrameY(_direction);
		_currentFrameY = _direction;

		if (_direction == 0)
		{
			_currentFrameX = 0;
		}
		else
			_currentFrameX = enemy_action->getMaxFrameX();
	}
	
	if ((!is_find && _state == enemy_state::ATTACK))
	{
		enemy_doing = false;
	}

	if ((!is_find && _state == enemy_state::HIDE))
	{
		enemy_doing = false;
	}

	if (_state == enemy_state::HURT)
	{
		if (_direction == 0)
		{
			_rc.left += 1;
			_rc.right += 1;
		}
		else
		{
			_rc.left -= 1;
			_rc.right -= 1;
		}
	}
}

bool mush::bullet_Fire()
{
	_fireCount++;

	if (_fireCount % _Fire_interval == 0)
	{
		_fireCount = 0;

		return true;
	}

	return false;
}

void mush::enemy_isHit()
{
	is_hit = true;
	enemy_doing = true;

	if (_state == enemy_state::HIDE)
	{
		boss_second_action = true;
		enemy_action = IMAGEMANAGER->findImage("mush_hide");

		if (_direction == 0)
		{
			enemy_action->setFrameY(_direction);
			_currentFrameX = enemy_action->getMaxFrameX();
			_currentFrameY = _direction;
		}
		else
		{
			enemy_action->setFrameY(_direction);
			_currentFrameX = 0;
			_currentFrameY = _direction;
		}
	}
	else
	{
		enemy_action = IMAGEMANAGER->findImage("mush_hurt");
		if (_direction == 0)
		{
			enemy_action->setFrameY(_direction);
			_currentFrameX = 0;
			_currentFrameY = _direction;
		}
		else
		{
			enemy_action->setFrameY(_direction);
			_currentFrameX = enemy_action->getMaxFrameX();
			_currentFrameY = _direction;
		}
	}

	_state = enemy_state::HURT;
}


