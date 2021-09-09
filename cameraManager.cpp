#include "stdafx.h"
#include "cameraManager.h"
#include "Player.h"

cameraManager::cameraManager()
{
}

cameraManager::~cameraManager()
{
}

HRESULT cameraManager::init(int width, int height)
{
	//backDC�� frontDCũ�� ����
	backDC_sizeX = width;
	backDC_sizeY = height;
	camera_x = WINSIZEX;
	camera_y = WINSIZEY;

	//ȭ�� Rect ����
	_window = RectMake(0, 0, WINSIZEX, WINSIZEY);

	return S_OK;
}

void cameraManager::release()
{
}

void cameraManager::updateScreen(float standardX, float standardY, float ratioX, float ratioY)
{
	//�÷��̾� �������� ȭ��(_screen)����
	//playground.cpp�� update()���� ��� �������ָ� ��
	//�μ��� ������ �Ǵ� ��ü�� x, y��ǥ�� ����(����� player�� x, y)

	//X�� (��, ��)
	if (standardX - camera_x * ratioX < 0)
	{
		standardX = 0;
	}
	else if (standardX + camera_x * (1 - ratioX) > backDC_sizeX)
	{
		standardX = backDC_sizeX - camera_x;
	}
	else
		standardX -= camera_x * ratioX;

	if (standardX - camera_x * ratioX > WINSIZEX*9)
	{
		standardX = WINSIZEX * 10;
	}

	//Y�� (��, ��)

	if (standardY - camera_y * ratioY < 0)
	{
		standardY = 0;
	}
	else if (standardY + camera_y * (1 - ratioY) > backDC_sizeY)
	{
		standardY = backDC_sizeY - camera_y;
	}
	else
		standardY -= camera_y * ratioY;

	//����
	_window = RectMake(standardX, standardY, camera_x, camera_y);
	_player->get_hpPointer()->update(_window.left, _window.top + 100);
}

void cameraManager::render(HDC frontDC, int destX, int destY, HDC backDC, bool map, bool mini)
{
	/********* �μ� ���� *********/ /*
	HDC frontDC: ���縦 ���� DC (������ �츮�� ���� ȭ��)
	int destX: �츮�� ���� ȭ�鿡�� ������� x��ǥ
	int destY: �츮�� ���� ȭ�鿡�� ������� y��ǥ
	HDC backDC: ������ �� DC (������ �츮�� �׸��� ȭ��)
	bool map: true�� ��ü�� ���, false�� �� ���
	bool mini: true�� �̴ϸ� ���, false�� �� ���
	*/

	if (map) {
		StretchBlt(
			backDC,
			_window.left,
			_window.top,
			camera_x,
			camera_y,
			backDC,
			0,
			0,
			backDC_sizeX,
			backDC_sizeY,
			SRCCOPY);
	}
	//�̴ϸ� ������ true�� ��ũ���� ������ܿ� �̴ϸ� ���
	else if (mini) {
		StretchBlt(
			backDC,
			_window.right - camera_x / 4,
			_window.top,
			camera_x / 4,
			camera_y / 5,
			backDC,
			_window.left,
			_window.top,
			camera_x,
			camera_y,
			SRCCOPY);
	}
	//DC�������� ��Ӻ��縦 ����(ȭ�� ������ ����)
	BitBlt(
		frontDC,				//����޴� DC
		destX,					//����޴� ��ǥ(left)
		destY,					//����޴� ��ǥ(top)
		camera_x,				//����Ǵ� ����ũ��
		camera_y,			//����Ǵ� ����ũ��
		backDC,					//����Ǵ� DC
		_window.left, _window.top,	//����Ǵ� ������ǥ(left, top)
		SRCCOPY);				//�״�� �����ؿ���(���)

}