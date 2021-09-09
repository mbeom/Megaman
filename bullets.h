#pragma once
#include "gameNode.h"
#include <vector>

struct tagBullet
{
	image* bulletImage;		//총알 이미지 
	RECT rc;				//총알 렉트
	float x, y;				//총알 좌표(실시간)
	float angle;			//총알 각도
	float radius;			//총알 반지름
	float speed;			//총알 스피드
	float fireX, fireY;		//총알 발사시 처음 좌표(발사된 좌표)
	float arc_angle;
	bool isFire;			//발사됐누?
	bool isCrash;
	int direction;
	int count;				//총알 프레임카운트용
};

//공용으로 너도 나도 쏠 총알
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
	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);
	void change_angle(int arrNum);
	void move();

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};


//발사할때 생성해서 쏠 미싸일
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