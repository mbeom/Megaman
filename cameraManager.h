#pragma once
#include "image.h"
#include "singletonBase.h"

class Player;

class cameraManager : public singletonBase<cameraManager>
{
private:
	RECT _window;
	int backDC_sizeX, backDC_sizeY; // 백버퍼의 사이즈
	int camera_x, camera_y;			// 우리가 캡쳐할 화면
	Player* _player;

public:

	cameraManager();
	~cameraManager();

	HRESULT init(int width, int height);
	void release();
	
	//카메라 위치 갱신
	void set_camera(int x, int y) { camera_x = x, camera_y = y; }
	void updateScreen(float standardX, float standardY, float ratioX = 0.5f, float ratioY = 0.5f);
	//카메라 출력
	void render(HDC frontDC, int destX, int destY, HDC backDC, bool map = false, bool mini = false);
	void Link_PlayerClass(Player* player) { _player = player; }

};

