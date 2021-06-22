#include <Windows.h>
#include "resource.h"
#include "WndDefines.h"

#define TIMER_ID		0


// 타이머 상태를 나타내기 위한 열거 형식입니다.
typedef enum tag_TimerState { WAIT, RUN, PAUSE } TimerState;

HINSTANCE hInstance;

// 타이머 다이얼로그 핸들
HWND hTimerDlg;

// 설정한 시간을 나타냅니다.
INT setSec;

// 남은 시간을 나타냅니다.
INT remSec;

// 타이머 상태를 나타냅니다.
TimerState timerState;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK TimerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	switch (msg)
	{
	case WM_CREATE:
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);
		return 0;
	}

	case WM_KEYDOWN:
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			// 타이머 다이얼로그가 유효하지 않은지 확인합니다.
			if (!IsWindow(hTimerDlg))
			{
				/// - IsWindow(hWnd) : hWnd 가 NULL 이 아닌지 확인합니다.

				// 타이머 다이얼로그를 생성합니다.
				hTimerDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_TIMER), hwnd, (DLGPROC)TimerProc);
				/// - CreateDialog() : DialogBox() 와 요구하는 인수는 동일하지만,
				///   CreateDialog() 는 창을 만들기만 하고 만든 즉시 핸들을 리턴합니다.
				///   이 함수로 생성한 창은 DestroyWindow() 로 삭제해야 하며
				///   항상 사용된 핸들은 NULL 로 초기화 시키는 것이 좋습니다.

				// 생성한 다이얼로그를 표시합니다.
				ShowWindow(hTimerDlg, SW_SHOW);
			}

		}
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK TimerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		// 대기 상태로 설정
		timerState = WAIT;

		// 스태틱 컨트롤 캡션 설정
		SetDlgItemText(hwnd, IDC_STATIC_REMAIN, TEXT("설정할 시간을 입력하세요."));

		return FALSE;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BUTTON_START)
		{
			// IDC_EDIT_SEC 에 입력된 값을 얻습니다.
			setSec = GetDlgItemInt(hwnd, IDC_EDIT_SEC, NULL, FALSE);

			// 0 이하라면 잘못된 값이므로 메시지 박스를 띄우고 진행을 멈춥니다.
			if (setSec <= 0)
			{
				MessageBox(hwnd, TEXT("시간을 다시 입력해주세요!"), TEXT("WARNING"), MB_OK);
				return FALSE;
			}


			// 실행 상태로 설정
			timerState = RUN;

			// 남은 시간의 값을 초기화합니다.
			remSec = setSec;

			// 스태틱 컨트롤 캡션 설정
			TCHAR staticCaptionString[256];
			wsprintf(staticCaptionString, TEXT("남은 시간 : %d 초"), remSec);
			SetDlgItemText(hwnd, IDC_STATIC_REMAIN, staticCaptionString);

			// 타이머 실행
			/// - 일정 시간동안 반복적인 작업을 할수 있도록 윈도우에서 제공하는 기능
			/// - SetTimer() 함수를 사용하여 생성하며,
			/// - KillTimer() 함수를 사용하여 끝낼 수 있습니다.
			SetTimer(hwnd, TIMER_ID, 1000, NULL);
			/// - SetTimer(hwnd, nIDEvent, uElapse, TimerFunc)
			/// - hwnd  : 타이머를 실행시킬 윈도우 핸들을 전달합니다.
			/// - nIDEvent : 타이머를 구분하기 위한 번호를 전달합니다.
			///              이 값은 KillTimer() 함수로 타이머를 종료시킬 때 사용됩니다.
			/// - uElapse : 반복시킬 시간을 전달합니다.
			/// - TimerFunc : 반복될 때마다 호출될 함수의 포인터를 전달합니다.
			///   만약 NULL 을 전달한다면 WM_TIMER 메시지가 발생하며, 이 곳에서
			///   별도의 함수 정의 없이 이벤트를 처리할 수 있습니다.
		}
		else if (LOWORD(wParam) == IDC_BUTTON_PAUSE)
		{
			// 타이머 상태 검사
			if (timerState == PAUSE)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_BUTTON_PAUSE), TEXT("일시정지"));
				
				timerState = RUN;
				SetTimer(hwnd, TIMER_ID, 1000, NULL);
			}
			else if (timerState == RUN)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_BUTTON_PAUSE), TEXT("재시작"));

				timerState = PAUSE;

				KillTimer(hwnd, TIMER_ID);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON_CLOSE)
		{
			// 대기 상태로 설정
			timerState = WAIT;
			remSec = setSec = 0;

			DestroyWindow(hTimerDlg);
			hTimerDlg = NULL;
		}

		return FALSE;
	}

	case WM_TIMER:
	{
		// 남은 시간 1 감소
		--remSec;


		// 만약 남은 시간이 0 이라면
		if (remSec <= 0)
		{
			// 대기 상태로 설정
			timerState = WAIT;
			remSec = setSec = 0;

			// 스태틱 컨트롤 캡션 설정
			SetDlgItemText(hwnd, IDC_STATIC_REMAIN, TEXT("끝"));

			// 타이머 끝내기
			KillTimer(hwnd, 0);

			// 메시지 박스를 띄웁니다.
			MessageBox(hwnd, TEXT("설정한 시간이 끝났습니다."), TEXT("TIMER FINISHED"), MB_OK);

		}
		else
		{
			// 스태틱 컨트롤 캡션 설정
			TCHAR staticCaptionString[256];
			wsprintf(staticCaptionString, TEXT("남은 시간 : %d 초"), remSec);
			SetDlgItemText(hwnd, IDC_STATIC_REMAIN, staticCaptionString);
		}

		return FALSE;
	}

	case WM_CLOSE:
		// 대기 상태로 설정
		timerState = WAIT;
		remSec = setSec = 0;

		DestroyWindow(hTimerDlg);
		hTimerDlg = NULL;
		return FALSE;
	}

	return FALSE;
}