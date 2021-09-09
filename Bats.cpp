#include "stdafx.h"
#include "Bats.h"

Bats::Bats()
{
	IMAGEMANAGER->addFrameImage("bats", "bats_move.bmp", 660, 220, 6, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("bats_attack", "bats_attack.bmp", 220, 220, 1, 2, true, RGB(255, 0, 255), false);
	
	_type = FLYING;
}

void Bats::move()
{
	angle += 0.1f;

	_rc.left -= 4;
	_rc.right -= 4;
	dist_count -= 5;

	_rc.top -= 2 * sin(angle);
	_rc.bottom -= 2 * sin(angle);

	if (dist_count <= 0)
	{
		angle = PI/2;
	}
}
