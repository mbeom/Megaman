#pragma once
#include "gameNode.h"

class HP_bar : public gameNode
{
private:
	RECT _HP_Bar;
	int _x, _y;
	float _width, _height;

	image* _HP_Top;
	image* _HP_Bottom;
	image* _edge;

public:

	HRESULT init(int x, int y, int width, int height, const char* name);
	void release();
	void update(int x, int y);
	void render();

	void setGauge(float currentGauge, float maxGauge);
	void setGauge_V(float currentGauge, float maxGauge);

	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
};

