#include <Windows.h>
#include <iostream>

#include "resource.h"
#include "WndDefines.h"


using namespace std;

#ifdef UNICODE
#define tcout wcout
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#define tcout cout
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define LOG(log) tcout << TEXT("LINE : ") << __LINE__ << TEXT(" : ") << log << endl


// ���̾�α� �ڽ�
/// - ����ڿ��� ������ �����ų� �߰� ������ �����ִ� ��
///   ����ڿ� ��ȭ�� �� �ִ� ���� â�� �ǹ��մϴ�.
/// - ��� ���̾�α�
///   ��ȭ ���ڰ� Ȱ��ȭ �Ǿ��� �� �ٸ� �����츦 ��Ʈ�� �� �� �����ϴ�.
/// - �𵨸��� ���̾�α�
///   ��ȭ ���ڰ� Ȱ��ȭ �Ǿ��� �� �ٸ� �����츦 ��Ʈ�� �� �� �ֽ��ϴ�.

typedef enum tag_Gender { MAN, WOMAN } Gender;
typedef enum tag_Job { FIREFIGHTER, COOK, POLICE } Job;

// ������ ��� ��ġ
INT x;
INT y;

// ������ ������ ����
Gender gender;
Job job;

// ��Ʈ�� �ڵ�
HBITMAP hBitmap0,
		hBitmap1,
		hBitmap2,
		hBitmap3,
		hBitmap4,
		hBitmap5;

HINSTANCE hInstance;

// ���� �ϴܿ� ��� ���ڿ��� ��Ÿ���ϴ�.
TCHAR str[256];

HBITMAP GetJobBmp();
LPCTSTR GetJobName();

// ���̾�α׿��� ����� ���ν����� �����մϴ�.
BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

		// Message Queue ���� ���� ������ �޽����� ���ν����� �����մϴ�.
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE :
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);

		hBitmap0 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmap1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		hBitmap2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		hBitmap3 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		hBitmap4 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		hBitmap5 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));

		lstrcpy(str, GetJobName());

		return 0;
	}

	case WM_KEYDOWN :
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgProc) == IDOK)
			{
				/// - DialogBox() : ��� ���̾�α׸� �����ϴ� ��ũ�� �Լ�
				/// - ���������� CreateWindow() �Լ��� ����ϰ� ������, ���� �� ���ϰ�
				///   â�� ����� �ֵ��� �����ִ� ��ũ�� �Լ��Դϴ�.
				/// - DialogBox() �Լ��� ���� ������� ���̾�α״� EndDialog() �� ���� ������ �� ����.
				InvalidateRect(NULL, NULL, TRUE);
			}
		}

		return 0;
	}

	case WM_PAINT :
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		HDC memDC = CreateCompatibleDC(hdc);

		HBITMAP oldBMP = (HBITMAP)SelectObject(memDC, GetJobBmp());

		BITMAP bmpInfo;
		GetObject(GetJobBmp(), sizeof(BITMAP), &bmpInfo);

		// ��Ʈ�� ���
		BitBlt(hdc, x, y, bmpInfo.bmWidth, bmpInfo.bmHeight, memDC, 0, 0, SRCCOPY);
		
		// ���ڿ� ���
		SetTextAlign(hdc, TA_TOP | TA_CENTER);
		TextOut(hdc,
			x + (bmpInfo.bmWidth * 0.5f),
			y + (bmpInfo.bmHeight) + 10,
			str,
			lstrlen(str));


		SelectObject(memDC, oldBMP);
		DeleteDC(memDC);

		EndPaint(hwnd, &ps);
		return 0;
	}
		

	case WM_DESTROY :

		// ��Ʈ�� ����
		DeleteObject(hBitmap0);
		DeleteObject(hBitmap1);
		DeleteObject(hBitmap2);
		DeleteObject(hBitmap3);
		DeleteObject(hBitmap4);
		DeleteObject(hBitmap5);

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

HBITMAP GetJobBmp()
{
	switch (job)
	{
	case FIREFIGHTER :
		return (gender == MAN) ? hBitmap0 : hBitmap1;
	case COOK :
		return (gender == MAN) ? hBitmap2 : hBitmap3;
	case POLICE :
		return (gender == MAN) ? hBitmap4 : hBitmap5;
	}

	return NULL;
}

