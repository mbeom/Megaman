#include "stdafx.h"
#include "Object.h"
#include "Player.h"
#include "enemyManager.h"

HRESULT Obj::init()
{
	IMAGEMANAGER->addImage("stage_01", "stage_001.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_02", "stage_002.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_03", "stage_003.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_04", "stage_004.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_05", "stage_005.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_06", "stage_006.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_07", "stage_007.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_08", "stage_008.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_09", "stage_009.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_10", "stage_010.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_11", "stage_011.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage_12", "stage_012.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);

	IMAGEMANAGER->addImage("stage1", "stage_01.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage2", "stage_02.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage3", "stage_03.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage4", "stage_04.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage5", "stage_05.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage6", "stage_06.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage7", "stage_07.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage8", "stage_08.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage9", "stage_09.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage10", "stage_10.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage11", "stage_11.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addImage("stage12", "stage_12.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);


	IMAGEMANAGER->addImage("practice", "practice.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);

	// 벽 초기화
	char temp[128];
	for (int i = 0; i < 9; i++)
	{
		sprintf_s(temp, "stage_0%d", i + 1);
		stage[i] = IMAGEMANAGER->findImage(temp);
	}
	for (int i = 9; i < 11; i++)
	{
		sprintf_s(temp, "stage_%d", i + 1);
		stage[i] = IMAGEMANAGER->findImage(temp);
	}

	x_left = 0;
	y = 0;
	current_stage = 0;
	is_pass = false;
	show_map = false;
	//stage[0] = IMAGEMANAGER->findImage("practice");

	return S_OK;
}

void Obj::release()
{

}

void Obj::render()
{
	char temp[128];
	for (int i = 0; i < 11; i++)
	{
		sprintf_s(temp, "stage%d", i + 1);
		IMAGEMANAGER->findImage(temp)->render(getMemDC(), 0 + WINSIZEX * i, 0);
	}
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		for (int i = 0; i < 11; i++)
		{
			stage[i]->render(getMemDC(), 0 + WINSIZEX * i, 0);
		}
	}

}

void Obj::update()
{
	pixel_collision();
	enemy_pixel_collision();

	if (_player_info->get_playerInfo().body.left > WINSIZEX * 10 + 100)
	{
		stage[10] = IMAGEMANAGER->findImage("stage_12");
	}
}

void Obj::pixel_collision()
{
	x_left = _player_info->get_playerInfo().body.left + _player_info->get_playerInfo().width / 2;
	x_right = _player_info->get_playerInfo().body.right;
	y = _player_info->get_playerInfo().body.bottom - _player_info->get_playerInfo().height / 2;
	on_ground = _player_info->on_ground();
	on_Fground = _player_info->get_playerInfo().on_flying_wall;
	
	COLORREF color_L, color_R;
	int r_L, g_L, b_L;
	int r_R, g_R, b_R;

	if (_player_info->get_direction() == 0)
	{
		if ((x_left - 50) < (current_stage * WINSIZEX) && !is_pass)
		{
			is_pass = true;
			current_stage -= 1;
		}
		if (x_right < (current_stage * WINSIZEX))
		{
			is_pass = false;
		}
	}
	else if (_player_info->get_direction() == 1)
	{
		if (x_right > WINSIZEX + (current_stage * WINSIZEX) && !is_pass)
		{
			is_pass = true;
			current_stage += 1;

			if((current_stage + 1) != 11)
			_enemy_info->set_bats((current_stage + 1) * WINSIZEX, 200);
		}
		if ((x_left - 50) > WINSIZEX + (current_stage * WINSIZEX))
		{
			is_pass = false;
		}
	}
	// 바닥과 충돌시
	for (int i = y - 50; i < y + 55; ++i)
	{
		int x = x_left - WINSIZEX * current_stage;
		// 캐릭터 왼쪽 끝 픽셀 색깔
		color_L = GetPixel(stage[current_stage]->getMemDC(), x - 50, i);
		r_L = GetRValue(color_L);
		g_L = GetGValue(color_L);
		b_L = GetBValue(color_L);

		// 캐릭터 오른쪽 끝 픽셀 색깔
		color_R = GetPixel(stage[current_stage]->getMemDC(), x + 50, i);
		r_R = GetRValue(color_R);
		g_R = GetGValue(color_R);
		b_R = GetBValue(color_R);

		int r_C, g_C, b_C;
		if (_player_info->get_direction() == 0)
		{
			r_C = r_R;
			g_C = g_R;
			b_C = b_R;
		}
		else
		{
			r_C = r_L;
			g_C = g_L;
			b_C = b_L;
		}

		if (r_C == 255 && g_C == 0 && b_C == 0)
		{
			if (!on_ground)
			{
				_player_info->check_ground(i);
				_player_info->set_onGround();
				break;
			}
			_player_info->set_top(i);
			break;
		}
		else
		{
			if (on_ground && !on_Fground)
			{
				if (i < y + 53) continue;
				if (r_R != 255 && g_R != 0 && b_R != 0 && r_L != 255 && g_L != 0 && b_L != 0)
				{
					_player_info->get_gravity();
				}
			}

			if (_player_info->get_state() == JUMP || _player_info->get_state() == JUMP_DOWN
				|| _player_info->get_state() == WALL || _player_info->get_state() == WALL_ATTACK || _player_info->get_state() == SPECIAL2)
			{
				if (r_L == 255 && g_L == 0 && b_L == 0 || r_R == 255 && g_R == 0 && b_R == 0)
				{
					_player_info->set_top(i);
					if (!on_ground)
					{
						_player_info->check_ground(i);
						_player_info->set_onGround();
						break;
					}
				}
			}
		}
	}

		// 측면의 벽과 충돌시
	for (int i = x_left - 50; i < x_left + 50; i++)
	{
		int x = i - WINSIZEX * current_stage;
		COLORREF color = GetPixel(stage[current_stage]->getMemDC(), x, y);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 255 && b == 0)
		{
			if (_player_info->get_playerInfo().direction == 1 && i > x_left - 50)
			{
				_player_info->check_wall(i, 1);
				break;
			}
			else
			{
				_player_info->check_wall(i+1, 0);
			}
		}
	}

	// 부유하는 땅과 충돌시
	for (int i = y; i < y + 51; ++i)
	{
		// 캐릭터 왼쪽 끝 픽셀 색깔
		COLORREF color_L = GetPixel(stage[current_stage]->getMemDC(), x_left - 50, i);
		int r_L = GetRValue(color_L);
		int g_L = GetGValue(color_L);
		int b_L = GetBValue(color_L);

		// 캐릭터 오른쪽 끝 픽셀 색깔
		COLORREF color_R = GetPixel(stage[current_stage]->getMemDC(), x_left + 50, i);
		int r_R = GetRValue(color_R);
		int g_R = GetGValue(color_R);
		int b_R = GetBValue(color_R);

		int r_C, g_C, b_C;
		if (_player_info->get_direction() == 0)
		{
			r_C = r_R;
			g_C = g_R;
			b_C = b_R;
		}
		else
		{
			r_C = r_L;
			g_C = g_L;
			b_C = b_L;
		}

		if (r_C == 0 && g_C == 0 && b_C == 255)
		{
			if (!on_Fground)
			{
				y = i - _player_info->get_playerInfo().height;
				_player_info->check_Fground(i);
				_player_info->set_onGround();
				break;
			}
		}
		else
		{
			if (on_Fground && on_ground)
			{
				if (i < y + 50) continue;
				if (_player_info->get_direction() == 1 && b_R != 255)
				{
					_player_info->get_gravity();
					_player_info->setoff_FonGround();
				}
				if (_player_info->get_direction() == 0 && b_L != 255)
				{
					_player_info->get_gravity();
					_player_info->setoff_FonGround();
				}
			}

			if (_player_info->get_state() == JUMP || _player_info->get_state() == JUMP_DOWN
				|| _player_info->get_state() == WALL || _player_info->get_state() == WALL_ATTACK || _player_info->get_state() == SPECIAL2)
			{
				if (r_L == 0 && g_L == 0 && b_L == 255 || r_R == 0 && g_R == 0 && b_R == 255)
				{
					_player_info->set_top(i);
					if (!on_Fground)
					{
						y = i - _player_info->get_playerInfo().height;
						_player_info->check_Fground(i);
						_player_info->set_onGround();
						break;
					}
				}
			}
		}
	}
}

