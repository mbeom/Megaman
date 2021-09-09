#include "stdafx.h"
#include "HP_Bar.h"

HRESULT HP_bar::init(int x, int y, int width, int height, const char* name)
{
    _x = x;
    _y = y;

    _HP_Bar = RectMake(x, y, width, height);
  
    _HP_Top = new image;
    _HP_Top->init("HP_Top.bmp", width, height, true, RGB(255, 0, 255),false);
    _HP_Bottom = new image;
    _HP_Bottom->init("HP_Bottom.bmp", width, height, true, RGB(255, 0, 255),false);
    _edge = new image;

    if(name == "player")
        _edge->init("Bar_edge.bmp", width, height, true, RGB(255, 0, 255), false);

    if(name == "boss")
        _edge->init("bossBar_edge.bmp", width, height, true, RGB(255, 0, 255), false);

    //게이지의 가로크기는 중첩된 게이지 이미지 중에서 앞에 실제로 조정될 가로크기로!
    _width = _HP_Top->getWidth();
    _height = 0;

    return S_OK;
}

void HP_bar::release()
{
}

void HP_bar::update(int x, int y)
{
    _HP_Bar = RectMakeCenter(x, y, _HP_Top->getWidth(),
        _HP_Top->getHeight());
}

void HP_bar::render()
{
    _HP_Bottom->render(getMemDC(),
        _HP_Bar.left + _HP_Bottom->getWidth() / 2,
        _y + _HP_Bottom->getHeight() / 2, 0, 0,
        _HP_Bottom->getWidth(), _HP_Bottom->getHeight());

    //얘는 게이지 크기가 조절되어야하기때문에 가로크기(sourWidth)에 _width 변수 써줌
    _HP_Top->render(getMemDC(),
        _HP_Bar.left + _HP_Bottom->getWidth() / 2,
        (_y + _HP_Bottom->getHeight() / 2) + _height, 0, 0,
        _HP_Bottom->getWidth(), _HP_Bottom->getHeight() - _height - 35);

    _edge->render(getMemDC(), _HP_Bar.left + _HP_Bottom->getWidth() / 2, _y + _HP_Bottom->getHeight() / 2);

    //Rectangle(getMemDC(), _HP_Bar);
}

void HP_bar::setGauge(float currentGauge, float maxGauge)
{
    //퍼센테이지로 계산해주면 더욱 쉽고 조옿습니다
    _width = (currentGauge / maxGauge) * _HP_Bottom->getWidth();
}

void HP_bar::setGauge_V(float currentGauge, float maxGauge)
{
    //_height = maxGauge - ((currentGauge / maxGauge)* _HP_Bottom->getHeight());

    _height = ((maxGauge - currentGauge) / maxGauge) * (_HP_Bottom->getHeight() - 50);

    if (maxGauge == 1000)
    {
        _height = ((maxGauge - currentGauge) / maxGauge) * (_HP_Bottom->getHeight() - 77);
    }
}
