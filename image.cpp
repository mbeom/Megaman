#include "stdafx.h"
#include "image.h"

#pragma comment(lib, "msimg32.lib")

//멤버 이니셜라이즈
image::image()
	: _imageInfo(NULL),
	_fileName(NULL),
	_trans(false),
	_transColor(RGB(0, 0, 0))
{
}


image::~image()
{
}

HRESULT image::init(int width, int height, bool alpha)
{
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc); //빈 DC영역 생성해줌
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//위에 초기화를 제대로했는데 잘 안됐다면?
	if (_imageInfo == nullptr)
	{
		release();

		return E_FAIL;
	}

	if (alpha)
	{
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, BOOL trans, COLORREF transColor, bool alpha)
{
	if (_imageInfo != NULL) release();

	//DC를 가져다 사용할땐 GetDC()
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//만약에 이미지 초기화가 실패하면
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	if (alpha)
	{
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}

	//DC를 가져와서 사용하고 끝에 해제해준다
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor, bool alpha)
{
	if (_imageInfo != NULL) release();

	//DC를 가져다 사용할땐 GetDC()
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//만약에 이미지 초기화가 실패하면
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	if (alpha)
	{
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}

	//DC를 가져와서 사용하고 끝에 해제해준다
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor, bool alpha)
{
	if (_imageInfo != NULL) release();

	//DC를 가져다 사용할땐 GetDC()
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//만약에 이미지 초기화가 실패하면
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	if (alpha)
	{
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}

	//DC를 가져와서 사용하고 끝에 해제해준다
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void image::release()
{
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);

		_trans = false;
		_transColor = RGB(0, 0, 0);
	}


}

void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_trans = trans;
	_transColor = transColor;
}


void image::render(HDC hdc, int destX, int destY)
{
	float delta_x = 0 - (destX - WINSIZEX / 2);
	float delta_y = 0 - (destY - WINSIZEY);

	// player - camera : 플레이어가 화면 중심으로 카메라잡음
	if (delta_x > 0)
	{
		delta_x = 0;
	}
	if (delta_y > 0)
	{
		delta_y = 0;
	}

	if (delta_x < -WINSIZEX * 7)
	{
		delta_x = -WINSIZEX * 7;
	}
	if (delta_y < -WINSIZEY)
	{
		delta_y = -WINSIZEY;
	}

	if (_trans)
	{
		GdiTransparentBlt(
			hdc,				//복사될 영역의 DC
			destX,				//복사될 좌표(left)
			destY,				//복사될 좌표(top)
			_imageInfo->width,	//복사될 크기 (가로크기)
			_imageInfo->height,	//복사될 크기 (세로크기)

			_imageInfo->hMemDC,	//복사해올 DC 
			0, 0,				//복사해올 시작좌표(left, top)
			_imageInfo->width,	//복사해올 가로크기
			_imageInfo->height,	//복사해올 세로크기
			_transColor			//복사할때 제외할 픽셀값
		);
	}
	else
	{
		//DC영역간의 고속복사
		BitBlt(
			hdc,					//복사해서 붙여넣는 DC	
			0,						//붙여넣을 좌표(left)
			0,						//붙여넣을 좌표(top)
			_imageInfo->width,		//복사할 가로크기
			_imageInfo->height,		//복사할 세로크기

			_imageInfo->hMemDC,		//복사하려는 DC
			-delta_x, 
			-delta_y,		//복사해올 시작좌표(left, top)
			SRCCOPY);				//복사할때 변형일으키지말어라~

	}


}

void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,				//복사될 영역의 DC
			destX,				//복사될 좌표(left)
			destY,				//복사될 좌표(top)
			sourWidth,			//복사될 크기 (가로크기)
			sourHeight,			//복사될 크기 (세로크기)
			_imageInfo->hMemDC,	//복사해올 DC 
			sourX, sourY,		//복사해올 시작좌표(left, top)
			sourWidth,			//복사해올 가로크기
			sourHeight,			//복사해올 세로크기
			_transColor			//복사할때 제외할 픽셀값
		);
	}
	else
	{
		//DC영역간의 고속복사
		BitBlt(
			hdc,					//복사할 DC	
			destX,					//복사할 좌표(left)
			destY,					//복사할 좌표(top)
			sourWidth,				//복사할 가로크기
			sourHeight,				//복사할 세로크기
			_imageInfo->hMemDC,		//복사될 DC
			sourX, sourY,			//복사될 시작좌표(left, top)
			SRCCOPY);				//복사할때 변형일으키지말어라~

	}

}