void Obj::enemy_pixel_collision()
{
	vector<enemy*> _enemy_pool = _enemy_info->getVMinion();
	vector<enemy*>::iterator _pool_iter;

	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{
		if ((*_pool_iter)->get_type() == FLYING) continue;
		if ((*_pool_iter)->get_state() == enemy_state::DEAD) continue;

		COLORREF color_L, color_R;
		int r_L, g_L, b_L;
		int r_R, g_R, b_R;

		// 바닥과 충돌시
		for (int i = (*_pool_iter)->getRect().bottom - 38; i < (*_pool_iter)->getRect().bottom + 3; ++i)
		{
			int x = ((*_pool_iter)->getRect().left + 38) - WINSIZEX * (*_pool_iter)->get_stage();
			// 캐릭터 왼쪽 끝 픽셀 색깔
			color_L = GetPixel(stage[(*_pool_iter)->get_stage()]->getMemDC(), x - 38, i);
			r_L = GetRValue(color_L);
			g_L = GetGValue(color_L);
			b_L = GetBValue(color_L);

			// 캐릭터 오른쪽 끝 픽셀 색깔
			color_R = GetPixel(stage[(*_pool_iter)->get_stage()]->getMemDC(), x + 38, i);
			r_R = GetRValue(color_R);
			g_R = GetGValue(color_R);
			b_R = GetBValue(color_R);

			int r_C, g_C, b_C;
			if ((*_pool_iter)->get_direction() == 0)
			{
				r_C = r_R;
				g_C = g_R;
				b_C = b_R;
			}
			else
			{
				r_C = r_L;
				g_C = g_L;
				b_C = b_L;
			}

			if (r_C == 255 && g_C == 0 && b_C == 0)
			{
				if (!(*_pool_iter)->on_ground())
				{
					(*_pool_iter)->enemy_checkGround(i);
					break;
				}
				(*_pool_iter)->set_top(i);
				break;
			}
			else
			{
				if ((*_pool_iter)->on_ground() && !(*_pool_iter)->on_Fground())
				{
					if (i < (*_pool_iter)->getRect().top + 76) continue;
					if (r_R != 255 && g_R != 0 && b_R != 0 && r_L != 255 && g_L != 0 && b_L != 0)
					{
						(*_pool_iter)->enemy_getGravity();
					}
				}

				if ((*_pool_iter)->get_state() == enemy_state::ATTACK)
				{
					if (r_L == 255 && g_L == 0 && b_L == 0 || r_R == 255 && g_R == 0 && b_R == 0)
					{
						(*_pool_iter)->set_top(i);
						if (!(*_pool_iter)->on_ground())
						{
							(*_pool_iter)->enemy_checkGround(i);
							break;
						}
					}
				}
			}
		}

		// 측면의 벽과 충돌시
		for (int i = (*_pool_iter)->getRect().left; i < (*_pool_iter)->getRect().right; i++)
		{
			int x = i - WINSIZEX * (*_pool_iter)->get_stage();
			COLORREF color = GetPixel(stage[(*_pool_iter)->get_stage()]->getMemDC(), x, (*_pool_iter)->getRect().bottom - 10);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 0 && g == 255 && b == 0)
			{
				if (i > (*_pool_iter)->getRect().left)
				{
					(*_pool_iter)->enemy_checkWall(i, 1);
					break;
				}
				else
				{
					(*_pool_iter)->enemy_checkWall(i + 1, 0);
				}
			}
		}
	}
	/*
	// 부유하는 땅과 충돌시
	for (int i = y; i < y + 51; ++i)
	{
		// 캐릭터 왼쪽 끝 픽셀 색깔
		COLORREF color_L = GetPixel(stage[current_stage]->getMemDC(), x_left - 50, i);
		int r_L = GetRValue(color_L);
		int g_L = GetGValue(color_L);
		int b_L = GetBValue(color_L);

		// 캐릭터 오른쪽 끝 픽셀 색깔
		COLORREF color_R = GetPixel(stage[current_stage]->getMemDC(), x_left + 50, i);
		int r_R = GetRValue(color_R);
		int g_R = GetGValue(color_R);
		int b_R = GetBValue(color_R);

		int r_C, g_C, b_C;

		if ((*_pool_iter)->get_direction() == 0)
		{
			r_C = r_R;
			g_C = g_R;
			b_C = b_R;
		}
		else
		{
			r_C = r_L;
			g_C = g_L;
			b_C = b_L;
		}

		if (r_C == 0 && g_C == 0 && b_C == 255)
		{
			if (!on_Fground)
			{
				y = i - (*_pool_iter)->get_height();
				(*_pool_iter)->enemy_checkGround(i);
				(*_pool_iter)->setOn_Fground();
				break;
			}
		}
		else
		{
			if (on_Fground && on_ground)
			{
				if (i < y + 50) continue;
				if ((*_pool_iter)->get_direction() == 1 && b_R != 255)
				{
					(*_pool_iter)->enemy_getGravity();
					(*_pool_iter)->setOff_Fground();
				}
				if ((*_pool_iter)->get_direction() == 0 && b_L != 255)
				{
					(*_pool_iter)->enemy_getGravity();
					(*_pool_iter)->setOff_Fground();
				}
			}

			if ((*_pool_iter)->get_state() == enemy_state::ATTACK)
			{
				if (r_L == 0 && g_L == 0 && b_L == 255 || r_R == 0 && g_R == 0 && b_R == 255)
				{
					(*_pool_iter)->set_top(i);
					if (!on_Fground)
					{
						y = i - (*_pool_iter)->get_height();
						(*_pool_iter)->enemy_checkGround(i);
						(*_pool_iter)->setOn_Fground();
						break;
					}
				}
			}
		}
	}
	*/
}
