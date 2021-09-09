#include "stdafx.h"
#include "boss.h"

boss::boss()
{
	IMAGEMANAGER->addFrameImage("boss_idle", "boss_idle.bmp", 1612, 600, 3, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("boss_start", "boss_start.bmp", 6985, 600, 13, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("boss_blade", "boss_blade.bmp", 4836, 600, 9, 2, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("boss_dead", "boss_dead.bmp", 2687, 600, 5, 2, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("boss_defense", "boss_defense.bmp", 2149, 600, 4, 2, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("boss_get", "boss_get.bmp", 2687, 600, 5, 2, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("boss_throw", "boss_throw.bmp", 5910, 600, 11, 2, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("boss_ult", "boss_ult.bmp", 11821, 600, 22, 2, true, RGB(168, 230, 29), false);

	_type = GROUND;
}

HRESULT boss::init(const char* imageName, POINT position)
{

	_currentFrameX = _currentFrameY = 0;
	_count = _fireCount = 0;
	_current_hp = _max_hp = 1000;

	enemy_action = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(position.x, position.y,
		100, 130);

	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	 angle = 0;
	_direction = 0;
	_gravity = 0;
	_hitCount = 0;

	dist_count = WINSIZEX;
	_current_stage = (_rc.left + _width) / WINSIZEX;
	_state = enemy_state::START;

	is_ground = false;
	is_Fground = false;
	is_find = false;
	is_hit = false;
	is_boss = true;
	boss_wait = false;
	enemy_doing = true;
	boss_second_action = false;

	for (int i = 0; i < (int)enemy_state::enum_max; i++)
	{
		action_interval[i] = 0;
	}

	return S_OK;
}

void boss::update()
{
	_count++;
	if (_count % 6 == 0)
	{
		// ************* 스타트 ***************************
		if (_state == enemy_state::START)
		{
			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);

				if (is_ground)
				{
					_currentFrameX++;
				}

				if (_currentFrameX >= enemy_action->getMaxFrameX() - 1)
				{
					_currentFrameX = 0;
					enemy_doing = false;
				}
			}
		}
		// ************* 공격시 ***************************
		if (_state == enemy_state::ATTACK)
		{
			action_interval[static_cast<int>(enemy_state::ATTACK)] += 1;

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);

				if (action_interval[(int)(enemy_state::ATTACK)] % 3 == 0)
				{
					_currentFrameX++;

					hitbox_width += 40;
					if (hitbox_width > 350)
						hitbox_width = 350;

					hitbox_height += 5;
				}

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					_direction = 1;
					action_interval[(int)(enemy_state::ATTACK)] = 0;
					hitbox_width = hitbox_height = 0;
					make_idle();
				}
				_boss_hitbox = RectMake(_rc.right - hitbox_width, _rc.top - 10, hitbox_width, hitbox_height);
			}
			else
			{
				enemy_action->setFrameX(_currentFrameX);

				if (action_interval[static_cast<int>(enemy_state::ATTACK)] % 3 == 0)
				{
					_currentFrameX--;

					hitbox_width += 40;
					if (hitbox_width > 350)
						hitbox_width = 350;
					hitbox_height += 5;
				}

				if (_currentFrameX <= 0)
				{
					_direction = 0;
					hitbox_width = hitbox_height = 0;
					action_interval[(int)(enemy_state::ATTACK)] = 0;
					make_idle();
				}
				_boss_hitbox = RectMake(_rc.left - 10, _rc.top - 10, hitbox_width, hitbox_height);
			}
		}

		// ************* IDLE ***************************
		if(_state == enemy_state::IDLE)
		{
			action_interval[static_cast<int>(enemy_state::IDLE)] += 1;

			if (action_interval[(int)enemy_state::IDLE] > 25)
			{
				action_interval[(int)enemy_state::IDLE] = 0;
				enemy_doing = false;
			}

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);

				if (action_interval[(int)enemy_state::IDLE] % 10 == 0)
				{
					_currentFrameX++;
				}

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					_currentFrameX = 0;
				}
			}
			if(_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);

				if (action_interval[(int)enemy_state::IDLE] % 10 == 0)
				{
					_currentFrameX--;
				}

				if (_currentFrameX <= 0)
				{
					_currentFrameX = enemy_action->getMaxFrameX();
				}
			}
		}
		if (_state == enemy_state::DEFENSE)
		{
			action_interval[static_cast<int>(enemy_state::IDLE)] += 1;

			if (action_interval[(int)enemy_state::IDLE] > 20)
			{
				enemy_doing = true;
				action_interval[(int)enemy_state::IDLE] = 0;
				hitbox_width = hitbox_height = 0;
				_boss_hitbox = RectMake(_rc.left, _rc.top, hitbox_width, hitbox_height);
				make_idle();
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

		if (_state == enemy_state::THROW)
		{
			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);

				_currentFrameX++;

				if (_currentFrameX >= enemy_action->getMaxFrameX() && !boss_second_action)
				{
					boss_wait = true;
					_currentFrameX = enemy_action->getMaxFrameX();
					
				}

				if (_currentFrameX >= enemy_action->getMaxFrameX() && boss_second_action)
				{
					enemy_doing = true;
					boss_second_action = false;
					
					_state = enemy_state::GET;
					enemy_action = IMAGEMANAGER->findImage("boss_get");
					_currentFrameX = 0;
					_currentFrameY = _direction;


				}
			}
			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);

				_currentFrameX--;

				if (_currentFrameX <= 0 && !boss_second_action)
				{
					boss_wait = true;
					_currentFrameX = 0;
				}

				if (_currentFrameX <= 0 && boss_second_action)
				{
					enemy_doing = true;
					boss_second_action = false;

					_state = enemy_state::GET;
					enemy_action = IMAGEMANAGER->findImage("boss_get");
					_currentFrameX = enemy_action->getMaxFrameX();
					_currentFrameY = _direction;
				}
			}
		}

		if (_state == enemy_state::GET)
		{
			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);

				_currentFrameX++;

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					enemy_doing = true;
					make_idle();
				}
			}
			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);

				_currentFrameX--;

				if (_currentFrameX <= 0)
				{
					enemy_doing = true;
					make_idle();
				}
			}
		}

		if (_state == enemy_state::ULT)
		{
			action_interval[static_cast<int>(enemy_state::ULT)] += 1;

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);
				if (action_interval[static_cast<int>(enemy_state::ULT)] % 1 == 0)
				{
					_currentFrameX++;
				}

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					enemy_doing = true;
					boss_second_action = true;
					action_interval[static_cast<int>(enemy_state::ULT)] = 0;
					make_idle();
				}
			}
			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);

				if (action_interval[static_cast<int>(enemy_state::ULT)] % 1 == 0)
				{
					_currentFrameX--;
				}

				if (_currentFrameX <= 0)
				{
					enemy_doing = true;
					boss_second_action = true;
					action_interval[static_cast<int>(enemy_state::ULT)] = 0;
					make_idle();
				}
			}
		}
		if (_state == enemy_state::DEAD)
		{
			action_interval[static_cast<int>(enemy_state::DEAD)] += 1;

			if (action_interval[(int)enemy_state::DEAD] > 30)
			{
				end_game = true;
				enemy_doing = true;
				boss_second_action = true;
			}

			if (_direction == 0)
			{
				enemy_action->setFrameX(_currentFrameX);
				if (action_interval[static_cast<int>(enemy_state::DEAD)] % 3 == 0)
				{
					_currentFrameX++;
				}

				if (_currentFrameX >= enemy_action->getMaxFrameX())
				{
					_currentFrameX = 1;
				}
			}
			if (_direction == 1)
			{
				enemy_action->setFrameX(_currentFrameX);

				if (action_interval[static_cast<int>(enemy_state::DEAD)] % 3 == 0)
				{
					_currentFrameX--;
				}

				if (_currentFrameX <= 0)
				{
					_currentFrameX = enemy_action->getMaxFrameX() - 1;
				}
			}
		}
		_count = 0;
	}
	// 현재 위치 갱신
	_current_stage = (_rc.left + _width) / WINSIZEX;

	move();
	enemy_state();
	get_pattern();
}

