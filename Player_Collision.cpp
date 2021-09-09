#include "stdafx.h"
#include "Player.h"
#include "enemyManager.h"

void Player::check_ground(LONG top)
{
	_edge_left = 0;
	_edge_right = WINSIZEX;
	_edge_bottom = WINSIZEY;
	_edge_top = top;

	_second_action = false;

	if (_state != SPECIAL2 && _state != START && _state != DEAD)
	{
		_player.action = IMAGEMANAGER->findImage("IDLE");
		_player.action->setFrameY(_player.direction);
	}
	_player.body.bottom = top;
	_player.body.top = _player.body.bottom - _player.height;
}

void Player::check_wall(LONG len, int direction)
{
	_dash_speed = 0;
	_second_action = false;
	_shadow_action = false;

	// ���鿡�� �浹�� (������)

	if (direction == 1)
	{
		_player.body.right = len;
		_player.body.left = _player.body.right - _player.width;

		if (_state == JUMP || _state == JUMP_DOWN)
		{
			_player.action = IMAGEMANAGER->findImage("WALL");
			_player.direction = 1;
			_player.action->setFrameY(0);
			_state = WALL;
			_gravity = 1.0f;
			_jump_power = 0.0f;
		}
	}

	// ���鿡�� �浹�� (������)
	else
	{
		_player.body.left = len;
		_player.body.right = _player.body.left + _player.width;

		if (_state == JUMP || _state == JUMP_DOWN)
		{
			_player.action = IMAGEMANAGER->findImage("WALL");
			_player.direction = 0;
			_player.action->setFrameY(1);
			_state = WALL;
			_gravity = 1.0f;
			_jump_power = 0.0f;
		}
	}
}

void Player::check_Fground(LONG top)
{

	_second_action = false;
	_player.on_flying_wall = true;

	_edge_top = top;

	if (_state != SPECIAL2 && _state != START)
	{
		_player.action = IMAGEMANAGER->findImage("IDLE");
		_player.action->setFrameY(_player.direction);
	}
	_player.body.bottom = top;
	_player.body.top = _player.body.bottom - _player.height;
}

