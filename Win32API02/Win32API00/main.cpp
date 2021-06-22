#include <Windows.h>
#include "WndDefines.h"
#include "resource.h"

#pragma comment (lib, "msimg32.lib")

// ������ �׷���
/// GDI Object : �׷��� ��¿� ���Ǵ� ����
/// - HPEN : ���� �׸� �� ���˴ϴ�.
/// - HBRUSH : ���� �׸� �� ���˴ϴ�.
/// - HBITMAP : ��Ʈ�� �̹����� �׸� �� ����մϴ�.

// GDI Object ��� ���
/// - ����� DC ������ ����� GDI OBJECT �� ����Ͽ� ����մϴ�.
///   ����� �� �������� ����� GDI OBJECT �� ���˴ϴ�.
/// - ����� �����ٸ� Stock Object �� �ƴ� �̻� �� �����ؾ� �մϴ�.
///   ������ �� �ִ� GDI OBJECT �� DC ���� ������� �ƴ� OBJECT ���� �մϴ�.
/// - HGDIOBJ SelectObject(HDC hdc, HGDIOBJ obj) �� ���Ͽ� hdc �� ����� GDI OBJECT �� ���ý�ų �� �ֽ��ϴ�.
/// - DeleteObject(HGDIOBJ obj) �� ���Ͽ� Ư���� GDI OBJECT �� ������ �� �ֽ��ϴ�.

// StockObject
/// - OS �� �̸� ���� �����ϴ� GDI OBJECT �Դϴ�.
/// - HGDIOBJ GetStockObject(int fnObject) �� ���Ͽ� Ư���� StockObject �� ���� �� �ֽ��ϴ�.
/// 
/// fnObject �� ������ �� �ִ� ��
/// BLACK_BRUSH : ���� �귯��
/// WHITE_BRUSH : ��� �귯��
/// DKGRAY_BRUSH : ��ο� ȸ�� �귯��
/// LTGRAY_BRUSH : ���� ȸ�� �귯��
/// GRAY_BRUSH : ȸ�� �귯��
/// NULL_BRUSH : ���� �귯��
/// 
/// WHITE_PEN : ��� ��
/// BLACK_PEN : ���� ��
/// NULL_PEN : ���� ��
/// 
/// DC_BRUSH : ��� �귯���� �ڵ��� ��ȯ�մϴ�.
/// DC_PEN : ������ ���� �ڵ��� ��ȯ�մϴ�.



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstance;

