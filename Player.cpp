#include "stdafx.h"
#include "Player.h"
#include "Object.h"

HRESULT Player::init()
{
	// 플레이어 애니메이션 이미지
	IMAGEMANAGER->addFrameImage("IDLE", "zero_idle.bmp", 2475, 600, 6, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("RUN", "zero_run.bmp", 6660, 600, 16, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("DASH", "zero_dash.bmp", 3690, 600, 9, 2, true, RGB(147, 187, 236), true);
	IMAGEMANAGER->addFrameImage("JUMP", "zero_jump2.bmp", 4920, 600, 12, 2, true, RGB(147, 187, 236), true);
	IMAGEMANAGER->addFrameImage("WALL", "zero_wall.bmp", 375, 500, 1, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("WALL_ATTACK", "zero_wall_attack.bmp", 4510, 600, 11, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("ATTACK1", "zero_attack1.bmp", 4920, 600, 12, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("ATTACK2", "zero_attack2.bmp", 4920, 600, 12, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("ATTACK3", "zero_attack3.bmp", 5740, 600, 14, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("ATTACK4", "zero_attack4.bmp", 1125, 500, 3, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("SHOOT", "zero_shoot.bmp", 4510, 600, 11, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("LADDER", "zero_ladder.bmp", 4875, 500, 13, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("SPECIAL", "zero_sa1.bmp", 4381, 680, 11, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("SPECIAL2", "zero_sa2.bmp", 6560, 600, 16, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("SPECIAL3", "zero_sa3.bmp", 5330, 600, 13, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("START", "zero_start.bmp", 6160, 600, 15, 2, true, RGB(147, 187, 236), false);
	IMAGEMANAGER->addFrameImage("JUMP_ATTACK", "zero_jump_attack.bmp", 3690, 600, 9, 2, true, RGB(147, 187, 236), true);
	IMAGEMANAGER->addFrameImage("HURT", "zero_hurt.bmp", 1640, 600, 4, 2, true, RGB(147, 187, 236), true);
	IMAGEMANAGER->addFrameImage("DEAD", "zero_dead.bmp", 1640, 600, 4, 2, true, RGB(147, 187, 236), true);

	// 플레이어 공격 이펙트 애니메이션 이미지
	IMAGEMANAGER->addFrameImage("bullet_effect", "effect_bullet.bmp", 480, 240, 4, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("hard_effect", "effect_hard.bmp", 600, 240, 5, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("blade_effect", "effect_blade.bmp", 720, 240, 6, 2, true, RGB(255, 0, 255), false);

	_player.body = RectMake(100, 300, 100, 100);
	_player.width = _player.body.right - _player.body.left;
	_player.height = _player.body.bottom - _player.body.top;
	_player.direction = 1;
	_player.on_ground = false;
	_player.on_flying_wall = false;
	_player.doing = true;
	_player.ladder = false;
	_state = START;
	_player.action = IMAGEMANAGER->findImage("START");
	_player.action->setFrameY(_player.direction);
	_player.is_charged = false;
	_player.is_hit = false;

	_HP = new HP_bar;
	_HP->init(_player.body.left - 100, _player.body.top - 300, 100, 150, "player");
	_current_hp = 100;
	_maxHP = 100;
	_HP->setGauge_V(_current_hp, _maxHP);

	_count = _index = temp_interval = 0;
	_left_index = _player.action->getMaxFrameX();
	_jump_power = 0.0f;
	_gravity = 0.0f;
	_dash_speed = 0.0f;
	_wall_friction = 0.0f;

	_edge_left = _edge_right = _edge_bottom = _edge_top = 0;
	_Ledge_left = _Ledge_right = _Ledge_bottom = _Ledge_top = 0;

	_bullet = new hyunMoo;
	_bullet->init(10, 700);

	_Special_bullet = new hyunMoo;
	_Special_bullet->init(30, 800);
	_bullet_dist = 200;

	_shadow_action = false;
	_second_action = false;
	effect_set = false;

	end = false;

	for (int i = 0; i < enum_Max; i++)
	{
		each_interval[i] = 0;
	}

	return S_OK;
}

void Player::release()
{
}

void Player::render()
{
	int x_pos = _player.body.left - _player.action->getFrameWidth() / 2 + _player.width / 2;
	int y_pos = _player.body.top - _player.action->getFrameHeight() / 2 + _player.height / 2;

	_player.action->frameRender(getMemDC(), x_pos, y_pos);

	if (_state == DASH || _state == JUMP || _state == JUMP_ATTACK)
	{
		if (_shadow_action)
		{
			if (_player.direction == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					_shadow_img[i]->alphaFrameRender(getMemDC(), x_pos + 15 + 15 * i, y_pos, 30);
				}

			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					_shadow_img[i]->alphaFrameRender(getMemDC(), x_pos - 15 - 15 * i, y_pos, 30);
				}
			}
		}

	}

	if (effect_set)
	{
		run_effect();
		_player.effect->frameRender(getMemDC(), effect_x, effect_y);
	}


	_bullet->render();
	_Special_bullet->render();
	_HP->render();

	//Rectangle(getMemDC(), _shadow.left, _shadow.top, _shadow.right, _shadow.bottom);
	//Rectangle(getMemDC(), _player.body.left, _player.body.top, _player.body.right, _player.body.bottom);
	/*
	char temp[128];
	sprintf_s(temp, " 플레이어 위치: %d, %d ", _state, _player.body.top);
	TextOut(getMemDC(), _player.body.left, _player.body.top - 200, temp, strlen(temp));
	*/
	//Rectangle(getMemDC(), _player.hit_box);
}

void Player::update()
{
	// 플레이어가 맵 바깥으로 나가지 못하도록
	if (_player.body.right > WINSIZEX * 11)
	{
		_player.body.right = WINSIZEX * 11;
		_player.body.left = _player.body.right - _player.width;
	}
	// 플레이어가 맵 바깥으로 나가지 못하도록
	if (_player.body.left < 0)
	{
		_player.body.left = 0;
		_player.body.right = _player.body.left + _player.width;
	}
	// 총알
	_bullet->update("player");
	// 필살기
	_Special_bullet->flasher_move();
	// HP
	_HP->setGauge_V(_current_hp, _maxHP);

	// HP가 0이하일시 게임종료 및 죽은모습으로 이미지 변환
	if ((_current_hp <= 0 && !end) || _player.body.top > WINSIZEY && !end)
	{
		end = true;
		_player.doing = true;

		_state = DEAD;
		_player.action = IMAGEMANAGER->findImage("DEAD");
		_player.action->setFrameY(_player.direction);

		if (_player.direction == 0)
		{
			_index = 0;
		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
		}
	}
	// 죽었으면 더이상 맞지않음
	if (end)
	{
		_player.is_hit = true;
		_current_hp = 0;
		if (_jump_power == 0)
		{
			_jump_power = 7;
		}
		if (!_player.on_ground)
		{
			_gravity = 0.2f;
			_player.body.top += _jump_power - _gravity;
			_player.body.bottom += _jump_power - _gravity;
		}
	}
	else
	{
		key_setting();
		move_img();
		define_status();
		based_on_status();
		collision_enemy();
	}

	// 피격시 무적시간을 정하기 위한 인터벌
	if (each_interval[HURT] >= 15)
	{
		each_interval[HURT] += 1;
	}
	// 무적시간 후 다시 피격가능설정
	if (each_interval[HURT] > 70)
	{
		_player.is_hit = false;
		each_interval[HURT] = 0;
	}

}

void Player::key_setting()
{
	// 아무행동을 취하지않고, 떨어지는중이 아닐떄, 방향키 오른쪽 누르면 달림
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _state != JUMP_DOWN && _state != SPECIAL2 && !_player.doing)
	{
		_player.action = IMAGEMANAGER->findImage("RUN");
		_player.direction = 1;
		_player.action->setFrameY(_player.direction);

		_state = RIGHT_RUN;
		_player.body.left += 5;
		_player.body.right += 5;
	}
	// 아무행동을 취하지않고, 떨어지는중이 아닐떄, 방향키 오른쪽을 떼면 오른쪽을 쳐다봄(IDLE)
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && !_player.doing && _state != SPECIAL2)
	{
		_player.action = IMAGEMANAGER->findImage("IDLE");
		_player.direction = 1;
		_player.action->setFrameY(_player.direction);
		_state = RIGHT_IDLE;
		_left_index = _player.action->getMaxFrameX();
	}
	// 아무행동을 취하지않고, 떨어지는중이 아닐떄, 왼쪽 누르면 달림
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_player.doing && _state != JUMP_DOWN && _state != SPECIAL2)
	{
		_player.action = IMAGEMANAGER->findImage("RUN");
		_player.direction = 0;
		_player.action->setFrameY(_player.direction);
		_state = LEFT_RUN;
		_player.body.left -= 5;
		_player.body.right -= 5;
	}
	// 아무행동을 취하지않고, 떨어지는중이 아닐떄, 방향키 왼쪽 떼면 왼쪽을 쳐다봄(IDLE)
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_player.doing && _state != SPECIAL2)
	{
		_player.action = IMAGEMANAGER->findImage("IDLE");
		_player.direction = 0;
		_player.action->setFrameY(_player.direction);
		_state = LEFT_IDLE;
		_index = 0;
	}
	// 제로의 1타 공격 모션
	if (KEYMANAGER->isOnceKeyDown('X') && !_player.doing)
	{
		_player.doing = true;
		_player.action = IMAGEMANAGER->findImage("ATTACK1");
		_player.action->setFrameY(_player.direction);

		hit_width = 60;
		hit_height = 60;
		if (_player.direction == 0)
		{
			_index = 0;
			_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - 20, hit_width, hit_height);

		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_player.hit_box = RectMake(_player.body.right, _player.body.top - 20, hit_width, hit_height);
		}

		_state = ATTACK;
	}
	/*
	// 제로의 2타 공격 모션
	if (KEYMANAGER->isOnceKeyDown('C') && !_player.doing)
	{
		_player.doing = true;
		_player.action = IMAGEMANAGER->findImage("ATTACK2");
		_player.action->setFrameY(_player.direction);

		hit_width = 120;
		hit_height = 40;
		if (_player.direction == 0)
		{
			_index = 0;
			_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - 20 + _player.height/2, hit_width, hit_height);
		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_player.hit_box = RectMake(_player.body.right, _player.body.top - 20 + _player.height / 2, hit_width, hit_height);
		}

		_state = ATTACK2;
	}

	// 제로의 3타 공격 모션
	if (KEYMANAGER->isOnceKeyDown('V') && !_player.doing)
	{
		_player.doing = true;
		_player.action = IMAGEMANAGER->findImage("ATTACK3");
		_player.action->setFrameY(_player.direction);

		hit_width = 60;
		hit_height = 60;
		if (_player.direction == 0)
		{
			_index = 0;
			_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - 20, hit_width, hit_height);

		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_player.hit_box = RectMake(_player.body.right, _player.body.top - 20, hit_width, hit_height);
		}

		_state = ATTACK3;
	}
	*/

	/* 공격 후 칼 집어넣는 모션임
	if (KEYMANAGER->isOnceKeyDown('B') && !_player.doing)
	{
		_player.doing = true;
		_player.action = IMAGEMANAGER->findImage("ATTACK4");
		_player.action->setFrameY(_player.direction);

		if (_player.direction == 0)
		{
			_index = 0;
		}
		else
			_left_index = _player.action->getMaxFrameX();

		_state = ATTACK4;
	}
	*/
	// 대쉬 모션
	if (KEYMANAGER->isOnceKeyDown('Z') && !_player.doing)
	{
		_player.action = IMAGEMANAGER->findImage("DASH");
		_player.action->setFrameY(_player.direction);
		_player.doing = true;
		_shadow_action = true;
		_dash_speed = 5.0f;

		for (int i = 0; i < 4; i++)
		{
			_shadow_img[i] = IMAGEMANAGER->findImage("DASH");
			_shadow_img[i]-> setFrameY(_player.direction);
		}

		if (_player.direction == 1)
		{
			_index = 0;
		}
		else
			_left_index = _player.action->getMaxFrameX();

		_state = DASH;
	}
	if (KEYMANAGER->isOnceKeyDown('A') && !_player.doing)
	{
		_player.doing = true;
		_player.action = IMAGEMANAGER->findImage("SHOOT");

		if (_player.direction == 0)
		{
			_index = 0;
			_player.action->setFrameY(0);
		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_player.action->setFrameY(1);
		}

		_state = SHOOT;
	}
	if (KEYMANAGER->isOnceKeyUp('A') && !_player.is_charged && _player.doing)
	{
		_player.is_charged = true;

		if (_player.direction == 0)
		{
			_index = 0;
			_bullet->fire(_player.body.left - 10, _player.body.top + 20, 0, "bullet.bmp");
			_player.action->setFrameY(0);
		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_bullet->fire(_player.body.right + 10, _player.body.top + 20, 1, "bullet.bmp");
			_player.action->setFrameY(1);
		}
	}
	// 스페이스를 누르면 점프
	if (KEYMANAGER->isStayKeyDown(VK_SPACE) && !_player.doing)
	{
		_player.doing = true;
		_player.on_ground = false;
		_player.on_flying_wall = false;
		_player.action = IMAGEMANAGER->findImage("JUMP");
		_player.action->setFrameY(_player.direction);
		_shadow_action = false;

		if (_player.direction == 0)
		{
			_index = 0;
		}
		else
			_left_index = _player.action->getMaxFrameX();

		_state = JUMP;
		_gravity = 0.2f;
		_jump_power = 8.0f;
	}

	if (_player.ladder && KEYMANAGER->isStayKeyDown(VK_DOWN) &&
		!_player.doing && _player.body.bottom < _Ledge_bottom)
	{
		_player.doing = true;
		_player.on_ground = false;
		_player.action = IMAGEMANAGER->findImage("LADDER");
		_player.action->setFrameY(0);

		_state = LADDER;
		_index = 0;

		_player.body.top = _Ledge_top - 2;
		_player.body.bottom = _player.body.top + _player.height;
		_player.body.left = _Ledge_left;
		_player.body.right = _player.body.left + _player.width;
	}

	if (_player.ladder && KEYMANAGER->isStayKeyDown(VK_UP) &&
		!_player.doing && _player.body.bottom >= _Ledge_bottom)
	{
		_player.doing = true;
		_player.on_ground = false;
		_player.action = IMAGEMANAGER->findImage("LADDER");
		_player.action->setFrameY(0);

		_state = LADDER;
		_index = 0;

		_player.body.bottom = _Ledge_bottom - 2;
		_player.body.top = _player.body.bottom - _player.height;
		_player.body.left = _Ledge_left;
		_player.body.right = _player.body.left + _player.width;
	}

	if (KEYMANAGER->isOnceKeyDown('S') && KEYMANAGER->isStayKeyDown(VK_UP)
		&& !_player.doing && _state != JUMP_DOWN)
	{
		_player.body.top -= 1.0f;
		_player.body.bottom = _player.body.top + _player.height;

		_player.doing = true;
		_player.on_ground = false;
		_player.on_flying_wall = false;

		_player.action = IMAGEMANAGER->findImage("SPECIAL");
		_state = SPECIAL1;

		hit_width = 80;
		hit_height = 130;

		if (_player.direction == 0)
		{
			_index = 0;
			_player.action->setFrameY(_player.direction);
			_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - hit_height, hit_width, 200);
		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_player.action->setFrameY(_player.direction);
			_player.hit_box = RectMake(_player.body.right, _player.body.top - hit_height, hit_width, 200);
		}

	}

	if (KEYMANAGER->isOnceKeyDown('D') && !_player.doing)
	{
		_player.doing = true;

		_player.action = IMAGEMANAGER->findImage("SPECIAL3");
		_state = SPECIAL3;

		if (_player.direction == 0)
		{
			_index = 0;
			_player.action->setFrameY(_player.direction);
		}
		else
		{
			_left_index = _player.action->getMaxFrameX();
			_player.action->setFrameY(_player.direction);
		}
	}
}

void Player::move_img()
{
	_count++;

	if (_count % 2 == 0)
	{
		//오른쪽 IDLE
		if (_state == RIGHT_IDLE)
		{
			_player.action->setFrameX(_index);
			each_interval[RIGHT_IDLE]++;

			if (each_interval[RIGHT_IDLE] % 6 == 0)
			{
				_index++;
			}

			if (_index >= 4) _index = 0;
		}
		//왼쪽 IDLE
		if (_state == LEFT_IDLE)
		{
			_player.action->setFrameX(_index);
			each_interval[LEFT_IDLE]++;

			if (each_interval[LEFT_IDLE] % 6 == 0)
			{
				_index--;
			}
			if (_index <= 0) _index = _player.action->getMaxFrameX();
		}
		//오른쪽 달리기
		if (_state == RIGHT_RUN)
		{
			_player.action->setFrameX(_index);
			_index++;

			if (_index >= _player.action->getMaxFrameX()) _index = 2;
		}
		//왼쪽 달리기
		if (_state == LEFT_RUN)
		{
			_player.action->setFrameX(_left_index);
			_left_index--;

			if (_left_index <= 0) _left_index = _player.action->getMaxFrameX() - 2;
		}
		// 1타 공격
		if (_state == ATTACK)
		{
			hit_width += 13;
			hit_height += 13;
			if (hit_width > 110)
			{
				hit_width = 110;
			}
			if (hit_height > 130)
			{
				hit_height = 130;
			}

			each_interval[ATTACK]++;

			if (_player.direction == 0) // 오른쪽 방향
			{
				_player.action->setFrameX(_index);
				_index++;

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					hit_width = hit_height = 0;
				}
			}
			else // 왼쪽 방향
			{
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index <= 0)
				{
					_left_index = 0;
					_player.doing = false;
					hit_width = hit_height = 0;
				}
			}
		}
		// 2타 공격
		if (_state == ATTACK2)
		{
			each_interval[ATTACK2]++;
			if (_player.direction == 0) // 오른쪽방향
			{
				_player.action->setFrameX(_index);
				_index++;
				_player.hit_box.right += 30;

				if (_player.hit_box.right - 80 > _player.body.right)
				{
					_player.hit_box.right = _player.body.right + 80;
				}
				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					hit_width = hit_height = 0;
					_player.hit_box.right = _player.hit_box.left;
				}
			}
			else // 왼쪽방향
			{
				_player.action->setFrameX(_left_index);
				_left_index--;
				_player.hit_box.left -= 30;

				if (_player.hit_box.left + 80 < _player.body.left)
				{
					_player.hit_box.left = _player.body.left - 80;
				}
				if (_left_index <= 0)
				{
					_player.hit_box.left = _player.hit_box.right;
					hit_width = hit_height = 0;
					_left_index = 0;
					_player.doing = false;
				}
			}
		}
		// 3타 공격
		if (_state == ATTACK3)
		{
			hit_width += 20;
			hit_height += 20;
			if (hit_width > 150)
			{
				hit_width = 150;
			}
			if (hit_height > 150)
			{
				hit_height = 150;
			}
			each_interval[ATTACK3]++;
			if (_player.direction == 0) // 오른쪽 방향
			{
				_player.action->setFrameX(_index);
				_index++;

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					hit_width = hit_height = 0;
				}
			}
			else // 왼쪽 방향
			{
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index <= 0)
				{
					_left_index = 0;
					hit_width = hit_height = 0;
					_player.doing = false;
				}
			}
		}
		/*
		if (_state == ATTACK4)
		{
			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);
				_index++;

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
				}
			}
			else
			{
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index <= 0)
				{
					_left_index = 0;
					_player.doing = false;
				}
			}
		}
		*/
		// 대쉬
		if (_state == DASH)
		{
			if (_player.direction == 1) // 오른쪽방향
			{
				_player.action->setFrameX(_index);

				if (_shadow_action)
				{
					for (int i = 0; i < 4; i++)
					{
						_shadow_img[i]->setFrameX(_index);
					}
				}
				each_interval[DASH]++;
				if (each_interval[DASH] % 2 == 0)
				{
					_index++;

				}

				if (_index >= _player.action->getMaxFrameX())
				{
					_index = 0;
					_dash_speed = 0;
					each_interval[DASH] = 0;
					_player.doing = false;
					_state = RIGHT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}
			}
			else if (_player.direction == 0) // 왼쪽방향
			{
				_player.action->setFrameX(_left_index);

				if (_shadow_action)
				{
					for (int i = 0; i < 4; i++)
					{
						_shadow_img[i]->setFrameX(_left_index);
					}
				}

				each_interval[DASH]++;
				if (each_interval[DASH] % 2 == 0)
				{
					_left_index--;

				}

				if (_left_index <= 0)
				{
					_left_index = 0;
					_dash_speed = 0;
					_player.doing = false;
					_state = LEFT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}
			}
		}
		// 점프나 떨어지는 중일떄
		if (_state == JUMP || _state == JUMP_DOWN)
		{
			if (_player.direction == 0) // 오른쪽방향
			{
				_player.action->setFrameX(_index);

				for (int i = 0; i < 4; i++)
				{
					//_shadow_img[i]->setFrameX(_index);
				}
				_index++;

				if (_index >= _player.action->getMaxFrameX() - 1)
				{
					_index = _player.action->getMaxFrameX() - 1;
				}
			}
			if (_player.direction == 1) // 왼쪽방향
			{
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index <= 1)
				{
					_left_index = 1;
				}
			}
		}

		// 벽에 붙어있을시
		if (_state == WALL)
		{
			_player.action->setFrameX(0);
		}

		if (_state == LADDER)
		{
			_player.action->setFrameX(_index);
			_index++;

			if (_index > 8)
			{
				_index = 0;
			}

			// 사다리에서 내려왔을때 착지시 
			if (_edge_top >= _player.body.top)
			{
				_player.doing = false;
				_player.ladder = false;
				_player.on_ground = true;
				_state = RIGHT_IDLE;

				_player.action = IMAGEMANAGER->findImage("IDLE");
				_player.direction = 0;
				_player.action->setFrameY(_player.direction);
				_index = 0;
			}	
		}

		if (_state == WALL_ATTACK)
		{
			if (_player.direction == 1)
			{
				_player.direction = 1;
				_player.action->setFrameX(_index);
				_index++;

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					_second_action = false;
					_state = WALL;
					hit_width = hit_height = 0;

					_player.action = IMAGEMANAGER->findImage("WALL");
					_player.action->setFrameY(0);
				}
			}
			// 땅에 다 내려올떄 왼쪽 방향으로 IDLE 상태가 됩니다.
			if (_player.direction == 0)
			{
				_player.direction = 0;
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index <= 0)
				{
					_player.doing = false;
					_second_action = false;
					_state = WALL;
					hit_width = hit_height = 0;

					_player.action = IMAGEMANAGER->findImage("WALL");
					_player.action->setFrameY(1);
				}
			}
		}
		if (_state == SHOOT)
		{
			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);
				_index++;

				if (_index >= 4 && !_player.is_charged)
				{
					_index = 1;
				}

				if (_index < 4 && _player.is_charged)
				{
					_index = 5;
				}

				if (_index >= _player.action->getMaxFrameX())
				{
					_index = 0;
					_player.doing = false;
					_player.is_charged = false;

					_state = LEFT_IDLE;
					_player.direction = 0;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}

			}
			// 땅에 다 내려올떄 왼쪽 방향으로 IDLE 상태가 됩니다.
			if (_player.direction == 1)
			{
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index + 3 <= _player.action->getMaxFrameX() && !_player.is_charged)
				{
					_left_index = _player.action->getMaxFrameX();
				}
				if (_left_index + 3 > _player.action->getMaxFrameX() && _player.is_charged)
				{
					_left_index = _left_index - 4;
				}
				if (_left_index <= 0)
				{
					_left_index = _player.action->getMaxFrameX();
					_player.doing = false;
					_player.is_charged = false;

					_state = RIGHT_IDLE;
					_player.direction = 1;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}
			}
		}

		if (_state == SPECIAL1)
		{
			each_interval[SPECIAL1]++;

			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);

				if (each_interval[SPECIAL1] % 2 == 0)
				{
					_index++;
				}
				_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - hit_height, hit_width, 200);

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					_index = 0;
					hit_width = hit_height = 0;
					each_interval[SPECIAL1] = 0;

					_state = JUMP_DOWN;
					_player.direction = 0;
					_player.action = IMAGEMANAGER->findImage("JUMP");
					_player.action->setFrameY(_player.direction);
				}
			}
			else
			{
				_player.action->setFrameX(_left_index);

				if (each_interval[SPECIAL1] % 2 == 0)
				{
					_left_index--;
				}
				_player.hit_box = RectMake(_player.body.right, _player.body.top - hit_height, hit_width, 200);
				
				if (_left_index <= 0)
				{
					_player.doing = false;
					hit_width = hit_height = 0;
					each_interval[SPECIAL1] = 0;

					_state = JUMP_DOWN;
					_player.direction = 1;
					_player.action = IMAGEMANAGER->findImage("JUMP");
					_player.action->setFrameY(_player.direction);

				}
			}
		}

		if (_state == SPECIAL2)
		{
			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);
				_index++;
				_player.hit_box = RectMake(_player.body.left + _player.width / 2 - hit_width, _player.body.top + _player.height / 2, hit_width, hit_height);
				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					_index = 0;
					_state = LEFT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}
			}

			else if (_player.direction == 1)
			{
				_player.action->setFrameX(_left_index);
				_left_index--;
				_player.hit_box = RectMake(_player.body.left + _player.width/2, _player.body.top + _player.height / 2, hit_width, hit_height);
				if (_left_index <= 0)
				{
					_player.doing = false;
					_state = RIGHT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}
			}

		}

		if (_state == SPECIAL3)
		{
			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);
				each_interval[SPECIAL3]++;
				if (each_interval[SPECIAL3] % 3 == 0)
				{
					_index++;
				}

				if (_index > 5 && _index <= 9)
				{
					_Special_bullet->flasher_fire(_player.body.right + _bullet_dist, _player.body.bottom);
					_Special_bullet->flasher_fire(_player.body.left - _bullet_dist, _player.body.bottom);
					_bullet_dist += 250;
				}

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					_index = 0;
					_bullet_dist = 200;
					_state = LEFT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
					each_interval[SPECIAL3] = 0;
				}
			}

			else
			{
				_player.action->setFrameX(_left_index);
				each_interval[SPECIAL3]++;
				if (each_interval[SPECIAL3] % 3 == 0)
				{
					_left_index--;
				}


				if (_left_index > 2 && _left_index <= 6)
				{
					_Special_bullet->flasher_fire(_player.body.right + _bullet_dist, _player.body.bottom);
					_Special_bullet->flasher_fire(_player.body.left - _bullet_dist, _player.body.bottom);
					_bullet_dist += 250;
				}

				if (_left_index <= 0)
				{
					_player.doing = false;
					_bullet_dist = 200;
					_state = RIGHT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
					each_interval[SPECIAL3] = 0;
				}
			}
		}

		if (_state == START)
		{
			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);
				_index++;

				if (_index > 1 && !_player.on_ground)
				{
					_index = 0;
				}

				if (_index >= _player.action->getMaxFrameX())
				{
					_player.doing = false;
					_state = LEFT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
					_index = 0;
				}
			}
			else
			{
				_player.action->setFrameX(_left_index);
				_left_index--;

				if (_left_index + 1 >= _player.action->getMaxFrameX() && !_player.on_ground)
				{
					_left_index = _player.action->getMaxFrameX();
				}

				if (_left_index <= 0)
				{
					_player.doing = false;
					_state = RIGHT_IDLE;
					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(_player.direction);
				}
			}
		}

		if (_state == JUMP_ATTACK)
		{
			if (_player.direction == 0)
			{
				_player.action->setFrameX(each_interval[JUMP_ATTACK]);
				each_interval[JUMP_ATTACK]++;
				_player.hit_box = RectMake(_player.body.right - hit_width, _player.body.top - 20, hit_width, hit_height);
				if (each_interval[JUMP_ATTACK] >= _player.action->getMaxFrameX())
				{
					_second_action = false;
					hit_width = hit_height = 0;
					_state = JUMP;
					_player.action = IMAGEMANAGER->findImage("JUMP");
					_player.action->setFrameY(_player.direction);
				}
			}

			else
			{
				_player.action->setFrameX(each_interval[JUMP_ATTACK]);
				each_interval[JUMP_ATTACK]--;
				_player.hit_box = RectMake(_player.body.left, _player.body.top - 20, hit_width, hit_height);
				if (each_interval[JUMP_ATTACK] <= 0)
				{
					_second_action = false;
					hit_width = hit_height = 0;
					_state = JUMP;
					_player.action = IMAGEMANAGER->findImage("JUMP");
					_player.action->setFrameY(_player.direction);
				}
			}
		}
		if (_state == HURT)
		{
			each_interval[HURT]++;

			if (_player.direction == 0)
			{
				_player.action->setFrameX(_index);

				if (each_interval[HURT] % 5 == 0)
				{
					_index++;
				}

				if (_index >= _player.action->getMaxFrameX())
				{
					_state = LEFT_IDLE;
					_player.doing = false;

					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(0);
					_index = 0;
				}
			}
			else
			{
				_player.action->setFrameX(_left_index);
				if (each_interval[HURT] % 5 == 0)
				{
					_left_index--;
				}

				if (_left_index <= 0)
				{
					_state = RIGHT_IDLE;
					_player.doing = false;

					_player.action = IMAGEMANAGER->findImage("IDLE");
					_player.action->setFrameY(1);
					_left_index = _player.action->getMaxFrameX();
				}
			}
		}
		_count = 0;

		if (_player.direction == 0 && _state != ATTACK2 && _state != SPECIAL1 && _state != SPECIAL2 && _state != JUMP_ATTACK && _state != WALL_ATTACK)
		{
			_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - 20, hit_width, hit_height);
		}
		else if(_player.direction == 1 &&_state != ATTACK2 && _state != SPECIAL1 && _state != SPECIAL2 && _state != JUMP_ATTACK && _state != WALL_ATTACK)
			_player.hit_box = RectMake(_player.body.right, _player.body.top - 20, hit_width, hit_height);
	}
}
void Player::get_damage(float dmg)
{
	_current_hp -= dmg;
}
/*
void Player::check_crash()
{
	RECT temp;
	RECT temp_Player = RectMake(_player.body.left, _player.body.top + 1, _player.width, _player.height);

	for (int i = 0; i < 3; i++)
	{
		if (IntersectRect(&temp, &temp_Player, &_ladder[i].rc))
		{
			_Ledge_top = _ladder[i].rc.top;
			_Ledge_bottom = _ladder[i].rc.bottom;
			_Ledge_left = _ladder[i].rc.left;
			_Ledge_right = _ladder[i].rc.right;
		}
	}
}
*/
void Player::define_status()
{
	// 점프도안했고 땅위에도 없으며 벽에 매달리지도 않았다 -> 내려가는중(JUMP_DOWN)
	if (_state != JUMP && !_player.on_ground && _state != WALL && _state != LADDER && _state != WALL_ATTACK
		&& _state != START && _state != SPECIAL1 && _state != SPECIAL2 && _state != JUMP_ATTACK && _state != HURT)
	{
		_state = JUMP_DOWN;
	}

	if ((_player.body.left + _player.width / 2) > _Ledge_left &&
		(_player.body.left + _player.width / 2) < _Ledge_right)
	{
		if (_Ledge_top == _player.body.bottom || _Ledge_bottom == _player.body.bottom)
		{
			_player.ladder = true;
		}
		else
			_player.ladder = false;
	}
	else
		_player.ladder = false;
}