LPCTSTR GetJobName()
{
	switch (job)
	{
	case FIREFIGHTER:
		return TEXT("�ҹ��");
	case COOK:
		return TEXT("�丮��");
	case POLICE:
		return TEXT("����");
	}
	return TEXT("");
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	// ���̾�αװ� �޴� ù ��° �޽����Դϴ�.
	/// - �̰��� �ʱ�ȭ�� ���Ǵ� ������ �ۼ��մϴ�.
	case WM_INITDIALOG :
	{
		SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
		SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
		// - SetDlgItemInt(HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned)
		/// - ���̾�α��� ��Ʈ�ѿ� ���� �����͸� �����մϴ�.
		/// - hDlg : ������ ��Ʈ���� �����ϴ� ���̾�α� �ڵ��� �����մϴ�.
		/// - nIDDlgItem : ������ ��Ʈ���� ID �� �����մϴ�.
		/// - uValue : ��Ʈ�ѿ� ������ �������� �����մϴ�.
		///   Ÿ���� UINT �����̳�, bSigned �� ���� ��ȣ�ִ� ������ �� �� �ֽ��ϴ�.
		/// - bSigned : uValue ���� ��ȣ �ִ� ���� �������� ��Ÿ���ϴ�.
		///   �� ���� TRUE �̸�, uValue �� ������� ���� ��ȣ�� ��Ÿ����
		///   FALSE �̸� uValue �� ��ȣ ���� ���� ������ �˴ϴ�.
		
		SetDlgItemText(hwnd, IDC_EDIT_NAME, str);
		/// - ���̾�α��� ��Ʈ�ѿ� ���ڿ� �����͸� �����մϴ�.
		
		// ���� ��ư üũ
		CheckRadioButton(hwnd, IDC_RADIO_FIREFIGHTER, IDC_RADIO_POLICE, IDC_RADIO_FIREFIGHTER + job);

		// üũ �ڽ� üũ
		CheckDlgButton(hwnd, IDC_CHECK_MALE, (gender == MAN) ? BST_CHECKED : BST_UNCHECKED);

		return 0;
	}

	case WM_COMMAND :
	{
		if (LOWORD(wParam) == IDC_BUTTON_SET)
		{
			x = GetDlgItemInt(hwnd, IDC_EDIT_X, NULL, FALSE);
			y = GetDlgItemInt(hwnd, IDC_EDIT_Y, NULL, FALSE);
			/// - GetDlgItemInt(HWND hDlg, int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned)
			/// - ��Ʈ���� �ؽ�Ʈ�� �о� ���������� ��ȯ�� �����մϴ�.
			/// - hDlg : ��Ʈ���� �����ϴ� ���̾�α� �ڵ��� �����մϴ�.
			/// - nIDDlgItem : �о�� ��Ʈ�� ID �� �����մϴ�.
			/// - lpTranslated : ��ȯ�� ���� ���θ� ���Ϲޱ� ���� �Ű� �����̸�,
			///   ���� �ް��� �� �� BOOL ���� ���� �ּҸ� �����մϴ�.
			///   ���� ���� �ʿ䰡 ���ٸ� NULL �� �����մϴ�.
			/// - bSigned : ��ȣ �ִ� �������� �����մϴ�.
			
			LOG(TEXT("x = ") << x);
			LOG(TEXT("y = ") << y);

			// ���� ��ư üũ ���¿� ���� ó���մϴ�.
			if		(IsDlgButtonChecked(hwnd, IDC_RADIO_FIREFIGHTER))	job = FIREFIGHTER;
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO_COOK))			job = COOK;
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO_POLICE))		job = POLICE;

			// üũ �ڽ� üũ ���¿� ���� ó���մϴ�.
			// IDC_CHECK_MALE �̶�� ID �� ���� ��Ʈ���� �ڵ��� ����ϴ�.
			HWND hCheckGender = GetDlgItem(hwnd, IDC_CHECK_MALE);
			gender = (SendMessage(hCheckGender, BM_GETCHECK, 0, 0) == BST_CHECKED) ? MAN : WOMAN;

			GetDlgItemText(hwnd, IDC_EDIT_NAME, str, _countof(str));
			/// - GetDlgItemText() : ��Ʈ���� ���ڿ��� �н��ϴ�.


			EndDialog(hwnd, IDOK);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_CANCEL)
		{

			EndDialog(hwnd, IDCANCEL);
		}

		else if ((LOWORD(wParam) == IDC_RADIO_FIREFIGHTER)	||
				 (LOWORD(wParam) == IDC_RADIO_COOK)			||
				 (LOWORD(wParam) == IDC_RADIO_POLICE))
		{
			if		(IsDlgButtonChecked(hwnd, IDC_RADIO_FIREFIGHTER))	job = FIREFIGHTER;
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO_COOK))			job = COOK;
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO_POLICE))		job = POLICE;

			lstrcpy(str, GetJobName());
			SetDlgItemText(hwnd, IDC_EDIT_NAME, str);
		}

		return 0;
	}

	case WM_CLOSE :
		EndDialog(hwnd, IDCANCEL);
		/// - EndDialog(hDlg, nResult) : ��� ���̾�α׸� �����ŵ�ϴ�.
		/// - hDlg : �����ų ���̾�α� �ڵ�
		/// - nResult : ��ȭ������ ���� �ڵ��̸� �� ���� ��ȭ ���ڸ� ������ �Լ��� ���ϰ����� ���˴ϴ�.

		return FALSE;
	}
	return FALSE;
}