int APIENTRY wWinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPTSTR		lpszCmdParam, 
	int			nCmdShow)
{
	HWND hWnd;
	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbClsExtra = wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = ::hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = TEXT("WndClass");
	wndClass.lpszMenuName = NULL;

	RegisterClass(&wndClass);
	hWnd = CreateWindow(
		wndClass.lpszClassName,
		WND_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT,
		NULL, NULL,
		wndClass.hInstance,
		NULL);
	
	ShowWindow(hWnd, nCmdShow);
	
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);

		// Message Queue ���� ���� ������ �޽����� ���ν����� �����մϴ�.
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	static HBITMAP myBitmap;

	switch (msg)
	{
	case WM_CREATE :
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);

		// ��Ʈ���� �ҷ��ɴϴ�.
		myBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SHINING_KIM));
		/// - LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName) : Bitmap ���ҽ��� �о�� �� ����ϴ� �Լ�
		/// - ���� �����Ѵٸ� BITMAP Handle �� �����ϸ�, �����Ѵٸ� NULL �� �����մϴ�.
		/// - hInstance : ��Ʈ�� ���ҽ��� ���� �ν��Ͻ� �ڵ� �����մϴ�.
		/// - lpBitmapName : �̹��� ������ �̸��� �����մϴ�.
		/// 
		/// - MAKEINTRESOURCE
		///   ������ ���ҽ� ID ���� ���ڿ��� ������ ĳ�������ִ� ��ũ���Դϴ�.


		return 0;
	}

	case WM_PAINT :
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// �׵θ��� ���� ��� ������ ������ �簢���� �׷����ô�.

		HBRUSH myBrush = CreateSolidBrush(RGB(121, 28, 145));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		/// - BRUSH : ���� ä�� �� ����ϴ� GDIOBJ �Դϴ�.
		/// - �ܻ����� ä��� ���
		///   HBRUSH CreateSolidBrush(COLORREF color)
		/// - Ư���� �������� ä��� ���
		///   HBRUSH CreateHatchBrush(int style, COLORREF color)
		///   - HS_CROSS		�ٵ��� ��		   [����������������������]
		///   - HS_BDIAGONAL	������ ��		   [������������]
		///   - HS_FDIAGONAL	������ ��		   [������������]
		///   - HS_DIAGCROSS	������ ������ ��	   [�أأأأأ�]
		///   - HS_HORIZONTAL	����			   [����������������������]
		///   - HS_VERTICAL		������			   [����������������������]
		
		HPEN myPen = CreatePen(PS_SOLID, 5, RGB(56, 55, 72));
		HPEN oldPen = (HPEN)SelectObject(hdc, myPen);
		// PEN : ���� �׸� �� ����ϴ� GDIOBJ �Դϴ�.
		/// - HPEN CreatePen(int style, int width, COLORREF color)
		/// - style �� ���� ����(width) �� 1 �� ��쿡�� ������ �� �ֽ��ϴ�.
		///   ���� 1 �� �ƴ϶�� �⺻ ��(PS_SOLID) �� �����˴ϴ�.
		/// - style �� ������ �� �ִ� ����
		///   - PS_SOLID		�Ǽ�					[��������������������������������  ]
		///   - PS_DASH			�ļ�					[�� �� �� �� �� �� �� ��   ]
		///   - PS_DOT			����					[��������������������������������  ]
		///   - PS_DASHDOT		�����⼱				[�� �� �� �� �� �� �� ��   ]
		///   - PS_DASHDOTDOT	�����⼱				[�� �� �� �� �� �� �� �� �� ]


		
		Rectangle(hdc, 50, 50, 150, 150);

		// DC �� ���� ����ϴ� BRUSH ��ü�� ���ý�ŵ�ϴ�.
		/// - �̶� DC �� ������̴� myBrush �� ��ȯ�˴ϴ�.
		SelectObject(hdc, oldBrush);
		DeleteObject(myBrush);	// myBrush ����

		DeleteObject(SelectObject(hdc, oldPen));	// myPen ����



		// ���� ������ �簢���� �׷����ô�.
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, myPen);

		Rectangle(hdc, 100, 100, 250, 250);

		DeleteObject(SelectObject(hdc, oldPen));






		// ���� �ļ� �׵θ��� �ٵ��� ����� �� �׸���
		myPen = CreatePen(PS_DASH, 1, RGB(50, 200, 200));
		myBrush = CreateHatchBrush(HS_CROSS, RGB(120, 240, 40));
		oldPen = (HPEN)SelectObject(hdc, myPen);
		oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

		Ellipse(hdc, 300, 300, 550, 550);

		DeleteObject(SelectObject(hdc, myPen));
		DeleteObject(SelectObject(hdc, myBrush));
			
		/*
		HPEN / HBRUSH ������ drawPen, drawBrush, oldDrawPen, oldDrawBrush �� �����ϰ�
		������ ���� ������ �׷�������.

		Rectangle						Ellipse
		Left : 300						CenterXY : 400, 200
		Top : 150						Radius : 50
		Width : 50						PenStyle : PS_DASHDOT
		Height : 100					PenColor : [0, 0, 255]
		PenWidth : 3					Brush Style : HS_DIAGCROSS
		PenColor : [0, 255, 0]			BrushColor : [255, 0, 255]
		Brush Style : HS_FDIAGONAL
		BrushColor : [255, 0, 0]
		*/

		// Rectangle
		HPEN drawPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0)),
			oldDrawPen = (HPEN)SelectObject(hdc, drawPen);
		HBRUSH drawBrush = CreateHatchBrush(HS_FDIAGONAL, RGB(255, 0, 0)),
			oldDrawBrush = (HBRUSH)SelectObject(hdc, drawBrush);

		Rectangle(hdc, 300, 150, 350, 250);

		DeleteObject(SelectObject(hdc, oldDrawPen));
		DeleteObject(SelectObject(hdc, oldDrawBrush));

		// Ellipse
		drawPen = CreatePen(PS_DASHDOT, 1, RGB(0, 0, 255));
		oldDrawPen = (HPEN)SelectObject(hdc, drawPen);
		drawBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 255));
		oldDrawBrush = (HBRUSH)SelectObject(hdc, drawBrush);

		Ellipse(hdc, 350, 150, 450, 250);

		DeleteObject(SelectObject(hdc, oldDrawPen));
		DeleteObject(SelectObject(hdc, oldDrawBrush));


		// �޸𸮻� ���ο� DC �� �����մϴ�.
		HDC memDC = CreateCompatibleDC(hdc);
		/// - CreateCompatibleDC(HDC hdc)
		///   hdc �� ȣȯ�Ǵ�(���� ������ ����) DC �� �޸𸮻� �Ҵ��մϴ�.
		/// - ����� �����ٸ� �Ҵ�� DC �� DeleteDC() �� �����ؾ� �մϴ�.

		// HBITMAP �� GDIOBJ �̹Ƿ� ������ ���� ���� ��ü�� �����մϴ�.
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, myBitmap);

		// BITMAP ���� ����� �˾Ƴ��� ���Ͽ� ��Ʈ�� ������ ������ ������ �����մϴ�.
		BITMAP bitmapInfo;
		/// - BITMAP : �ε��� ��Ʈ���� ������ ������ �� ����ϴ� ����ü�Դϴ�.

		// �ε��� ��Ʈ�� �����Ϳ� ���� ������ ���մϴ�.
		GetObject(myBitmap, sizeof(BITMAP), &bitmapInfo);
		/// GetObject() : GDIOBJ �� ���� ������ ���մϴ�.
		/// - ������ ������Ʈ�� �ڵ�
		/// - ����� �����l ũ���Դϴ�.
		/// - ������Ʈ�� ������ ��ȯ���� ������ ���� �ּ�

		// ��Ʈ���� ����մϴ�.
		/// Bitblt : �̹����� ȭ�鿡 ����մϴ�.
		/// StretchBlt : �̹����� ũ�⸦ �����Ͽ� ȭ�鿡 ����մϴ�.
		/// TransparentBlt : �̹����� Ư���� ������ �����ϰ�, ũ�⸦ �����Ͽ� ȭ�鿡 ����մϴ�.
		

		/*
		BitBlt(
			hdc,				// �̹����� ����� ��ġ
			50, 50,				// �̹����� ����� ���� ��� ��ǥ
			bitmapInfo.bmWidth, // ���� �̹������� �߶�� ����ũ��
			bitmapInfo.bmHeight,// ���� �̹������� �߶�� ����ũ��
			memDC,              // �̹����� �ڵ�
			0, 0,				// �̹����� �ڸ��� ������ ��ġ (�� ��ġ���� cx, cy ��ŭ �߶�� ȭ�鿡 �׸��ϴ�.)
			SRCCOPY);			// ��� ���
		/// - SRCCOPY : ���� �̹����� ����մϴ�.
		/// - NOTSRCCOPY : �̹����� ������ ������ŵ�ϴ�.
		/// - BLACKNESS : �̹����� ������ ������ �ƴ�, ���������� ǥ���մϴ�.
		/// - WHITENESS : �̹����� ������ ������ �ƴ�, �Ͼ������ ǥ���մϴ�.
		*/

		// SetStretchBltMode(HDC hdc, int mode) : �̹��� ��� / Ȯ�� �� �߻��ϴ� �ս��� �����ϴ� �Լ�
		/// - hdc : StretchBlt �� ����� �� �׷����� DC
		/// - mode : ���� ���
		///   - BLACKONWHITE : ���ŵǰų� �����ϴ� �ȼ��� ������ ����Ͽ� AND ������ �����մϴ�.
		///   - HALFTONE	 : �ȼ��� ��ջ����� ���� �ȼ��� ���� �����ϴ� ��ġ�� ����մϴ�.
		///   - WHITEONBLACK : ���ŵǰų� �����ϴ� �ȼ��� ������ ����Ͽ� OR ������ �����մϴ�.
		SetStretchBltMode(hdc, HALFTONE);

		/*
		StretchBlt(
			hdc,
			50, 50,
			bitmapInfo.bmWidth/2,
			bitmapInfo.bmHeight/2,
			memDC,
			0, 0,
			bitmapInfo.bmWidth,
			bitmapInfo.bmHeight,
			SRCCOPY);
		*/

		TransparentBlt(
			hdc,
			50, 50,
			bitmapInfo.bmWidth,
			bitmapInfo.bmHeight,
			memDC,
			0, 0,
			bitmapInfo.bmWidth,
			bitmapInfo.bmHeight,
			RGB(255, 0, 255));

		// memDC �� ������̴� ��Ʈ�� �ڵ��� �����մϴ�.
		SelectObject(memDC, oldBitmap);

		// �Ҵ��� DC �� �����մϴ�.
		DeleteDC(memDC);

		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_DESTROY :

		DeleteObject(myBitmap);
		
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}