void Player::based_on_status()
{
	// 점프중인 상태에서는 기본적으로 하강.
	if (_state == JUMP)
	{
		_player.body.top -= _jump_power - _gravity;
		_player.body.bottom -= _jump_power - _gravity;

		_gravity += 0.2f;

		if (_wall_friction != 0)
		{
			_wall_friction += 1;
		}

		if (_wall_friction < 7)
		{
			if (_player.direction == 0)
			{
				_player.body.left += _wall_friction;
				_player.body.right += _wall_friction;
			}
			else
			{
				_player.body.left -= _wall_friction;
				_player.body.right -= _wall_friction;
			}
		}
		else
			_second_action = false;

		if(!_second_action)
			key_pressed();

		// 점프하면서 왼쪽을 누르면 오른쪽으로 이동
		if (KEYMANAGER->isStayKeyDown(VK_DOWN) && KEYMANAGER->isStayKeyDown('S') && _state == JUMP)
		{
			_state = SPECIAL2;
			_player.action = IMAGEMANAGER->findImage("SPECIAL2");
			hit_width = 60;
			hit_height = 150;

			if (_player.direction == 0)
			{
				_index = 0;
				_player.action->setFrameY(0);
				_player.hit_box = RectMake(_player.body.left + _player.width / 2, _player.body.top + _player.height / 2, hit_width, hit_height);
			}
			else
			{
				_left_index = _player.action->getMaxFrameX();
				_player.hit_box = RectMake(_player.body.left + _player.width / 2 - hit_width, _player.body.top + _player.height / 2, hit_width, hit_height);
				_player.action->setFrameY(1);
			}

		}

		if (KEYMANAGER->isStayKeyDown('X') && !_second_action)
		{
			_state = JUMP_ATTACK;
			_second_action = true;
			_shadow_action = false;

			_player.action = IMAGEMANAGER->findImage("JUMP_ATTACK");
			hit_width = 220;
			hit_height = 130;

			if (_player.direction == 0)
			{
				each_interval[JUMP_ATTACK] = 0;
				_player.action->setFrameY(_player.direction);
				_player.hit_box = RectMake(_player.body.right - hit_width, _player.body.top - 20, hit_width, hit_height);
			}
			else
			{
				each_interval[JUMP_ATTACK] = _player.action->getMaxFrameX();
				_player.action->setFrameY(_player.direction);
				_player.hit_box = RectMake(_player.body.left, _player.body.top - 20, hit_width, hit_height);
			}
		}
		land();
	}
	// 벽을 붙잡고 있는 상태에서는 기본적으로 조금씩 하강.
	if (_state == WALL)
	{
		_gravity = 1.0f;
		_jump_power = 0.0f;
		_dash_speed = 0;
		_wall_friction = 0;
		_player.body.top += _jump_power + _gravity;
		_player.body.bottom += _jump_power + _gravity;

		// SPACE를 누를시 점프한다.
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_player.doing = true;
			_second_action = true;
			_player.action = IMAGEMANAGER->findImage("JUMP");
			_state = JUMP;
			_gravity = 0.2f;
			_jump_power = 8.0f;
			_wall_friction = 1.0f;

			_player.action->setFrameY(_player.direction);
		}

		if (KEYMANAGER->isStayKeyDown('X') && !_second_action)
		{
			_player.doing = true;
			_second_action = true;
			_player.action = IMAGEMANAGER->findImage("WALL_ATTACK");
			_state = WALL_ATTACK;

			hit_width = 100;
			hit_height = 130;

			if (_player.direction == 0)
			{
				_left_index = _player.action->getMaxFrameX();
				_player.action->setFrameY(1);
				_player.hit_box = RectMake(_player.body.left + hit_width, _player.body.top, hit_width, hit_height);
			}
			else if(_player.direction == 1)
			{
				_index = 0;
				_player.action->setFrameY(0);
				_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top, hit_width, hit_height);
			}
		}

		land();
	}

	if (_state == WALL_ATTACK)
	{
		_gravity = 1.0f;
		_jump_power = 0.0f;
		_player.body.top += _jump_power + _gravity;
		_player.body.bottom += _jump_power + _gravity;

		land();
	}

	// 땅위에 올라와있을시 중력 0, 점프력 0
	if (_player.on_ground)
	{
		_gravity = 0.0f;
		_jump_power = 0.0f;
		_wall_friction = 0.0f;
	}

	// 부유한 땅 위에 올라와있을시
	if (_player.on_flying_wall)
	{
		// 아래 방향키와 SPACE 키를 같이 누르면 JUMP_DOWN 상태가 되며 땅으로 하강함.
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_player.doing = true;
			if (_player.doing && KEYMANAGER->isStayKeyDown(VK_SPACE))
			{
				_player.on_ground = false;
				_state = JUMP_DOWN;
			}
		}
		if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
		{
			_player.doing = false;
		}
	}

	// 하강 상태일때 기본적으로 하강
	if (_state == JUMP_DOWN)
	{
		_gravity += 0.25f;
		_player.body.top += _gravity;
		_player.body.bottom += _gravity;

		// 오른쪽 방향
		if (_player.direction == 0)
		{
			_player.action = IMAGEMANAGER->findImage("JUMP");
			_player.action->setFrameY(_player.direction);
			_player.action->setFrameX(9);

		}
		// 왼쪽 방향
		else
		{
			_player.action = IMAGEMANAGER->findImage("JUMP");
			_player.action->setFrameY(_player.direction);
			_player.action->setFrameX(1);
		}
		// 왼쪽을 누르면 하강하면서 왼쪽으로 이동
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_player.direction = 0;
			_player.action->setFrameY(_player.direction);
			_player.body.left -= 3;
			_player.body.right -= 3;
		}
		// 왼쪽을 누르면 하강하면서 오른쪽으로 이동
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_player.direction = 1;
			_player.action->setFrameY(_player.direction);
			_player.body.left += 3;
			_player.body.right += 3;
		}
		if (KEYMANAGER->isStayKeyDown('X') && !_second_action)
		{
			_state = JUMP_ATTACK;
			_second_action = true;
			_shadow_action = false;

			_player.action = IMAGEMANAGER->findImage("JUMP_ATTACK");
			hit_width = 220;
			hit_height = 130;

			if (_player.direction == 0)
			{
				each_interval[JUMP_ATTACK] = 0;
				_player.action->setFrameY(_player.direction);
				_player.hit_box = RectMake(_player.body.right - hit_width, _player.body.top - 20, hit_width, hit_height);
			}
			else
			{
				each_interval[JUMP_ATTACK] = _player.action->getMaxFrameX();
				_player.action->setFrameY(_player.direction);
				_player.hit_box = RectMake(_player.body.left, _player.body.top - 20, hit_width, hit_height);
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_DOWN) && KEYMANAGER->isStayKeyDown('S') && _state == JUMP_DOWN)
		{
			_state = SPECIAL2;
			_player.action = IMAGEMANAGER->findImage("SPECIAL2");

			hit_width = 60;
			hit_height = 150;

			if (_player.direction == 0)
			{
				_index = 0;
				_player.action->setFrameY(0);
				_player.hit_box = RectMake(_player.body.left + _player.width / 2 - hit_width, _player.body.top + _player.height / 2, hit_width, hit_height);
			}
			else
			{
				_left_index = _player.action->getMaxFrameX();
				_player.hit_box = RectMake(_player.body.left + _player.width / 2, _player.body.top + _player.height / 2, hit_width, hit_height);
				_player.action->setFrameY(1);
			}

		}
		// 착지시 점프할떄 바라보던 방향으로 IDLE 상태가 됩니다.
		land();
	}

	if (_state == LADDER)
	{
		key_pressed();
	}

	if (_state == DASH)
	{
		if (_player.direction == 0)
		{
			_player.body.left -= 5 + _dash_speed;
			_player.body.right -= 5 + _dash_speed;
		}
		else
		{
			_player.body.left += 5 + _dash_speed;
			_player.body.right += 5 + _dash_speed;
		}

		// SPACE를 누를시 점프한다.
 		if (KEYMANAGER->isStayKeyDown(VK_SPACE))
		{
			_state = JUMP;
			_gravity = 0.2f;
			_jump_power = 8.0f;

			_player.body.top -= _jump_power - _gravity;
			_player.body.bottom -= _jump_power - _gravity;

			_player.action = IMAGEMANAGER->findImage("JUMP");
			_player.action->setFrameY(_player.direction);

			_player.on_ground = false;
			_player.on_flying_wall = false;

			for (int i = 0; i < 4; i++)
			{
				_shadow_img[i] = IMAGEMANAGER->findImage("JUMP");
				_shadow_img[i]->setFrameY(_player.direction);
			}

		}
	}
	if (_state == SPECIAL1)
	{
		if (_player.direction == 0)
		{
			_player.body.left -= 3.0f;
			_player.body.right = _player.body.left + _player.width;
			_player.body.top -= 5.0f;
			_player.body.bottom = _player.body.top + _player.height;
		}
		else
		{
			_player.body.left += 3.0f;
			_player.body.right = _player.body.left + _player.width;
			_player.body.top -= 5.0f;
			_player.body.bottom = _player.body.top + _player.height;
		}
	}

	if (_state == SPECIAL2)
	{
		_player.body.top -= _jump_power - _gravity;
		_player.body.bottom -= _jump_power - _gravity;
		_gravity += 0.2f;

		if (_player.direction == 0)
		{
			if (_index >= 5 && !_player.on_ground)
			{
				_index = 3;
			}
			else if (_index < 5 && _player.on_ground)
			{
				hit_width = hit_height = 0;
				_index = 7;
			}
		}
		else
		{
			if (_left_index + 5 >= _player.action->getMaxFrameX() && !_player.on_ground)
			{
				_player.doing = false;
				_left_index = _player.action->getMaxFrameX() - 4;

			}
			else if (_left_index + 5 >= _player.action->getMaxFrameX() && _player.on_ground)
			{
				hit_width = hit_height = 0;
				_index = _player.action->getMaxFrameX() - 7;
			}
		}
	}

	if (_state == START)
	{
		_player.body.top -= _jump_power - _gravity;
		_player.body.bottom -= _jump_power - _gravity;
		_gravity += 0.2f;
	}

	if (_state == JUMP_ATTACK)
	{
		_player.body.top -= _jump_power - _gravity;
		_player.body.bottom -= _jump_power - _gravity;
		_gravity += 0.2f;

		key_pressed();
		land();
	}

	if (_state == LEFT_IDLE || _state == RIGHT_IDLE)
	{
		hit_width = hit_height = 0;
	}

	if (_state == ATTACK)
	{
		if (each_interval[ATTACK] > 12)
		{
			each_interval[ATTACK]++;
		}

		if (each_interval[ATTACK] > 6)
		{
			// 제로의 2타 공격 모션
			if (KEYMANAGER->isOnceKeyDown('C'))
			{
				_state = ATTACK2;
				_player.doing = true;
				_player.action = IMAGEMANAGER->findImage("ATTACK2");
				_player.action->setFrameY(_player.direction);

				hit_width = 120;
				hit_height = 40;
				if (_player.direction == 0)
				{
					_index = 0;
					_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - 20 + _player.height / 2, hit_width, hit_height);
				}
				else
				{
					_left_index = _player.action->getMaxFrameX();
					_player.hit_box = RectMake(_player.body.right, _player.body.top - 20 + _player.height / 2, hit_width, hit_height);
				}
				each_interval[ATTACK] = 0;
			}
		}
		if (each_interval[ATTACK] > 20)
		{
			each_interval[ATTACK] = 0;
		}
	}

	if (_state == ATTACK2)
	{
		if (each_interval[ATTACK2] > 12)
		{
			each_interval[ATTACK2]++;
		}
		if (each_interval[ATTACK2] > 6)
		{
			// 제로의 3타 공격 모션
			if (KEYMANAGER->isOnceKeyDown('V'))
			{
				_state = ATTACK3;
				_player.doing = true;
				_player.action = IMAGEMANAGER->findImage("ATTACK3");
				_player.action->setFrameY(_player.direction);

				hit_width = 60;
				hit_height = 60;
				if (_player.direction == 0)
				{
					_index = 0;
					_player.hit_box = RectMake(_player.body.left - hit_width, _player.body.top - 20, hit_width, hit_height);

				}
				else
				{
					_left_index = _player.action->getMaxFrameX();
					_player.hit_box = RectMake(_player.body.right, _player.body.top - 20, hit_width, hit_height);
				}
				each_interval[ATTACK2] = 0;
			}
		}
		if (each_interval[ATTACK2] > 20)
		{
			each_interval[ATTACK2] = 0;
		}
	}
	if (_state == DEAD)
	{
		each_interval[DEAD]++;
		if (_player.direction == 0)
		{
			_player.action->setFrameX(_index);
			if (each_interval[DEAD] % 8 == 0)
			{
				_index++;
			}
			if (_index > _player.action->getMaxFrameX())
			{
				_index = _player.action->getMaxFrameX();
			}
		}
		else
		{
			_player.action->setFrameX(_left_index);
			if (each_interval[DEAD] % 8 == 0)
			{
				_left_index--;
			}
			if (_left_index <= 0)
			{
				_left_index = 0;
			}
		}
	}
	if (_state == HURT)
	{
		_gravity = 0;
		if (_player.direction == 0)
		{
			_player.body.left += 1.0f;
			_player.body.right += 1.0f;
		}
		else
		{
			_player.body.left -= 1.0f;
			_player.body.right -= 1.0f;
		}
	}

}


