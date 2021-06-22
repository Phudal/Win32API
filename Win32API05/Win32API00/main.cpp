#include <Windows.h>
#include "WndDefines.h"

#include <iostream>

#pragma comment (linker, "/entry:wWinMainCRTStartup /subsystem:console")

using namespace std;

// Shape Radio
#define IDC_RADIO_SHAPE_RECT		1001
#define IDC_RADIO_SHAPE_ELLIPSE		1002

// Shape Scrollbar
#define IDC_SCROLL_R				1011
#define IDC_SCROLL_G				1012
#define IDC_SCROLL_B				1013

// Rect Edit
#define IDC_RECT_EDIT_RGB			1021

// Ellipse Edit
#define IDC_ELLIPSE_EDIT_RGB		1022


HINSTANCE hInstance;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Edit 컨트롤에 색상값을 입력할 때 사용될 함수
void SetColorEdit(HWND hwnd, int r, int g, int b)
{
	TCHAR colorStr[256];
	wsprintf(colorStr, TEXT("[%d] [%d] [%d]"), r, g, b);
	SetWindowText(hwnd, colorStr);
}

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
	static HWND hsColorR, hsColorG, hsColorB,
		hEditRectRGB, hEditEllipseRGB;

	// 현재 설정중인 도형의 색상 변수를 가리킬 포인터 변수
	static int	rectColorR, rectColorG, rectColorB,
		ellipseColorR, ellipseColorG, ellipseColorB,
		* controlledColorR, * controlledColorG, * controlledColorB;

	switch (msg)
	{
	case WM_CREATE:
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);

		// Create Static
		{
			CreateWindow(TEXT("static"), TEXT("사각형"),
				WS_CHILD | WS_VISIBLE,
				45, 10, 100, 15, hwnd, NULL, hInstance, NULL);

			CreateWindow(TEXT("static"), TEXT("원"),
				WS_CHILD | WS_VISIBLE,
				240, 10, 100, 15, hwnd, NULL, hInstance, NULL);


			CreateWindow(TEXT("static"), TEXT("색상을 설정할 도형을 선택하세요"),
				WS_CHILD | WS_VISIBLE,
				15, 140, 250, 30, hwnd, NULL, hInstance, NULL);
		}

		// Radio Button
		{
			CreateWindow(TEXT("button"), TEXT("RECT"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
				30, 160, 60, 25, hwnd, (HMENU)IDC_RADIO_SHAPE_RECT, hInstance, NULL);

			CreateWindow(TEXT("button"), TEXT("ELLIPSE"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				210, 160, 150, 25, hwnd, (HMENU)IDC_RADIO_SHAPE_ELLIPSE, hInstance, NULL);

			// 처음 선택되도록 할 버튼 설정
			CheckRadioButton(hwnd, IDC_RADIO_SHAPE_RECT, IDC_RADIO_SHAPE_ELLIPSE, IDC_RADIO_SHAPE_RECT);
		}

		// Create Edit
		{
			hEditRectRGB = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER,
				20, 190, 110, 25, hwnd, (HMENU)IDC_RECT_EDIT_RGB, hInstance, NULL);

			hEditEllipseRGB = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER,
				200, 190, 110, 25, hwnd, (HMENU)IDC_ELLIPSE_EDIT_RGB, hInstance, NULL);


		}

		// Create Scrollbar
		{
			hsColorR = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				60, 225, 250, 20, hwnd, (HMENU)IDC_SCROLL_R, hInstance, NULL);
			SetScrollRange(hsColorR, SB_CTL, 0, 255, FALSE);
			SetScrollPos(hsColorR, SB_CTL, 0, FALSE);

			hsColorG = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				60, 250, 250, 20, hwnd, (HMENU)IDC_SCROLL_G, hInstance, NULL);
			SetScrollRange(hsColorG, SB_CTL, 0, 255, FALSE);
			SetScrollPos(hsColorG, SB_CTL, 0, FALSE);

			hsColorB = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				60, 275, 250, 20, hwnd, (HMENU)IDC_SCROLL_B, hInstance, NULL);
			SetScrollRange(hsColorB, SB_CTL, 0, 255, FALSE);
			SetScrollPos(hsColorB, SB_CTL, 0, FALSE);



			// 처음에는 사각형으로 선택되어있으므로 사각형의 색상값을 가리키도록 합니다.
			controlledColorR = &rectColorR;
			controlledColorG = &rectColorG;
			controlledColorB = &rectColorB;

			// Edit 내용 설정
			SetColorEdit(hEditRectRGB, rectColorR, rectColorG, rectColorB);
			SetColorEdit(hEditEllipseRGB, ellipseColorR, ellipseColorG, ellipseColorB);

		}


		return 0;
	}

	case WM_COMMAND:
	{
		// 라디오 버튼 이벤트 처리
		if (LOWORD(wParam) == IDC_RADIO_SHAPE_RECT)
		{
			controlledColorR = &rectColorR;
			controlledColorG = &rectColorG;
			controlledColorB = &rectColorB;
		}
		else if (LOWORD(wParam) == IDC_RADIO_SHAPE_ELLIPSE)
		{
			controlledColorR = &ellipseColorR;
			controlledColorG = &ellipseColorG;
			controlledColorB = &ellipseColorB;
		}

		SetScrollPos(hsColorR, SB_CTL, *controlledColorR, TRUE);
		SetScrollPos(hsColorG, SB_CTL, *controlledColorG, TRUE);
		SetScrollPos(hsColorB, SB_CTL, *controlledColorB, TRUE);

		return 0;
	}

	case WM_HSCROLL:
	{
		// 스크롤 바의 현재 위치 얻습니다.
		int currentPosition = GetScrollPos((HWND)lParam, SB_CTL);

		if (LOWORD(wParam) == SB_LINELEFT)		currentPosition = max(0, currentPosition - 1);
		else if (LOWORD(wParam) == SB_LINERIGHT)	currentPosition = min(255, currentPosition + 1);
		else if (LOWORD(wParam) == SB_PAGELEFT)		currentPosition = max(0, currentPosition - 10);
		else if (LOWORD(wParam) == SB_PAGERIGHT)	currentPosition = min(255, currentPosition + 10);
		else if (LOWORD(wParam) == SB_THUMBTRACK)	currentPosition = HIWORD(wParam);

		// 이벤트가 발생한 핸들에 따라 색상값을 설정합니다.
		if ((HWND)lParam == hsColorR) *controlledColorR = currentPosition;
		else if ((HWND)lParam == hsColorG) *controlledColorG = currentPosition;
		else if ((HWND)lParam == hsColorB) *controlledColorB = currentPosition;

		// 스크롤 바 Thumb 위치 설정
		SetScrollPos((HWND)lParam, SB_CTL, currentPosition, TRUE);

		// Edit 내용 변경
		SetColorEdit(hEditRectRGB, rectColorR, rectColorG, rectColorB);
		SetColorEdit(hEditEllipseRGB, ellipseColorR, ellipseColorG, ellipseColorB);

		// 무효화 영역 생성
		RECT reDrawArea = { 20, 30, 300, 130 };
		InvalidateRect(hwnd, &reDrawArea, FALSE);

		return 0;
	}


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH myBrush = CreateSolidBrush(RGB(rectColorR, rectColorG, rectColorB));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		Rectangle(hdc, 20, 30, 120, 130);
		DeleteObject(SelectObject(hdc, oldBrush));

		myBrush = CreateSolidBrush(RGB(ellipseColorR, ellipseColorG, ellipseColorB));
		oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		Ellipse(hdc, 200, 30, 300, 130);
		DeleteObject(SelectObject(hdc, oldBrush));


		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}