#include <Windows.h>
#include "WndDefines.h"

// 윈도우 컨트롤
/// - 사용자가 윈도우 프로그램을 제어하고 싶을 때 사용할 수 있도록 윈도우에서 제공하는 도구
/// - CreateWindow() 를 이용하여 생성할 수 있으며, 클래스명을 정확하게 작성해야 합니다.

// PushButton, CheckBox, RadioButton
/// - 클래스명 : button
/// - 클릭하거나 체크하여 이벤트를 발생시키고
///   무엇을 할 것인지, 아닌지를 결정하는 용도로 사용됩니다.
#define IDC_BUTTON_1		101
#define IDC_CHECKBOX_1		102

#define IDC_RADIO_BUTTON1	103
#define IDC_RADIO_BUTTON2	104	
#define IDC_RADIO_BUTTON3	105
#define IDC_RADIO_BUTTON4	106
#define IDC_RADIO_BUTTON5	107
#define IDC_RADIO_BUTTON6	108

#define SHAPE_RECTANGLE 0
#define SHAPE_ELLIPSE 1
#define SHAPE_LINE 2
typedef int DRAW_SHAPE;

HINSTANCE	hInstance;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	// 체크박스 핸들
	static HWND hCheck1;

	static BOOL bDrawRect = FALSE;

	// 라디오 버튼 핸들
	static HWND hRadio1, hRadio2, hRadio3, hRadio4, hRadio5, hRadio6;

	// 그릴 영역
	static RECT drawArea = { 300, 210, 500, 310 };
	static COLORREF color = RGB(0, 0, 0);
	static DRAW_SHAPE shape = SHAPE_RECTANGLE;

	switch (msg)
	{
	case WM_CREATE :
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);

		// Push Button
		{
			CreateWindow(
				TEXT("button"),								// 클래스명
				TEXT("Hello"),								// 캡션 문자열
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,		// 버튼 스타일
				20, 20,										// 버튼의 좌측상단 위치
				100, 25,									// 버튼의 너비, 높이
				hwnd,										// 부모 윈도우 핸들
				(HMENU)IDC_BUTTON_1,						// 컨트롤 ID
				hInstance,									// 프로그램 식별자
				NULL);										// CREATESTRUCT 구조체를 통해 전달할 값
															// 지금은 사용할 일이 없으므로 NULL 을 전달합니다.
			// 버튼 스타일 정리
			/// - BS_PUSHBUTTON			푸시 버튼
			///
			/// - BS_CHECKBOX			수동 체크 박스
			/// - BS_3STATE				수동 3가지 상태 체크 박스
			/// - BS_AUTOCHECKBOX		자동 체크 박스
			/// - BS_AUTO3STATE			자동 3가지 상태 체크 박스
			///
			/// - BS_RADIOBUTTON		수동 라디오 버튼
			/// - BS_AUTORADIOBUTTON	자동 라디오 버튼
			/// - BS_GROUPBOX			그룹 박스

			/// WS_CHILD				자식 윈도우로 설정합니다.
			/// WS_VISIBLE				윈도우를 생성하며 화면에 출력합니다.
			/// WS_GROUP				라디오 버튼처럼 그룹을 이루는 컨트롤들의 그룹 지정에 사용됩니다.
			
			// 버튼 이벤트를 처리하는 방법
			// 클릭했을 경우 WM_COMMAND 에서 처리할 수 있습니다.
		}

		// Check Box
		{
			// 체크박스
			hCheck1 = CreateWindow(TEXT("button"), TEXT("Draw Ellipse or Rect"),
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				20, 80, 160, 25, hwnd, (HMENU)IDC_CHECKBOX_1, hInstance, NULL);
		}

		// Radio Button
		{
			CreateWindow(TEXT("button"), TEXT("도형"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				5, 200, 120, 110, hwnd, NULL, hInstance, NULL);
			CreateWindow(TEXT("button"), TEXT("색상"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				145, 200, 120, 110, hwnd, NULL, hInstance, NULL);

			// 라디오 그룹의 첫 번째에는 WS_GROUP을 설정
			hRadio1 = CreateWindow(TEXT("button"), TEXT("사각형"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				10, 215, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON1, hInstance, NULL);
			hRadio2 = CreateWindow(TEXT("button"), TEXT("원"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				10, 245, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON2, hInstance, NULL);
			hRadio3 = CreateWindow(TEXT("button"), TEXT("선"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				10, 275, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON3, hInstance, NULL);

			hRadio4 = CreateWindow(TEXT("button"), TEXT("검정"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				150, 215, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON4, hInstance, NULL);
			hRadio5 = CreateWindow(TEXT("button"), TEXT("빨강"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				150, 245, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON5, hInstance, NULL);
			hRadio6 = CreateWindow(TEXT("button"), TEXT("파랑"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				150, 275, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON6, hInstance, NULL);

			// 첫 번째 라디오 버튼을 체크시킴
			CheckRadioButton(hwnd, IDC_RADIO_BUTTON1, IDC_RADIO_BUTTON3, IDC_RADIO_BUTTON1);
			CheckRadioButton(hwnd, IDC_RADIO_BUTTON4, IDC_RADIO_BUTTON6, IDC_RADIO_BUTTON1);
			/// - CheckRadioButton(HWND, hDlg, int firstID, int lastID, int checkID)
			/// - hDlg : 윈도우 핸들
			/// - firstID : 첫 번째 라디오 버튼
			/// - lastID : 마지막 라디오 버튼
			/// - checkID : 선택시킬 라디오 버튼
		}

		return 0;
	}

	case WM_COMMAND :
	{
		if (LOWORD(wParam) == IDC_BUTTON_1)
		{
			MessageBox(hwnd, TEXT("집에 가고 싶다."), TEXT("집에 보내주세요"), MB_OK);
		}
		else if (LOWORD(wParam) == IDC_CHECKBOX_1)
		{
			// 체크 박스의 체크 상태를 확인합니다.
			/// - 체크가 되어있지 않다면
			if (SendMessage(hCheck1, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				/// - SendMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
				///   메시지를 윈도우에게 보냅니다.
				///   - hwnd : 메시지를 받을 윈도우 핸들
				///   - msg : 전달할 메시지
				///   - wParam : 메시지 부가 정보
				///   - lParam : 메시지 부가 정보

				// 체크박스의 상태는 BM_GETCHECK 로 확인할 수 있으며,
				// BM_SETCHECK 로 설정할 수 있습니다.
				// - 확인 / 설정 시 사용할 수 있는 값
				//   BST_UNCHECKED	   : 체크 되어있지 않음
				//   BST_CHECKED	   : 체크 되어있음
				//   BST_INDETERMINATE : 알 수 없음 ( 비활성화 )
				SendMessage(hCheck1, BM_SETCHECK, BST_CHECKED, 0);
				bDrawRect = TRUE;
			}
			// 체크되어있다면 체크 해제
			else
			{
				SendMessage(hCheck1, BM_SETCHECK, BST_UNCHECKED, 0);
				bDrawRect = FALSE;
			}

			RECT reDrawArea = { 200, 100, 400, 200 };
			InvalidateRect(hwnd, &reDrawArea, TRUE);
		}

		else if (LOWORD(wParam) == IDC_RADIO_BUTTON1)
		{
			shape = SHAPE_RECTANGLE;
			InvalidateRect(hwnd, &drawArea, TRUE);
		}
		else if (LOWORD(wParam) == IDC_RADIO_BUTTON2) 
		{
			shape = SHAPE_ELLIPSE;
			InvalidateRect(hwnd, &drawArea, TRUE);
		}
		else if (LOWORD(wParam) == IDC_RADIO_BUTTON3) 
		{
			shape = SHAPE_LINE;
			InvalidateRect(hwnd, &drawArea, TRUE);
		}
		else if (LOWORD(wParam) == IDC_RADIO_BUTTON4)
		{
			color = RGB(0, 0, 0);
			InvalidateRect(hwnd, &drawArea, TRUE);
		}
		else if (LOWORD(wParam) == IDC_RADIO_BUTTON5)
		{
			color = RGB(255, 0, 0);
			InvalidateRect(hwnd, &drawArea, TRUE);
		}
		else if (LOWORD(wParam) == IDC_RADIO_BUTTON6)
		{
			color = RGB(0, 0, 255);
			InvalidateRect(hwnd, &drawArea, TRUE);
		}
		return 0;
	}

	case WM_PAINT :
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 체크되어있다면 200, 100, 400, 200 위치에 사각형을,
		// 체크되어있지 않다면 원을 그려보세요.
		if (bDrawRect) Rectangle(hdc, 200, 100, 400, 200);
		else Ellipse(hdc, 200, 100, 400, 200);

		//
		{
			HBRUSH myBrush = CreateSolidBrush(color);
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

			HPEN myPen = CreatePen(PS_SOLID, 1, color);
			HPEN oldPen = (HPEN)SelectObject(hdc, myPen);

			switch (shape)
			{
			case SHAPE_RECTANGLE:
				Rectangle(hdc, 300, 210, 500, 310);
				break;
			case SHAPE_ELLIPSE:
				Ellipse(hdc, 300, 210, 500, 310);
				break;
			case SHAPE_LINE:
				MoveToEx(hdc, 300, 210, NULL);
				LineTo(hdc, 500, 310);
				break;
			}

			DeleteObject(SelectObject(hdc, oldBrush));
			DeleteObject(SelectObject(hdc, oldPen));

		}

		EndPaint(hwnd, &ps);

		return 0;
	}

	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}