void Player::collision_enemy()
{
	RECT temp;
	// �÷��̾� ��Ʈ�ڽ��� �̴Ͼ���� �浹������ ������ ����.
	for (int i = 0; i < _em->getVMinion().size(); i++)
	{
		RECT enemy = _em->getVMinion()[i]->getRect();
		// �÷��̾��� ���¿� ���� ������������ �˼��ֱ⶧����, ���º��� ������ ����
		if (IntersectRect(&temp, &_player.hit_box, &enemy))
		{
			// �̹� �¾Ҵٸ�, ���� ���͹����� ������ �ȹ޵��� ����ó��
			if (_em->getVMinion()[i]->get_ishit()) continue;
			
			// �⺻ ���� �� ����, �� ���� ������ 30���� ����
			if (_state == ATTACK || _state == ATTACK2 || _state == JUMP_ATTACK || _state == WALL_ATTACK)
			{
				_em->getVMinion()[i]->enemy_isHit();

				if (_em->getVMinion()[i]->Ndo_action())
				{
					_em->getVMinion()[i]->set_damage(20);
					_player.effect = IMAGEMANAGER->findImage("hard_effect");
				}
				else
				{
					_em->getVMinion()[i]->set_damage(30);
					_player.effect = IMAGEMANAGER->findImage("blade_effect");
				}

				if (_player.direction == 0)
				{
					_count_effect = _player.effect->getMaxFrameX();
					_player.effect->setFrameY(1);
					effect_x = enemy.left;
				}
				else
				{
					_count_effect = 0;
					_player.effect->setFrameY(0);
					effect_x = enemy.left;
				}

				effect_set = true;
				effect_y = enemy.top;
			}
			// Ư����� (�� , ����), �⺻ 3Ÿ ������ 50 ����
			if (_state == ATTACK3 || _state == SPECIAL1 || _state == SPECIAL2)
			{
				_em->getVMinion()[i]->enemy_isHit();
				if(_state == ATTACK3)
					_player.effect = IMAGEMANAGER->findImage("blade_effect");
				else
					_player.effect = IMAGEMANAGER->findImage("hard_effect");

				if (_em->getVMinion()[i]->Ndo_action())
				{
					_em->getVMinion()[i]->set_damage(35);
				}
				else
					_em->getVMinion()[i]->set_damage(50);

				if (_player.direction == 0)
				{
					_count_effect = _player.effect->getMaxFrameX();
					_player.effect->setFrameY(1);
					effect_x = enemy.right;
				}
				else
				{
					_count_effect = 0;
					_player.effect->setFrameY(0);
					effect_x = enemy.left;
				}

				effect_set = true;
				effect_y = enemy.top;
			}
		}

		for (int j = 0; j < _bullet->getVHyunMoo().size(); j++)
		{
			RECT bullet = _bullet->getVHyunMoo()[j].rc;
			// �÷��̾� �Ѿ˰� �浹�� ���̹� 30 ����
			if (IntersectRect(&temp, &bullet, &enemy))
			{
				if (_em->getVMinion()[i]->get_ishit()) continue;

				_em->getVMinion()[i]->enemy_isHit();

				if (_em->getVMinion()[i]->get_state() == enemy_state::HIDE)
				{
					_em->getVMinion()[i]->set_damage(10);
					_player.effect = IMAGEMANAGER->findImage("hard_effect");
				}
				else
				{
					_em->getVMinion()[i]->set_damage(20);
					_player.effect = IMAGEMANAGER->findImage("bullet_effect");
				}

				if (_player.direction == 0)
				{
					_count_effect = _player.effect->getMaxFrameX();
					_player.effect->setFrameY(1);
					effect_x = bullet.left - 20;
				}
				else
				{
					_count_effect = 0;
					_player.effect->setFrameY(0);
					effect_x = bullet.right - 10;
				}

				effect_set = true;
				effect_y = bullet.top - 25;
				_bullet->removeBullet(j);
			}
		}

		for (int j = 0; j < _Special_bullet->getVHyunMoo().size(); j++)
		{
			RECT bullet = _Special_bullet->getVHyunMoo()[j].rc;
			// �÷��̾� �Ѿ˰� �浹�� ���̹� 40 ����
			if (IntersectRect(&temp, &bullet, &enemy))
			{
				if (_em->getVMinion()[i]->get_ishit()) continue;

				_em->getVMinion()[i]->enemy_isHit();
				_em->getVMinion()[i]->set_damage(40);

				_count_effect = 0;

				_player.effect = IMAGEMANAGER->findImage("hard_effect");
				_player.effect->setFrameY(0);

				effect_set = true;
				effect_x = _em->getVMinion()[i]->getRect().left;
				effect_y = _em->getVMinion()[i]->getRect().top;
			}
		}
	}


}

void Player::get_gravity()
{
	_state = JUMP_DOWN;
	_player.on_ground = false;
	_jump_power = 0.0f;
	_gravity = 0.2f;
}

void Player::set_isHit()
{
	_state = HURT;
	_player.is_hit = true;
	_player.doing = true;
	_player.action = IMAGEMANAGER->findImage("HURT");

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

void Player::run_effect()
{
	temp_interval++;

	if (_player.direction == 0)
	{
		_player.effect->setFrameX(_count_effect);

		if (temp_interval % 3 == 0)
		{
			_count_effect--;
		}

		if (_count_effect <= 0 )
		{
			effect_set = false;
			temp_interval = 0;
		}
	}
	else
	{
		_player.effect->setFrameX(_count_effect);

		if (temp_interval % 3 == 0)
		{
			_count_effect++;
		}

		if (_count_effect > _player.effect->getMaxFrameX())
		{
			effect_set = false;
			temp_interval = 0;
		}
	}

}
