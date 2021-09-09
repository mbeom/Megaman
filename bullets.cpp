#include "stdafx.h"
#include "bullets.h"

//============================
// ## 발사할때 생성한다 ##
//============================

HRESULT hyunMoo::init(int bulletMax, float range)
{
	_bulletMax = bulletMax;
	_range = range;
	_direction = 0;
	_range_plus = 0;

	return S_OK;
}

void hyunMoo::release()
{
}

void hyunMoo::update(const char* name)
{
	if (name == "player")
		move();
	else
	{
		bumerang_move();
	}
}

void hyunMoo::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->count++;
		// 보스는 왼쪽, 오른쪽 구분이 없기떄문에 상시 0
		if (type == 1)
		{
			_viBullet->bulletImage->frameRender(getMemDC(),
				_viBullet->rc.left,
				_viBullet->rc.top,
				_viBullet->bulletImage->getFrameX(), 0);
		}
		// 다른 프레임은 방향이 있기때문에 총알의 방향으로 
		else
		_viBullet->bulletImage->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->bulletImage->getFrameX(), _viBullet->direction);

		// 보스총알이 아닐시
		if (_viBullet->count % 5 == 0 && type == 0)
		{
			if (_viBullet->direction == 0)
			{
				_viBullet->bulletImage->setFrameX(_viBullet->bulletImage->getFrameX() + 1);

				//최대 프레임보다 커지면
				if (_viBullet->bulletImage->getFrameX() >= _viBullet->bulletImage->getMaxFrameX())
				{
					_viBullet->bulletImage->setFrameX(6);
				}
			}
			else
			{
				_viBullet->bulletImage->setFrameX(_viBullet->bulletImage->getFrameX() - 1);

				//최대 프레임보다 커지면
				if (_viBullet->bulletImage->getFrameX() <= 0)
				{
					_viBullet->bulletImage->setFrameX(_viBullet->bulletImage->getMaxFrameX() - 6);
				}

			}
			_viBullet->count = 0;
		}
		// 보스 총알이면
		if (_viBullet->count % 5 == 0 && type == 1)
		{
			_viBullet->bulletImage->setFrameX(_viBullet->bulletImage->getFrameX() + 1);

			//최대 프레임보다 커지면
			if (_viBullet->bulletImage->getFrameX() >= _viBullet->bulletImage->getMaxFrameX())
			{
				_viBullet->bulletImage->setFrameX(0);
			}

			_viBullet->count = 0;
		}
		//Rectangle(getMemDC(),_viBullet->rc);
	}
}

void hyunMoo::fire(float x, float y, int direction, const char* name)
{
	//최대갯수보다 더 생성되려고 하면 하지않는다
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;

	if (name == "boss_bumerang.bmp")
	{
		bullet.bulletImage->init(name, 0, 0, 783, 100, 4, 1,
			true, RGB(168, 230, 29), false);
		bullet.speed = 10.0f;
		type = 1;
	}
	else
	{
		bullet.bulletImage->init(name, 0, 0, 1112, 120, 9, 2,
			true, RGB(147, 187, 236), false);
		bullet.speed = 16.0f;
		type = 0;
	}

	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	bullet.direction = direction;
	if (bullet.direction == 0)
	{
		bullet.bulletImage->setFrameX(0);
	}
	else
		bullet.bulletImage->setFrameX(bullet.bulletImage->getMaxFrameX());

	_vBullet.push_back(bullet);
}

void hyunMoo::flasher_fire(float x, float y)
{
	//최대갯수보다 더 생성되려고 하면 하지않는다
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->init("flasher.bmp", 0, 0, 4920, 400, 9, 1,
		true, RGB(147, 187, 236), false);
	bullet.speed = 10.0f;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	_vBullet.push_back(bullet);
}

void hyunMoo::boss_UltimateFire(float x, float y)
{
	//최대갯수보다 더 생성되려고 하면 하지않는다
	if (_bulletMax < _vBullet.size()) return;
	type = 1;
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->init("boss_sp.bmp", 0, 0, 1094, 700, 6, 1,
		true, RGB(168, 230, 29), false);
	bullet.speed = 10.0f;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	_vBullet.push_back(bullet);
}

void hyunMoo::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (_viBullet->direction == 0)
		{
			_viBullet->x -= _viBullet->speed;
		}
		else
			_viBullet->x += _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		//사거리보다 더 멀리 나가면(?)
		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;

	}
}

void hyunMoo::flasher_move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (_viBullet->direction == 0)
		{
			_viBullet->y -= _viBullet->speed;
		}
		else
			_viBullet->y += _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		//사거리보다 더 멀리 나가면(?)
		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;

	}
}

void hyunMoo::bumerang_move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (_viBullet->direction == 0)
		{
			_viBullet->x -= _viBullet->speed;
		}
		else
			_viBullet->x += _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		//사거리보다 더 멀리 나가면(?)
		if (_range + _range_plus < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			if (_viBullet->direction == 0)
			{
				_viBullet->direction = 1;
				_range_plus += 50;
			}
			else
			{
				_viBullet->direction = 0;
				_range_plus += 50;
			}

		}
		else ++_viBullet;

	}
}


void hyunMoo::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
	_range_plus = 0;
}

HRESULT bullet::init(const char* imageName, int bulletMax, float range)
{
	_imageName = imageName;
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void bullet::release()
{
}

void bullet::update()
{
	move();
}

void bullet::render()
{

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->bulletImage->render(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	}

}

void bullet::fire(float x, float y, float angle, float speed)
{
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.radius = bullet.bulletImage->getWidth() / 2;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = angle;
	bullet.arc_angle = PI - angle;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getWidth(),
		bullet.bulletImage->getHeight());

	_vBullet.push_back(bullet);

}

void bullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (_viBullet->isCrash == false)
		{
			_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
			_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		}
		else
		{
			_viBullet->x += cosf(_viBullet->arc_angle) * _viBullet->speed;
			_viBullet->y += -sinf(_viBullet->arc_angle) * _viBullet->speed;
		}

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void bullet::removeBullet(int arrNum)
{
	//벡터의 값을 지울떈  erase()
	_vBullet.erase(_vBullet.begin() + arrNum);
}

void bullet::change_angle(int arrNum)
{
	_vBullet[arrNum].isCrash = true;
}
