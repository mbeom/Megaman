#pragma once
#include "gameNode.h"
#include <vector>

struct tagBullet
{
	image* bulletImage;		//�Ѿ� �̹��� 
	RECT rc;				//�Ѿ� ��Ʈ
	float x, y;				//�Ѿ� ��ǥ(�ǽð�)
	float angle;			//�Ѿ� ����
	float radius;			//�Ѿ� ������
	float speed;			//�Ѿ� ���ǵ�
	float fireX, fireY;		//�Ѿ� �߻�� ó�� ��ǥ(�߻�� ��ǥ)
	float arc_angle;
	bool isFire;			//�߻�ƴ�?
	bool isCrash;
	int direction;
	int count;				//�Ѿ� ������ī��Ʈ��
};

//�������� �ʵ� ���� �� �Ѿ�
class bullet : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;
public:
	bullet() {};
	~bullet() {};

	HRESULT init(const char* imageName, int bulletMax, float range);
	void release();
	void update();
	void render();

	void fire(float x, float y, float angle, float speed);
	//�Ѿ��� �����޶�� �����ϴ� �Լ�
	void removeBullet(int arrNum);
	void change_angle(int arrNum);
	void move();

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};


//�߻��Ҷ� �����ؼ� �� �̽���
class hyunMoo : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	float _range_plus;
	int _bulletMax;
	int _direction;
	int type;
	bool _isFired;

public:
	hyunMoo() {};
	~hyunMoo() {};

	HRESULT init(int bulletMax, float range);
	void release();
	void update(const char* name);
	void render();

	void fire(float x, float y, int direction, const char* name);
	void flasher_fire(float x, float y);
	void boss_UltimateFire(float x, float y);

	void move();
	void flasher_move();
	void bumerang_move();
	void removeBullet(int arrNum);

	vector<tagBullet> getVHyunMoo() { return _vBullet; }
	vector<tagBullet>::iterator getVIHyunMoo() { return _viBullet; }
	bool is_Fired() { return _isFired;  }
};