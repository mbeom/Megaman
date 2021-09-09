#include "stdafx.h"
#include "image.h"

#pragma comment(lib, "msimg32.lib")

//��� �̴ϼȶ�����
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
	_imageInfo->hMemDC = CreateCompatibleDC(hdc); //�� DC���� ��������
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���� �ʱ�ȭ�� ������ߴµ� �� �ȵƴٸ�?
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

	//DC�� ������ ����Ҷ� GetDC()
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

	//���࿡ �̹��� �ʱ�ȭ�� �����ϸ�
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

	//DC�� �����ͼ� ����ϰ� ���� �������ش�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor, bool alpha)
{
	if (_imageInfo != NULL) release();

	//DC�� ������ ����Ҷ� GetDC()
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

	//���࿡ �̹��� �ʱ�ȭ�� �����ϸ�
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

	//DC�� �����ͼ� ����ϰ� ���� �������ش�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor, bool alpha)
{
	if (_imageInfo != NULL) release();

	//DC�� ������ ����Ҷ� GetDC()
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

	//���࿡ �̹��� �ʱ�ȭ�� �����ϸ�
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

	//DC�� �����ͼ� ����ϰ� ���� �������ش�
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

	// player - camera : �÷��̾ ȭ�� �߽����� ī�޶�����
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
			hdc,				//����� ������ DC
			destX,				//����� ��ǥ(left)
			destY,				//����� ��ǥ(top)
			_imageInfo->width,	//����� ũ�� (����ũ��)
			_imageInfo->height,	//����� ũ�� (����ũ��)

			_imageInfo->hMemDC,	//�����ؿ� DC 
			0, 0,				//�����ؿ� ������ǥ(left, top)
			_imageInfo->width,	//�����ؿ� ����ũ��
			_imageInfo->height,	//�����ؿ� ����ũ��
			_transColor			//�����Ҷ� ������ �ȼ���
		);
	}
	else
	{
		//DC�������� ���Ӻ���
		BitBlt(
			hdc,					//�����ؼ� �ٿ��ִ� DC	
			0,						//�ٿ����� ��ǥ(left)
			0,						//�ٿ����� ��ǥ(top)
			_imageInfo->width,		//������ ����ũ��
			_imageInfo->height,		//������ ����ũ��

			_imageInfo->hMemDC,		//�����Ϸ��� DC
			-delta_x, 
			-delta_y,		//�����ؿ� ������ǥ(left, top)
			SRCCOPY);				//�����Ҷ� ��������Ű�������~

	}


}

void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,				//����� ������ DC
			destX,				//����� ��ǥ(left)
			destY,				//����� ��ǥ(top)
			sourWidth,			//����� ũ�� (����ũ��)
			sourHeight,			//����� ũ�� (����ũ��)
			_imageInfo->hMemDC,	//�����ؿ� DC 
			sourX, sourY,		//�����ؿ� ������ǥ(left, top)
			sourWidth,			//�����ؿ� ����ũ��
			sourHeight,			//�����ؿ� ����ũ��
			_transColor			//�����Ҷ� ������ �ȼ���
		);
	}
	else
	{
		//DC�������� ���Ӻ���
		BitBlt(
			hdc,					//������ DC	
			destX,					//������ ��ǥ(left)
			destY,					//������ ��ǥ(top)
			sourWidth,				//������ ����ũ��
			sourHeight,				//������ ����ũ��
			_imageInfo->hMemDC,		//����� DC
			sourX, sourY,			//����� ������ǥ(left, top)
			SRCCOPY);				//�����Ҷ� ��������Ű�������~

	}

}

void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,						//����� ������ DC
			destX,						//����� ��ǥ(left)
			destY,						//����� ��ǥ(top)
			_imageInfo->frameWidth,		//����� ũ�� (����ũ��)
			_imageInfo->frameHeight,	//����� ũ�� (����ũ��)
			_imageInfo->hMemDC,			//�����ؿ� DC 
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,				//�����ؿ� ������ǥ(left, top)
			_imageInfo->frameWidth,		//�����ؿ� ����ũ��
			_imageInfo->frameHeight,	//�����ؿ� ����ũ��
			_transColor					//�����Ҷ� ������ �ȼ���
		);
	}
	else
	{
		//DC�������� ���Ӻ���
		BitBlt(
			hdc,					//������ DC	
			destX,					//������ ��ǥ(left)
			destY,					//������ ��ǥ(top)
			_imageInfo->frameWidth,	//������ ����ũ��
			_imageInfo->frameHeight,//������ ����ũ��
			_imageInfo->hMemDC,		//����� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,			//����� ������ǥ(left, top)
			SRCCOPY);				//�����Ҷ� ��������Ű�������~

	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{
		GdiTransparentBlt(
			hdc,						//����� ������ DC
			destX,						//����� ��ǥ(left)
			destY,						//����� ��ǥ(top)
			_imageInfo->frameWidth,		//����� ũ�� (����ũ��)
			_imageInfo->frameHeight,	//����� ũ�� (����ũ��)
			_imageInfo->hMemDC,			//�����ؿ� DC 
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,				//�����ؿ� ������ǥ(left, top)
			_imageInfo->frameWidth,		//�����ؿ� ����ũ��
			_imageInfo->frameHeight,	//�����ؿ� ����ũ��
			_transColor					//�����Ҷ� ������ �ȼ���
		);
	}
	else
	{
		//DC�������� ���Ӻ���
		BitBlt(
			hdc,					//������ DC	
			destX,					//������ ��ǥ(left)
			destY,					//������ ��ǥ(top)
			_imageInfo->frameWidth,	//������ ����ũ��
			_imageInfo->frameHeight,//������ ����ũ��
			_imageInfo->hMemDC,		//����� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,			//����� ������ǥ(left, top)
			SRCCOPY);				//�����Ҷ� ��������Ű�������~

	}
}


