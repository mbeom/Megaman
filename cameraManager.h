#pragma once
#include "image.h"
#include "singletonBase.h"

class Player;

class cameraManager : public singletonBase<cameraManager>
{
private:
	RECT _window;
	int backDC_sizeX, backDC_sizeY; // ������� ������
	int camera_x, camera_y;			// �츮�� ĸ���� ȭ��
	Player* _player;

public:

	cameraManager();
	~cameraManager();

	HRESULT init(int width, int height);
	void release();
	
	//ī�޶� ��ġ ����
	void set_camera(int x, int y) { camera_x = x, camera_y = y; }
	void updateScreen(float standardX, float standardY, float ratioX = 0.5f, float ratioY = 0.5f);
	//ī�޶� ���
	void render(HDC frontDC, int destX, int destY, HDC backDC, bool map = false, bool mini = false);
	void Link_PlayerClass(Player* player) { _player = player; }

};

