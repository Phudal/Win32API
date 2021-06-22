#include <Windows.h>
#include "resource.h"
#include "WndDefines.h"

#define TIMER_ID		0


// Ÿ�̸� ���¸� ��Ÿ���� ���� ���� �����Դϴ�.
typedef enum tag_TimerState { WAIT, RUN, PAUSE } TimerState;

HINSTANCE hInstance;

// Ÿ�̸� ���̾�α� �ڵ�
HWND hTimerDlg;

// ������ �ð��� ��Ÿ���ϴ�.
INT setSec;

// ���� �ð��� ��Ÿ���ϴ�.
INT remSec;

// Ÿ�̸� ���¸� ��Ÿ���ϴ�.
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

		// Message Queue ���� ���� ������ �޽����� ���ν����� �����մϴ�.
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
			// Ÿ�̸� ���̾�αװ� ��ȿ���� ������ Ȯ���մϴ�.
			if (!IsWindow(hTimerDlg))
			{
				/// - IsWindow(hWnd) : hWnd �� NULL �� �ƴ��� Ȯ���մϴ�.

				// Ÿ�̸� ���̾�α׸� �����մϴ�.
				hTimerDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_TIMER), hwnd, (DLGPROC)TimerProc);
				/// - CreateDialog() : DialogBox() �� �䱸�ϴ� �μ��� ����������,
				///   CreateDialog() �� â�� ����⸸ �ϰ� ���� ��� �ڵ��� �����մϴ�.
				///   �� �Լ��� ������ â�� DestroyWindow() �� �����ؾ� �ϸ�
				///   �׻� ���� �ڵ��� NULL �� �ʱ�ȭ ��Ű�� ���� �����ϴ�.

				// ������ ���̾�α׸� ǥ���մϴ�.
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
		// ��� ���·� ����
		timerState = WAIT;

		// ����ƽ ��Ʈ�� ĸ�� ����
		SetDlgItemText(hwnd, IDC_STATIC_REMAIN, TEXT("������ �ð��� �Է��ϼ���."));

		return FALSE;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BUTTON_START)
		{
			// IDC_EDIT_SEC �� �Էµ� ���� ����ϴ�.
			setSec = GetDlgItemInt(hwnd, IDC_EDIT_SEC, NULL, FALSE);

			// 0 ���϶�� �߸��� ���̹Ƿ� �޽��� �ڽ��� ���� ������ ����ϴ�.
			if (setSec <= 0)
			{
				MessageBox(hwnd, TEXT("�ð��� �ٽ� �Է����ּ���!"), TEXT("WARNING"), MB_OK);
				return FALSE;
			}


			// ���� ���·� ����
			timerState = RUN;

			// ���� �ð��� ���� �ʱ�ȭ�մϴ�.
			remSec = setSec;

			// ����ƽ ��Ʈ�� ĸ�� ����
			TCHAR staticCaptionString[256];
			wsprintf(staticCaptionString, TEXT("���� �ð� : %d ��"), remSec);
			SetDlgItemText(hwnd, IDC_STATIC_REMAIN, staticCaptionString);

			// Ÿ�̸� ����
			/// - ���� �ð����� �ݺ����� �۾��� �Ҽ� �ֵ��� �����쿡�� �����ϴ� ���
			/// - SetTimer() �Լ��� ����Ͽ� �����ϸ�,
			/// - KillTimer() �Լ��� ����Ͽ� ���� �� �ֽ��ϴ�.
			SetTimer(hwnd, TIMER_ID, 1000, NULL);
			/// - SetTimer(hwnd, nIDEvent, uElapse, TimerFunc)
			/// - hwnd  : Ÿ�̸Ӹ� �����ų ������ �ڵ��� �����մϴ�.
			/// - nIDEvent : Ÿ�̸Ӹ� �����ϱ� ���� ��ȣ�� �����մϴ�.
			///              �� ���� KillTimer() �Լ��� Ÿ�̸Ӹ� �����ų �� ���˴ϴ�.
			/// - uElapse : �ݺ���ų �ð��� �����մϴ�.
			/// - TimerFunc : �ݺ��� ������ ȣ��� �Լ��� �����͸� �����մϴ�.
			///   ���� NULL �� �����Ѵٸ� WM_TIMER �޽����� �߻��ϸ�, �� ������
			///   ������ �Լ� ���� ���� �̺�Ʈ�� ó���� �� �ֽ��ϴ�.
		}
		else if (LOWORD(wParam) == IDC_BUTTON_PAUSE)
		{
			// Ÿ�̸� ���� �˻�
			if (timerState == PAUSE)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_BUTTON_PAUSE), TEXT("�Ͻ�����"));
				
				timerState = RUN;
				SetTimer(hwnd, TIMER_ID, 1000, NULL);
			}
			else if (timerState == RUN)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_BUTTON_PAUSE), TEXT("�����"));

				timerState = PAUSE;

				KillTimer(hwnd, TIMER_ID);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON_CLOSE)
		{
			// ��� ���·� ����
			timerState = WAIT;
			remSec = setSec = 0;

			DestroyWindow(hTimerDlg);
			hTimerDlg = NULL;
		}

		return FALSE;
	}

	case WM_TIMER:
	{
		// ���� �ð� 1 ����
		--remSec;


		// ���� ���� �ð��� 0 �̶��
		if (remSec <= 0)
		{
			// ��� ���·� ����
			timerState = WAIT;
			remSec = setSec = 0;

			// ����ƽ ��Ʈ�� ĸ�� ����
			SetDlgItemText(hwnd, IDC_STATIC_REMAIN, TEXT("��"));

			// Ÿ�̸� ������
			KillTimer(hwnd, 0);

			// �޽��� �ڽ��� ���ϴ�.
			MessageBox(hwnd, TEXT("������ �ð��� �������ϴ�."), TEXT("TIMER FINISHED"), MB_OK);

		}
		else
		{
			// ����ƽ ��Ʈ�� ĸ�� ����
			TCHAR staticCaptionString[256];
			wsprintf(staticCaptionString, TEXT("���� �ð� : %d ��"), remSec);
			SetDlgItemText(hwnd, IDC_STATIC_REMAIN, staticCaptionString);
		}

		return FALSE;
	}

	case WM_CLOSE:
		// ��� ���·� ����
		timerState = WAIT;
		remSec = setSec = 0;

		DestroyWindow(hTimerDlg);
		hTimerDlg = NULL;
		return FALSE;
	}

	return FALSE;
}