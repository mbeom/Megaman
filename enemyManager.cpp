#include "stdafx.h"
#include "enemyManager.h"
#include "Player.h"

HRESULT enemyManager::init()
{
	IMAGEMANAGER->addImage("enemy_bullet", "enemy_bullet.bmp", 21, 21, true, RGB(255,0,255), false);
	IMAGEMANAGER->addFrameImage("bats_bullet", "bats_bullet.bmp", 60, 60, 2, 2 , true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("boss_bumerang", "boss_bumerang.bmp", 470, 60, 4, 2, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("boss_sp", "boss_sp.bmp", 1094, 700, 6, 1, true, RGB(168, 230, 29), false);
	IMAGEMANAGER->addFrameImage("explosion", "effect_explo.bmp", 720, 240, 6, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("explosion2", "explosion2.bmp", 1300, 200, 26, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("explosion3", "explosion3.bmp", 2684, 400, 26, 2, true, RGB(255, 0, 255), false);

	_bullet = new bullet;
	_bullet->init("enemy_bullet", 5, 500);

	_boss_bumerang = new hyunMoo;
	_boss_bumerang->init(0, 800);

	_boss_ult = new hyunMoo;
	_boss_ult->init(4, 1600);

	enemy_effect = new image;

	boss_HPBar = new HP_bar;
	boss_HPBar->init(13050, 0, 100, 200, "boss");
	boss_HPBar->setGauge_V(1000, 1000);

	is_run = false;
	_count = 0;

	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{
		(*_pool_iter)->update();
		boss_HPBar->setGauge_V((*_pool_iter)->get_hp(), 1000);
		if (!(*_pool_iter)->check_boss()) continue;
		if ((*_pool_iter)->waiting_boss())
		{
			RECT rc = (*_pool_iter)->getRect();
			
			if ((*_pool_iter)->get_direction() == 0)
			{
				_boss_bumerang->fire(rc.left - 90, (rc.top + rc.bottom) / 2, (*_pool_iter)->get_direction(), "boss_bumerang.bmp");
			}
			else
				_boss_bumerang->fire(rc.right + 90, (rc.top + rc.bottom) / 2, (*_pool_iter)->get_direction(), "boss_bumerang.bmp");

			(*_pool_iter)->Nwaiting_boss();
		}
		// 보스 궁극기 bool이 true 이며 상태가 ULT 이면 특수총알 생성
		if ((*_pool_iter)->Ndo_action() && (*_pool_iter)->get_state() == enemy_state::IDLE)
		{
			RECT rc = (*_pool_iter)->getRect();

			if ((*_pool_iter)->get_direction() == 0)
			{
				for (int i = 0; i < 5; i++)
				{
					_boss_ult->boss_UltimateFire(rc.left + 150 - 150 * i, rc.bottom - 350);
				}
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					_boss_ult->boss_UltimateFire(rc.right - 150 + 150 * i, rc.bottom - 350);
				}
			}

		}
		if ((*_pool_iter)->get_state() != enemy_state::IDLE)
		{
			for (int j = 0; j < _boss_ult->getVHyunMoo().size(); j++)
			{
				_boss_ult->removeBullet(j);
			}
		}
	}

	find_player();
	minionBulletFire();
	_bullet->update();
	_boss_bumerang->update("boss");

	collision_player();
	remove_unit();
//	collision();

}

void enemyManager::render()
{
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{
		(*_pool_iter)->render();
	}
	_bullet->render();
	_boss_bumerang->render();
	_boss_ult->render();

	if (is_run)
	{
		run_effect();
		enemy_effect->frameRender(getMemDC(), effect_x, effect_y);
	}
	boss_HPBar->render();
}

void enemyManager::set_bats(int pos_x, int pos_y)
{
	enemy* bats;
	bats = new Bats;
	bats->init("bats", PointMake(pos_x, pos_y));

	_enemy_pool.push_back(bats);
}

void enemyManager::set_mush(int pos_x, int pos_y)
{
	enemy* _mush;
	_mush = new mush;
	_mush->init("mush_idle", PointMake(pos_x, pos_y));

	_enemy_pool.push_back(_mush);
}

void enemyManager::set_boss(int pos_x, int pos_y)
{
	enemy* _boss;
	_boss = new boss;
	_boss->init("boss_start", PointMake(pos_x, pos_y));

	_enemy_pool.push_back(_boss);
}

void enemyManager::minionBulletFire()
{
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{	// 보스는 잡몹처럼 총알발사하지않음. 그러므로 보스일시 패스
		if ((*_pool_iter)->check_boss()) continue;

		if ((*_pool_iter)->get_type() == FLYING)
		{
			if ((*_pool_iter)->bullet_Fire())
			{
				RECT rc = (*_pool_iter)->getRect();

				_bullet->fire((rc.left + rc.right) / 2, rc.bottom + 5,
					getAngle((rc.left + rc.right) / 2, rc.bottom,
						_player->get_playerInfo().body.left + _player->get_playerInfo().width / 2,
						_player->get_playerInfo().body.top + _player->get_playerInfo().height / 2),
					7.0f);
			}
		}

		if ((*_pool_iter)->get_type() == GROUND && (*_pool_iter)->get_state() == enemy_state::ATTACK)
		{
			if ((*_pool_iter)->bullet_Fire())
			{
				RECT rc = (*_pool_iter)->getRect();

				if ((*_pool_iter)->get_direction() == 0)
				{
					_bullet->fire(rc.left - 5, (rc.top + rc.bottom) / 2 + 5,
						getAngle((rc.left + rc.right) / 2, rc.bottom,
							_player->get_playerInfo().body.left + _player->get_playerInfo().width / 2,
							_player->get_playerInfo().body.top + _player->get_playerInfo().height / 2),
						7.0f);
				}
				else
				{
					_bullet->fire(rc.right + 5, (rc.top + rc.bottom) / 2 + 5,
						getAngle((rc.left + rc.right) / 2, rc.bottom,
							_player->get_playerInfo().body.left + _player->get_playerInfo().width / 2,
							_player->get_playerInfo().body.top + _player->get_playerInfo().height / 2),
						7.0f);
				}

			}
		}

	}
}

void enemyManager::find_player()
{
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{	// 보스이면 평범한 미니언들의 탐색 및 탐지 기능을 하지않음.
		if ((*_pool_iter)->check_boss()) continue;

		int x = (*_pool_iter)->getRect().left + ((*_pool_iter)->getRect().right - (*_pool_iter)->getRect().left) / 2;
		int y = (*_pool_iter)->getRect().top + (*_pool_iter)->get_height()/2;

		int x2 = _player->get_playerInfo().body.left + (_player->get_playerInfo().body.right - _player->get_playerInfo().body.left) / 2;
		int y2 = _player->get_playerInfo().body.top + (_player->get_playerInfo().body.bottom - _player->get_playerInfo().body.top) / 2;

		if (getDistance(x, y, x2, y2) < 300)
		{
			// 플레이어 발견 플래그 온
			(*_pool_iter)->find_player();
			
			if ((*_pool_iter)->get_state() == enemy_state::LEFT_RUN || (*_pool_iter)->get_state() == enemy_state::RIGHT_RUN)
			{
				(*_pool_iter)->setOff_doing();
			}

			if (x > x2)
			{
				(*_pool_iter)->set_direction(0);
			}
			else
				(*_pool_iter)->set_direction(1);
		}
		else
		{
			(*_pool_iter)->Nfind_player();
			(*_pool_iter)->setOff_doing();
		}

		for (int j = 0; j < _player->get_bullet()->getVHyunMoo().size(); j++)
		{
			int bullet_x = _player->get_bullet()->getVHyunMoo()[j].rc.left;
			int bullet_y = _player->get_bullet()->getVHyunMoo()[j].rc.top;

			if (getDistance(x, y, bullet_x, bullet_y) > 300 && !(*_pool_iter)->get_isalert())
			{
				(*_pool_iter)->alert_player();

				if (x > bullet_x)
				{
					(*_pool_iter)->set_direction(0);
				}
				else
					(*_pool_iter)->set_direction(1);
			}
		}
	}
}

void enemyManager::collision_player()
{
	for (int i = 0; i < _bullet->getVBullet().size(); i++)
	{
		RECT temp;
		RECT player_rect = (_player->get_playerInfo().body);
		RECT hit_box = (_player->get_playerInfo().hit_box);

		// 플레이어에게 에너미 총알이 충돌시 플레이어가 데미지를입음.
		if (IntersectRect(&temp, &_bullet->getVBullet()[i].rc, &player_rect))
		{
			if (_player->is_hit()) continue;

			if (!_player->is_hit() && _player->get_state() != SPECIAL3)
			{
				_player->get_damage(10.0f);
				_player->set_isHit();
				_bullet->removeBullet(i);
			}
			break;
		}
		// 플레이어 칼과 에너미 총알 충돌시, 총알은 반사각에 의하여 튕겨져나감.
		if (IntersectRect(&temp, &_bullet->getVBullet()[i].rc, &hit_box))
		{
			if (_player->get_state() == SPECIAL1 || _player->get_state() == SPECIAL2 || _player->get_state() == SPECIAL3)
			{
				_bullet->removeBullet(i);
			}
			else
				_bullet->change_angle(i);
			break;
		}
	}

	for (int i = 0; i < _player->get_bullet()->getVHyunMoo().size(); i++)
	{
		RECT temp;
		RECT player_bullet = _player->get_bullet()->getVHyunMoo()[i].rc;
		for (int j = 0; j < getVMinion().size(); j++)
		{
			RECT boss_hitbox = getVMinion()[j]->get_hitbox();
			// 플레이어 총알과 보스가 디펜스 시 총알 삭제
			if (IntersectRect(&temp, &player_bullet, &boss_hitbox) && getVMinion()[j]->get_state() == enemy_state::DEFENSE)
			{
				enemy_effect = IMAGEMANAGER->findImage("hard_effect");
				if (_player->get_direction() == 0)
				{
					effect_dir = 0;
					effect_frameX = enemy_effect->getMaxFrameX();
					enemy_effect->setFrameY(1);
					effect_x = boss_hitbox.right + 35;
				}
				else
				{
					effect_dir = 1;
					effect_frameX = 0;
					enemy_effect->setFrameY(0);
					effect_x = boss_hitbox.left - 35;
				}

				is_run = true;
				effect_y = RND->getFromIntTo(boss_hitbox.top - 25, boss_hitbox.bottom - 50);
				_player->get_bullet()->removeBullet(i);

				break;
			}
		}
		// 보스 부메랑과 플레이어 총알 충돌시 총알 삭제
		for (int j = 0; j < _boss_bumerang->getVHyunMoo().size(); j++)
		{
			RECT boss_bumerang = _boss_bumerang->getVHyunMoo()[j].rc;
			if (IntersectRect(&temp, &player_bullet, &boss_bumerang))
			{
				enemy_effect = IMAGEMANAGER->findImage("hard_effect");
				if (_player->get_direction() == 0)
				{
					effect_frameX = enemy_effect->getMaxFrameX();
					enemy_effect->setFrameY(1);
					effect_x = boss_bumerang.right + 35;
				}
				else
				{
					effect_frameX = 0;
					enemy_effect->setFrameY(0);
					effect_x = boss_bumerang.left - 35;
				}

				is_run = true;
				effect_y = RND->getFromIntTo(boss_bumerang.top - 25, boss_bumerang.bottom - 50);

				_player->get_bullet()->removeBullet(i);
				break;
			}
		}
		//RECT boss_hit_box = getVMinion()[i]->get_hitbox();
	}
	for (int i = 0; i < getVMinion().size(); i++)
	{
		RECT temp;
		RECT player_rect = (_player->get_playerInfo().body);
		RECT enemy_rect = getVMinion()[i]->getRect();
		// 플레이어와 에너미 몸체와 충돌시 데미지
		if (IntersectRect(&temp, &enemy_rect, &player_rect))
		{
			if (_player->is_hit()) continue;

			_player->get_damage(10.0f);
			_player->set_isHit();
		}

		// 보스아니면 부메랑충돌은 생략.
		if (!getVMinion()[i]->check_boss()) continue;

		// 보스와 부메랑 충돌시
		for (int j = 0; j < _boss_bumerang->getVHyunMoo().size(); j++)
		{
			if (IntersectRect(&temp, &enemy_rect, &_boss_bumerang->getVHyunMoo()[j].rc))
			{
				_boss_bumerang->removeBullet(j);
				getVMinion()[i]->do_action();
			}
			break;
		}
		// 보스의 부메랑과 플레이어 몸체와 충돌시
		for (int j = 0; j < _boss_bumerang->getVHyunMoo().size(); j++)
		{
			if (IntersectRect(&temp, &player_rect, &_boss_bumerang->getVHyunMoo()[j].rc))
			{
				if (_player->is_hit()) continue;

				_player->get_damage(10.0f);
				_player->set_isHit();
			}
		}
		// 보스의 궁극기 기둥과 플레이어 몸체와 충돌시
		for (int j = 0; j < _boss_ult->getVHyunMoo().size(); j++)
		{
			RECT ult_rect = _boss_ult->getVHyunMoo()[j].rc;
			if (IntersectRect(&temp, &player_rect, &ult_rect))
			{
				if (_player->is_hit()) continue;

				_player->get_damage(10.0f);
				_player->set_isHit();
			}
		}
	}
}

void enemyManager::remove_unit()
{
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end();)
	{
		if ((*_pool_iter)->getRect().bottom < 0)
		{
			_pool_iter = _enemy_pool.erase(_pool_iter);
		}
		else if ((*_pool_iter)->getRect().bottom > WINSIZEY)
		{
			_pool_iter = _enemy_pool.erase(_pool_iter);
		}
		else if ((*_pool_iter)->get_endsignal())
		{
			enemy_effect = IMAGEMANAGER->findImage("explosion3");
			if (_player->get_direction() == 0)
			{
				effect_dir = 0;
				effect_frameX = enemy_effect->getMaxFrameX();
				enemy_effect->setFrameY(1);
				effect_x = (*_pool_iter)->getRect().right - 10;
			}
			else
			{
				effect_dir = 1;
				effect_frameX = 0;
				enemy_effect->setFrameY(0);
				effect_x = (*_pool_iter)->getRect().left + 10;
			}

			is_run = true;
			effect_y = (*_pool_iter)->getRect().top - 100;

			_pool_iter = _enemy_pool.erase(_pool_iter);
		}
		else if ((*_pool_iter)->get_hp() <= 0)
		{
			enemy_effect = IMAGEMANAGER->findImage("explosion2");
			if (_player->get_direction() == 0)
			{
				effect_dir = 0;
				effect_frameX = enemy_effect->getMaxFrameX();
				enemy_effect->setFrameY(1);
				effect_x = (*_pool_iter)->getRect().right - 10;
			}
			else
			{
				effect_dir = 1;
				effect_frameX = 0;
				enemy_effect->setFrameY(0);
				effect_x = (*_pool_iter)->getRect().left + 10;
			}

			is_run = true;
			effect_y = RND->getFromIntTo((*_pool_iter)->getRect().top - 50, (*_pool_iter)->getRect().bottom - 60);

			if(!(*_pool_iter)->check_boss())
				_pool_iter = _enemy_pool.erase(_pool_iter);
			else
				++_pool_iter;
		}
		else
		{
			++_pool_iter;
		}

	}
}

void enemyManager::run_effect()
{
	_count++;

	if (effect_dir == 0)
	{
		enemy_effect->setFrameX(effect_frameX);

		if (_count % 3 == 0)
		{
			effect_frameX--;
		}

		if (effect_frameX <= 0)
		{
			is_run = false;
			_count = 0;
		}
	}
	else
	{
		enemy_effect->setFrameX(effect_frameX);

		if (_count % 3 == 0)
		{
			effect_frameX++;
		}

		if (effect_frameX > enemy_effect->getMaxFrameX())
		{
			is_run = false;
			_count = 0;
		}
	}
}

POINT enemyManager::PointMake(LONG x, LONG y)
{
	POINT temp;

	temp.x = x;
	temp.y = y;

	return temp;
}
