#include <Windows.h>
#include "WndDefines.h"
#include "resource.h"

#pragma comment (lib, "msimg32.lib")

// 윈도우 그래픽
/// GDI Object : 그래픽 출력에 사용되는 도구
/// - HPEN : 선을 그릴 때 사용됩니다.
/// - HBRUSH : 면을 그릴 때 사용됩니다.
/// - HBITMAP : 비트맵 이미지를 그릴 때 사용합니다.

// GDI Object 사용 방법
/// - 사용할 DC 영역에 사용할 GDI OBJECT 를 등록하여 사용합니다.
///   사용할 때 마지막에 등록한 GDI OBJECT 가 사용됩니다.
/// - 사용이 끝난다면 Stock Object 가 아닌 이상 꼭 해제해야 합니다.
///   해제될 수 있는 GDI OBJECT 는 DC 에서 사용중이 아닌 OBJECT 여야 합니다.
/// - HGDIOBJ SelectObject(HDC hdc, HGDIOBJ obj) 를 통하여 hdc 가 사용할 GDI OBJECT 를 선택시킬 수 있습니다.
/// - DeleteObject(HGDIOBJ obj) 를 통하여 특정한 GDI OBJECT 를 해제할 수 있습니다.

// StockObject
/// - OS 가 미리 만들어서 관리하는 GDI OBJECT 입니다.
/// - HGDIOBJ GetStockObject(int fnObject) 를 통하여 특정한 StockObject 를 얻을 수 있습니다.
/// 
/// fnObject 에 전달할 수 있는 값
/// BLACK_BRUSH : 검정 브러쉬
/// WHITE_BRUSH : 흰색 브러쉬
/// DKGRAY_BRUSH : 어두운 회색 브러쉬
/// LTGRAY_BRUSH : 밝은 회색 브러쉬
/// GRAY_BRUSH : 회색 브러쉬
/// NULL_BRUSH : 투명 브러쉬
/// 
/// WHITE_PEN : 흰색 펜
/// BLACK_PEN : 검정 펜
/// NULL_PEN : 투명 펜
/// 
/// DC_BRUSH : 흰색 브러쉬의 핸들을 반환합니다.
/// DC_PEN : 검정색 펜의 핸들을 반환합니다.



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

		// Message Queue 에서 꺼내 가공한 메시지를 프로시저에 전달합니다.
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

		// 비트맵을 불러옵니다.
		myBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SHINING_KIM));
		/// - LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName) : Bitmap 리소스를 읽어올 때 사용하는 함수
		/// - 만약 성공한다면 BITMAP Handle 을 리턴하며, 실패한다면 NULL 을 리턴합니다.
		/// - hInstance : 비트맵 리소스를 가진 인스턴스 핸들 전달합니다.
		/// - lpBitmapName : 이미지 파일의 이름을 전달합니다.
		/// 
		/// - MAKEINTRESOURCE
		///   정수형 리소스 ID 값을 문자열로 적당히 캐스팅해주는 매크로입니다.


		return 0;
	}

	case WM_PAINT :
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 테두리와 면이 어떠한 색상을 가지는 사각형을 그려봅시다.

		HBRUSH myBrush = CreateSolidBrush(RGB(121, 28, 145));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		/// - BRUSH : 면을 채울 때 사용하는 GDIOBJ 입니다.
		/// - 단색으로 채우는 방식
		///   HBRUSH CreateSolidBrush(COLORREF color)
		/// - 특정한 패턴으로 채우는 방식
		///   HBRUSH CreateHatchBrush(int style, COLORREF color)
		///   - HS_CROSS		바둑판 줄		   [┼┼┼┼┼┼┼┼┼┼┼]
		///   - HS_BDIAGONAL	좌하향 줄		   [／／／／／／]
		///   - HS_FDIAGONAL	우하향 줄		   [＼＼＼＼＼＼]
		///   - HS_DIAGCROSS	좌하향 우하향 줄	   [ＸＸＸＸＸＸ]
		///   - HS_HORIZONTAL	수평선			   [───────────]
		///   - HS_VERTICAL		수직선			   [│││││││││││]
		
		HPEN myPen = CreatePen(PS_SOLID, 5, RGB(56, 55, 72));
		HPEN oldPen = (HPEN)SelectObject(hdc, myPen);
		// PEN : 선을 그릴 개 사용하는 GDIOBJ 입니다.
		/// - HPEN CreatePen(int style, int width, COLORREF color)
		/// - style 은 펜의 굵기(width) 가 1 일 경우에만 설정할 수 있습니다.
		///   만약 1 이 아니라면 기본 값(PS_SOLID) 로 설정됩니다.
		/// - style 에 설정할 수 있는 값들
		///   - PS_SOLID		실선					[────────────────  ]
		///   - PS_DASH			파선					[· ─ ─ ─ ─ ─ ─ ─   ]
		///   - PS_DOT			점선					[················  ]
		///   - PS_DASHDOT		일점쇄선				[· ─ · ─ · ─ · ─   ]
		///   - PS_DASHDOTDOT	이점쇄선				[· · ─ · · ─ · · ─ ]


		
		Rectangle(hdc, 50, 50, 150, 150);

		// DC 가 전에 사용하던 BRUSH 객체를 선택시킵니다.
		/// - 이때 DC 가 사용중이던 myBrush 가 반환됩니다.
		SelectObject(hdc, oldBrush);
		DeleteObject(myBrush);	// myBrush 해제

		DeleteObject(SelectObject(hdc, oldPen));	// myPen 해제



		// 면이 투명한 사각형을 그려봅시다.
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, myPen);

		Rectangle(hdc, 100, 100, 250, 250);

		DeleteObject(SelectObject(hdc, oldPen));






		// 옥색 파선 테두리의 바둑판 연녹색 원 그리기
		myPen = CreatePen(PS_DASH, 1, RGB(50, 200, 200));
		myBrush = CreateHatchBrush(HS_CROSS, RGB(120, 240, 40));
		oldPen = (HPEN)SelectObject(hdc, myPen);
		oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

		Ellipse(hdc, 300, 300, 550, 550);

		DeleteObject(SelectObject(hdc, myPen));
		DeleteObject(SelectObject(hdc, myBrush));
			
		/*
		HPEN / HBRUSH 형태의 drawPen, drawBrush, oldDrawPen, oldDrawBrush 를 선언하고
		다음과 같은 도형을 그려보세요.

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


		// 메모리상에 새로운 DC 를 생성합니다.
		HDC memDC = CreateCompatibleDC(hdc);
		/// - CreateCompatibleDC(HDC hdc)
		///   hdc 와 호환되는(색상 포멧이 같은) DC 를 메모리상에 할당합니다.
		/// - 사용이 끝난다면 할당된 DC 는 DeleteDC() 로 해제해야 합니다.

		// HBITMAP 도 GDIOBJ 이므로 해제를 위해 이전 객체를 저장합니다.
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, myBitmap);

		// BITMAP 원본 사이즈를 알아내기 위하여 비트맵 정보를 저장할 변수를 선언합니다.
		BITMAP bitmapInfo;
		/// - BITMAP : 로드한 비트맵의 정보를 저장할 때 사용하는 구조체입니다.

		// 로드한 비트맵 데이터에 대한 정보를 구합니다.
		GetObject(myBitmap, sizeof(BITMAP), &bitmapInfo);
		/// GetObject() : GDIOBJ 에 대한 정보를 구합니다.
		/// - 조사할 오브젝트의 핸들
		/// - 기록할 정보릐 크기입니다.
		/// - 오브젝트의 정보를 반환받을 변수에 대한 주소

		// 비트맵을 출력합니다.
		/// Bitblt : 이미지를 화면에 출력합니다.
		/// StretchBlt : 이미지의 크기를 조절하여 화면에 출력합니다.
		/// TransparentBlt : 이미지의 특정한 색상을 제외하고, 크기를 조절하여 화면에 출력합니다.
		

		/*
		BitBlt(
			hdc,				// 이미지를 출력할 위치
			50, 50,				// 이미지를 출력할 왼쪽 상단 좌표
			bitmapInfo.bmWidth, // 원본 이미지에서 잘라올 가로크기
			bitmapInfo.bmHeight,// 원본 이미지에서 잘라올 세로크기
			memDC,              // 이미지의 핸들
			0, 0,				// 이미지의 자르기 시작할 위치 (이 위치부터 cx, cy 만큼 잘라와 화면에 그립니다.)
			SRCCOPY);			// 출력 방법
		/// - SRCCOPY : 원본 이미지를 출력합니다.
		/// - NOTSRCCOPY : 이미지의 색상을 반전시킵니다.
		/// - BLACKNESS : 이미지를 원본의 색상이 아닌, 검정색으로 표시합니다.
		/// - WHITENESS : 이미지를 원본의 색상이 아닌, 하얀색으로 표시합니다.
		*/

		// SetStretchBltMode(HDC hdc, int mode) : 이미지 축소 / 확대 시 발생하는 손실을 보정하는 함수
		/// - hdc : StretchBlt 을 사용할 때 그려지는 DC
		/// - mode : 보정 방식
		///   - BLACKONWHITE : 제거되거나 존재하는 픽셀의 색상값을 사용하여 AND 연산을 수행합니다.
		///   - HALFTONE	 : 픽셀의 평균색상을 원본 픽셀의 색상에 근접하는 수치로 계산합니다.
		///   - WHITEONBLACK : 제거되거나 존재하는 픽셀의 색상값을 사용하여 OR 연산을 수행합니다.
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

		// memDC 가 사용중이던 비트맵 핸들을 해제합니다.
		SelectObject(memDC, oldBitmap);

		// 할당한 DC 를 해제합니다.
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