void boss::move()
{
	if (_state == enemy_state::ATTACK)
	{
		if (action_interval[static_cast<int>(enemy_state::ATTACK)] > 12)
		{
			if (_direction == 0)
			{
				_rc.left -= 13;
				_rc.right -= 13;
			}
			else
			{
				_rc.left += 13;
				_rc.right += 13;
			}
		}

	}
}

void boss::enemy_isHit()
{
	is_hit = true;
}

void boss::get_pattern()
{
	if (is_ground && _state == enemy_state::START && !enemy_doing)
	{
		_direction = 0;
		_currentFrameY = 0;

		enemy_doing = true;
		_state = enemy_state::IDLE;

		enemy_action = IMAGEMANAGER->findImage("boss_idle");
		enemy_action->setFrameY(_direction);
	}
	if (is_hit)
	{
		action_interval[static_cast<int>(enemy_state::HIDE )] += 1;

		if (action_interval[static_cast<int>(enemy_state::HIDE)] > 20)
		{
			is_hit = false;
			action_interval[static_cast<int>(enemy_state::HIDE)] = 0;
		}
	}

	if (_current_hp <= 0)
	{
		enemy_doing = true;
		stop = true;
		_state = enemy_state::DEAD;
		enemy_action = IMAGEMANAGER->findImage("boss_dead");
		enemy_action->setFrameY(_direction);
	}

	if (!enemy_doing)
	{
		boss_pattern = RND->getFromIntTo(0, 4);
		//boss_pattern = 3;
		enemy_doing = true;
		boss_second_action = false;

		switch (boss_pattern)
		{
		case 0:
			_state = enemy_state::ATTACK;
			enemy_action = IMAGEMANAGER->findImage("boss_blade");
			_currentFrameY = _direction;
			hitbox_width = 10;
			hitbox_height = 100;

			if (_direction == 0)
			{
				_currentFrameX = 0;
				_boss_hitbox = RectMake(_rc.right, _rc.top - 10, hitbox_width, hitbox_height);
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
				_boss_hitbox = RectMake(_rc.left - hitbox_width, _rc.top - 10, hitbox_width, hitbox_height);
			}
			break;

		case 1:
			_state = enemy_state::DEFENSE;
			enemy_action = IMAGEMANAGER->findImage("boss_defense");
			_currentFrameY = _direction;
			hitbox_width = 40;
			hitbox_height = 150;
			if (_direction == 0)
			{
				_currentFrameX = 0;
				_boss_hitbox = RectMake(_rc.left - hitbox_width, _rc.top - 25, hitbox_width, hitbox_height);
			}
			else
			{
				_currentFrameX = enemy_action->getMaxFrameX();
				_boss_hitbox = RectMake(_rc.right - hitbox_width/2, _rc.top - 25, hitbox_width, hitbox_height);
			}
			break;

		case 2:
			_state = enemy_state::THROW;
			enemy_action = IMAGEMANAGER->findImage("boss_throw");
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

		case 3:
			_state = enemy_state::ULT;
			enemy_action = IMAGEMANAGER->findImage("boss_ult");
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
		}
	}

}

void boss::draw()
{
	int x = _rc.left + _width / 2 - enemy_action->getFrameWidth() / 2;
	int y = _rc.top + _height / 2 - enemy_action->getFrameHeight() / 2;

	enemy_action->frameRender(getMemDC(), x, y, _currentFrameX, _currentFrameY);
}

void boss::make_idle()
{
	_state = enemy_state::IDLE;

	enemy_action = IMAGEMANAGER->findImage("boss_idle");
	_currentFrameY = _direction;
	if (_direction == 0)
	{
		_currentFrameX = 0;
	}
	else
		_currentFrameX = enemy_action->getMaxFrameX();
}