void Player::land()
{
	if (_player.on_ground)
	{
		_player.body.bottom = _edge_top;
		_player.body.top = _player.body.bottom - _player.height;
		_player.doing = false;
		_second_action = false;

		_player.action = IMAGEMANAGER->findImage("IDLE");
		if (_player.direction == 0)
		{
			_state = LEFT_IDLE;
			_index = 0;
			_player.action->setFrameY(_player.direction);
		}
		else
		{
			_state = RIGHT_IDLE;
			_player.action->setFrameY(_player.direction);
		}
	}
}

void Player::make_rect(LONG x, LONG y)
{
	_player.body = RectMake(x, y, _player.width, _player.height);
}

void Player::key_pressed()
{
	if (_state == JUMP || _state == JUMP_ATTACK)
	{
		// 점프하면서 왼쪽을 누르면 왼쪽으로 이동
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_player.direction = 0;
			_player.action->setFrameY(_player.direction);
			_player.body.left -= 5 + _dash_speed;
			_player.body.right -= 5 + _dash_speed;
		}

		// 점프하면서 왼쪽을 누르면 오른쪽으로 이동
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_player.direction = 1;
			_player.action->setFrameY(_player.direction);
			_player.body.left += 5 + _dash_speed;
			_player.body.right += 5 + _dash_speed;
		}
	}
	else
	{
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_player.body.top += 5;
			_player.body.bottom += 5;
		}
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			_player.body.top -= 5;
			_player.body.bottom -= 5;
		}
	}

}