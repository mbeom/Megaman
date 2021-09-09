#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화는 여기다 하세요 제발
HRESULT playGround::init()
{
	gameNode::init(true);

	_player = new Player;
	_obj = new Obj;
	_enemy = new enemyManager;

	_player->init();
	_obj->init();
	_enemy->init();

	_obj->Link_PlayerInfo(_player);
	_obj->Link_EnemyInfo(_enemy);
	_player->Link_ObjectClass(_obj);
	_player->Link_EnemyClass(_enemy);
	_enemy->Link_PlayerClass(_player);

	CAMERAMANAGER->Link_PlayerClass(_player);

	IMAGEMANAGER->addImage("intro", "intro.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);
	_background = IMAGEMANAGER->findImage("intro");

	_enemy->set_bats(1855, 600);
	_enemy->set_bats(2547, 600);
	_enemy->set_bats(3527, 281);
	_enemy->set_mush(793, 536);
	_enemy->set_mush(2672, 505);
	_enemy->set_mush(4879, 313);
	_enemy->set_mush(6132, 600);
	_enemy->set_mush(6749, 600);
	_enemy->set_mush(8384, 600);

	_intro = true;
	return S_OK;
}

//메모리 해제는 여기다 하세요 제발
void playGround::release()
{
	gameNode::release();

	delete _player;
	delete _obj;
	delete _enemy;
}

//여기에다 연산하세요 제에발
void playGround::update()
{
	gameNode::update();

	if (_intro)
	{
		if(KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			_intro = false;
		}
	}
	else
	{
		_player->update();
		_enemy->update();
		_obj->update();

		if (_player->get_playerInfo().body.left > WINSIZEX * 10 + 100 && _boss_num == 0)
		{
			_start = true;
			_boss_num++;
		}

		if (_start)
		{
			_start = false;
			_enemy->set_boss(13000, 200);
		}

		if (_player->end_player())
		{
			restart_count++;
		}

		if (restart_count > 300)
		{
			restart_count = 0;
			_boss_num = 0;
			init();
			_intro = false;
		}

		CAMERAMANAGER->updateScreen(_player->get_playerInfo().body.left, _player->get_playerInfo().body.top);
	}

}

//여기에다 그려라 좀! 쫌!
void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);

	// 위에 건들지마라
	//================제발 이 사이에 좀 그립시다==========================
	if (_intro)
	{
		_background->render(getMemDC(), 0, 0);
	}
	else
	{
		_obj->render();
		_enemy->render();
		_player->render();
	}

	/*
	Rectangle(backDC, _flying_wall[0].rc);
	Rectangle(backDC, _player.body);
	// (_player.body.left - _camera.x), _player.body.top - _camera.y);
	*/

	/*
	sprintf_s(temp, " 플레이어 위치: %d, %d ", _player.body.bottom, _ground[stage].rc.bottom);
	TextOut(backDC, _player.body.left, _player.body.top - 200, temp, strlen(temp));
	*/

	//==================================================
	//여기도 건들지마라

	CAMERAMANAGER->render(getHDC(), 0, 0, getMemDC());
}