void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,						//복사될 영역의 DC
			destX,						//복사될 좌표(left)
			destY,						//복사될 좌표(top)
			_imageInfo->frameWidth,		//복사될 크기 (가로크기)
			_imageInfo->frameHeight,	//복사될 크기 (세로크기)
			_imageInfo->hMemDC,			//복사해올 DC 
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,				//복사해올 시작좌표(left, top)
			_imageInfo->frameWidth,		//복사해올 가로크기
			_imageInfo->frameHeight,	//복사해올 세로크기
			_transColor					//복사할때 제외할 픽셀값
		);
	}
	else
	{
		//DC영역간의 고속복사
		BitBlt(
			hdc,					//복사할 DC	
			destX,					//복사할 좌표(left)
			destY,					//복사할 좌표(top)
			_imageInfo->frameWidth,	//복사할 가로크기
			_imageInfo->frameHeight,//복사할 세로크기
			_imageInfo->hMemDC,		//복사될 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,			//복사될 시작좌표(left, top)
			SRCCOPY);				//복사할때 변형일으키지말어라~

	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{
		GdiTransparentBlt(
			hdc,						//복사될 영역의 DC
			destX,						//복사될 좌표(left)
			destY,						//복사될 좌표(top)
			_imageInfo->frameWidth,		//복사될 크기 (가로크기)
			_imageInfo->frameHeight,	//복사될 크기 (세로크기)
			_imageInfo->hMemDC,			//복사해올 DC 
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,				//복사해올 시작좌표(left, top)
			_imageInfo->frameWidth,		//복사해올 가로크기
			_imageInfo->frameHeight,	//복사해올 세로크기
			_transColor					//복사할때 제외할 픽셀값
		);
	}
	else
	{
		//DC영역간의 고속복사
		BitBlt(
			hdc,					//복사할 DC	
			destX,					//복사할 좌표(left)
			destY,					//복사할 좌표(top)
			_imageInfo->frameWidth,	//복사할 가로크기
			_imageInfo->frameHeight,//복사할 세로크기
			_imageInfo->hMemDC,		//복사될 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,			//복사될 시작좌표(left, top)
			SRCCOPY);				//복사할때 변형일으키지말어라~

	}
}


void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	//오프셋 영역을 보정해주자 % 연산자로 하면 깔끔하다
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	//필요한 도구들
	int sourWidth;
	int sourHeight;

	RECT rcDest;
	RECT rcSour;

	//그려지는 영역을 설정하자(구해보자)
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawAreaX;
	int drawAreaH = drawArea->bottom - drawAreaY;

	//Y축 루프부터 (순서는 무관하다)

	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//화면 밖으로 나간 영역 확보 (복사해와야하는 범위 설정)
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//가로축도 합시다
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}

}

void image::alphaRender(HDC hdc, BYTE alpha)
{
	//실제 이미지 소스에 알파블렌드를 적용해주는 함수 
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//이미지를 먼저 복사합시다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, _imageInfo->x, _imageInfo->y, SRCCOPY);

		//복사해온 이미지에서 특정칼라(보통 마젠타)를 제거
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transColor);

		//알파 적용
		AlphaBlend(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//이미지를 먼저 복사합시다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		//복사해온 이미지에서 특정칼라(보통 마젠타)를 제거
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transColor);

		//알파 적용
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		// 잘라올 크기만큼 이미지를 먼저 복사합시다
		BitBlt(_blendImage->hMemDC, 0, 0, sourWidth, sourHeight,
			hdc, destX, destY, SRCCOPY);

		//복사해온 이미지에서 특정칼라(보통 마젠타)를 제거
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, sourWidth,
			sourHeight, _imageInfo->hMemDC, sourX, sourY, sourWidth,
			sourHeight, _transColor);

		//알파 적용
		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
}

void image::alphaFrameRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	//실제 이미지 소스에 알파블렌드를 적용해주는 함수 
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{

		//이미지를 먼저 복사합시다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
			hdc, destX, destY, SRCCOPY);

		//복사해온 이미지에서 특정칼라(보통 마젠타)를 제거
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _transColor);

		//알파 적용
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_blendImage->hMemDC, 0, 0,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_blendImage->hMemDC, 0, 0,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);
	}
}

void image::alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	//실제 이미지 소스에 알파블렌드를 적용해주는 함수 
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{

		//이미지를 먼저 복사합시다
		BitBlt(_blendImage->hMemDC,			 // 비트맵정보를 붙여넣을 새로운 임시 DC
			0, 							 // 새 DC에서 시작점의 X 좌표
			0, 							 // 새 DC에서 시작점의 Y 좌표
			_imageInfo->frameWidth, 	 // 이미지의 너비
			_imageInfo->frameHeight,	 // 이미지의 높이
			hdc, 						 // 복사할 비트맵 정보를 가진 DC
			destX, 						 // 복사 시작점의 X좌표를 정함. (X ~ imageWidth)
			destY, 						 // 복사 시작점의 Y좌표를 정함. (Y ~ imageHeight)
			SRCCOPY
		);

		//복사해온 이미지에서 특정칼라(보통 마젠타)를 제거
		GdiTransparentBlt(
			_blendImage->hMemDC, 									 // 마젠타 뺸 그림 붙여넣을 DC
			0, 														 // 시작점의 X 좌표
			0, 														 // 시작점의 Y 좌표
			_imageInfo->frameWidth, 								 // 이미지 너비
			_imageInfo->frameHeight,								 // 이미지 높이
			_imageInfo->hMemDC, 									 // 마젠타 색을 가지고있는 이미지 원본 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 	 // 복사 시작되는 X 좌표
			_imageInfo->currentFrameY * _imageInfo->frameHeight, 	 // 복사 시작되는 Y 좌표
			_imageInfo->frameWidth, 								 // 복사할 이미지 너비
			_imageInfo->frameHeight, 								 // 복사할 이미지 높이
			_transColor												 // 제거할 색(마젠타)
		);

		//알파 적용
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_blendImage->hMemDC, 0, 0,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc,							// 그림 그리는 DC
			destX, 						// 그려놓을 X 좌표
			destY, 						// 그려놓을 Y 좌표
			_imageInfo->frameWidth, 		// X 좌표로부터 이미지 너비
			_imageInfo->frameHeight,		// Y 좌표로부터 이미지 높이
			_blendImage->hMemDC, 		// 그림 가져올 DC
			0, 							// 그림 가져올 X좌표
			0,							// 그림 가져올 Y좌표
			_imageInfo->frameWidth, 		// X 좌표로부터 이미지 너비
			_imageInfo->frameHeight,		// Y 좌표로부터 이미지 높이
			_blendFunc);					// 알파값
	}
}