void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	//������ ������ ���������� % �����ڷ� �ϸ� ����ϴ�
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	//�ʿ��� ������
	int sourWidth;
	int sourHeight;

	RECT rcDest;
	RECT rcSour;

	//�׷����� ������ ��������(���غ���)
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawAreaX;
	int drawAreaH = drawArea->bottom - drawAreaY;

	//Y�� �������� (������ �����ϴ�)

	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//ȭ�� ������ ���� ���� Ȯ�� (�����ؿ;��ϴ� ���� ����)
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//�����൵ �սô�
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
	//���� �̹��� �ҽ��� ���ĺ����带 �������ִ� �Լ� 
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//�̹����� ���� �����սô�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, _imageInfo->x, _imageInfo->y, SRCCOPY);

		//�����ؿ� �̹������� Ư��Į��(���� ����Ÿ)�� ����
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transColor);

		//���� ����
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
		//�̹����� ���� �����սô�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		//�����ؿ� �̹������� Ư��Į��(���� ����Ÿ)�� ����
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transColor);

		//���� ����
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
		// �߶�� ũ�⸸ŭ �̹����� ���� �����սô�
		BitBlt(_blendImage->hMemDC, 0, 0, sourWidth, sourHeight,
			hdc, destX, destY, SRCCOPY);

		//�����ؿ� �̹������� Ư��Į��(���� ����Ÿ)�� ����
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, sourWidth,
			sourHeight, _imageInfo->hMemDC, sourX, sourY, sourWidth,
			sourHeight, _transColor);

		//���� ����
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
	//���� �̹��� �ҽ��� ���ĺ����带 �������ִ� �Լ� 
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{

		//�̹����� ���� �����սô�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
			hdc, destX, destY, SRCCOPY);

		//�����ؿ� �̹������� Ư��Į��(���� ����Ÿ)�� ����
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _transColor);

		//���� ����
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

	//���� �̹��� �ҽ��� ���ĺ����带 �������ִ� �Լ� 
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{

		//�̹����� ���� �����սô�
		BitBlt(_blendImage->hMemDC,			 // ��Ʈ�������� �ٿ����� ���ο� �ӽ� DC
			0, 							 // �� DC���� �������� X ��ǥ
			0, 							 // �� DC���� �������� Y ��ǥ
			_imageInfo->frameWidth, 	 // �̹����� �ʺ�
			_imageInfo->frameHeight,	 // �̹����� ����
			hdc, 						 // ������ ��Ʈ�� ������ ���� DC
			destX, 						 // ���� �������� X��ǥ�� ����. (X ~ imageWidth)
			destY, 						 // ���� �������� Y��ǥ�� ����. (Y ~ imageHeight)
			SRCCOPY
		);

		//�����ؿ� �̹������� Ư��Į��(���� ����Ÿ)�� ����
		GdiTransparentBlt(
			_blendImage->hMemDC, 									 // ����Ÿ �A �׸� �ٿ����� DC
			0, 														 // �������� X ��ǥ
			0, 														 // �������� Y ��ǥ
			_imageInfo->frameWidth, 								 // �̹��� �ʺ�
			_imageInfo->frameHeight,								 // �̹��� ����
			_imageInfo->hMemDC, 									 // ����Ÿ ���� �������ִ� �̹��� ���� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 	 // ���� ���۵Ǵ� X ��ǥ
			_imageInfo->currentFrameY * _imageInfo->frameHeight, 	 // ���� ���۵Ǵ� Y ��ǥ
			_imageInfo->frameWidth, 								 // ������ �̹��� �ʺ�
			_imageInfo->frameHeight, 								 // ������ �̹��� ����
			_transColor												 // ������ ��(����Ÿ)
		);

		//���� ����
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_blendImage->hMemDC, 0, 0,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc,							// �׸� �׸��� DC
			destX, 						// �׷����� X ��ǥ
			destY, 						// �׷����� Y ��ǥ
			_imageInfo->frameWidth, 		// X ��ǥ�κ��� �̹��� �ʺ�
			_imageInfo->frameHeight,		// Y ��ǥ�κ��� �̹��� ����
			_blendImage->hMemDC, 		// �׸� ������ DC
			0, 							// �׸� ������ X��ǥ
			0,							// �׸� ������ Y��ǥ
			_imageInfo->frameWidth, 		// X ��ǥ�κ��� �̹��� �ʺ�
			_imageInfo->frameHeight,		// Y ��ǥ�κ��� �̹��� ����
			_blendFunc);					// ���İ�
	}
}
