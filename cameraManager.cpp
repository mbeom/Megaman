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
	//backDC와 frontDC크기 설정
	backDC_sizeX = width;
	backDC_sizeY = height;
	camera_x = WINSIZEX;
	camera_y = WINSIZEY;

	//화면 Rect 생성
	_window = RectMake(0, 0, WINSIZEX, WINSIZEY);

	return S_OK;
}

void cameraManager::release()
{
}

void cameraManager::updateScreen(float standardX, float standardY, float ratioX, float ratioY)
{
	//플레이어 기준으로 화면(_screen)갱신
	//playground.cpp의 update()에서 계속 갱신해주면 됨
	//인수로 기준이 되는 객체의 x, y좌표를 받음(현재는 player의 x, y)

	//X축 (좌, 우)
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

	//Y축 (상, 하)

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

	//갱신
	_window = RectMake(standardX, standardY, camera_x, camera_y);
	_player->get_hpPointer()->update(_window.left, _window.top + 100);
}

void cameraManager::render(HDC frontDC, int destX, int destY, HDC backDC, bool map, bool mini)
{
	/********* 인수 설명 *********/ /*
	HDC frontDC: 복사를 받을 DC (실제로 우리가 보는 화면)
	int destX: 우리가 보는 화면에서 출력해줄 x좌표
	int destY: 우리가 보는 화면에서 출력해줄 y좌표
	HDC backDC: 복사해 올 DC (실제로 우리가 그리는 화면)
	bool map: true면 전체맵 출력, false면 안 출력
	bool mini: true면 미니맵 출력, false면 안 출력
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
	//미니맵 변수가 true면 스크린의 우측상단에 미니맵 출력
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
	//DC영역간의 고속복사를 해줌(화면 깜빡임 방지)
	BitBlt(
		frontDC,				//복사받는 DC
		destX,					//복사받는 좌표(left)
		destY,					//복사받는 좌표(top)
		camera_x,				//복사되는 가로크기
		camera_y,			//복사되는 세로크기
		backDC,					//복사되는 DC
		_window.left, _window.top,	//복사되는 시작좌표(left, top)
		SRCCOPY);				//그대로 복사해오기(모드)

}