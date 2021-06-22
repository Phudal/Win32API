#include <Windows.h>
#include <iostream>
#include "WndDefines.h"

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

using namespace std;

#if UNICODE
#define tcout wcout
#else
#define tcout cout
#endif

#define LOG(log) tcout << log << endl;


// Edit ��Ʈ��
/// - ����� �Է¹޴� �뵵�� ���Ǵ� ��Ʈ��
/// - Ŭ������ : edit

// ListBox ��Ʈ��
/// - �ڽ� ���� ���� �׸���� ������ �����ִ� �뵵�� ���Ǵ� ��Ʈ��
/// - Ŭ������: listbox

// ComboBox ��Ʈ��
/// - ����Ʈ ��Ʈ���� �߰��� Listbox
/// - Ŭ������ : combobox

// Static ��Ʈ��
/// - ������ ������ �����ֱ� ���� ����
/// - Ŭ������ : static

// Scrollbar ��Ʈ��
/// - ū ������ â�� ��� ǥ������ ���� �� bar�� �̿��Ͽ� ��� ������ Ȯ���� �� �ֵ��� �� �� �����
/// - Ŭ������ : scrollbar

#define IDC_EDIT1		101
#define IDC_EDIT2		102

#define IDC_LISTBOX		103
#define IDC_COMBOBOX	104

#define IDC_SCRRED		105
#define IDC_SCRGREEN	106
#define IDC_SCRBLUE		107

HINSTANCE hInstance;

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
	static HWND hEdit1, hEdit2;
	static bool editIsEmpty = TRUE;

	static HWND hListbox;
	static const TCHAR* Items[] = {
		TEXT("Apple"),
		TEXT("Orange"),
		TEXT("Melon"),
		TEXT("Grape"),
		TEXT("Strawberry")
	};

	static int colorR, colorG, colorB;

	static HWND hCombobox;

	static HWND hsRed, hsGreen, hsBlue;

	switch (msg)
	{
	case WM_CREATE:
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);

		// edit ��Ʈ�� ����
		{
			hEdit1 = CreateWindow(TEXT("edit"), TEXT("input..."), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 10, 200, 25, hwnd, (HMENU)IDC_EDIT1, hInstance, NULL);

			hEdit2 = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY | ES_CENTER,
				220, 10, 200, 25, hwnd, (HMENU)IDC_EDIT2, hInstance, NULL);

			// ����Ʈ���� ����� �� �ִ� ��Ÿ��
			/// - ES_AUTOHSCROLL : ������ �ʰ����� �� �ڵ����� ���� ��ũ�� ��ŵ�ϴ�.
			/// - ES_AUTOVSCROLL : ������ �ʰ����� �� �ڵ����� ���� ��ũ�� ��ŵ�ϴ�.
			///                    (���� ES_MULTILINE �� �Բ� ���˴ϴ�.)
			/// - ES_MULTILINE : ���� �� ������ �����ϵ��� �մϴ�.
			///
			/// - ES_LEFT : ���� ����(�⺻��)
			/// - ES_CENTER : �߾� ����
			/// - ES_RIGHT : ���� ����
			/// 
			/// - ES_LOWERCASE : �Է��� ��� �ҹ��ڷ� ��ȯ�մϴ�.
			/// - ES_UPPERCASE : �Է��� ��� �빮�ڷ� ��ȯ�մϴ�.
			/// - ES_READONLY : �б� �������� ���Ǹ� ������ �Ұ����մϴ�.
			/// - ES_NUMBER : ���ڸ� �Է��� �� �ֵ��� �մϴ�.
			/// 
			/// - WS_BORDER : ũ�⸦ ������ �� ���� �ܼ����� �� ��輱�� �߰��մϴ�.
		}

		// listbox ��Ʈ�� ����
		{
			hListbox = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
				10, 40, 100, 200, hwnd, (HMENU)IDC_LISTBOX, hInstance, NULL);

			/// ����Ʈ �ڽ� ��Ÿ��
			/// - LBS_MULTIPLESEL : �⺻ ���� �ϳ��̳�, ���� �׸��� ������ �� �ֵ��� ��
			/// - LBS_SORT : �߰��� �׸��� �ڵ����� ����
			/// - LBS_NOTIFY : �׸��� �������� ��� �θ� �����쿡�� ����
			/// - LBS_STANARD : [LBS_MULTIPLESEL | LBS_SORT | LBS_NOTIFY]
					
			// ����Ʈ �ڽ��� ��� �߰�
			// _countof : �迭�� ���� ��ȯ
			for (int i = 0; i < _countof(Items); i++)
			{
				SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)Items[i]);
				/// - �θ� �����찡 ����Ʈ �ڽ� ��Ʈ���� ������ �� ����� �� �ִ� �޼����� ������ ����
				/// - LB_ADDSTRING : ���ο� �׸��� �߰�
				/// - LB_DELETESTRING : �׸��� ������
				///   wParam�� ������ �׸��� �ε����� ����
				///   ���� �׸��� ������ ��ȯ
				/// - LB_GETCURSEL : ���õ� �׸��� �ε����� ��ȯ
				/// - LB_GETTEXT : Ư���� ����� ���ڿ��� ������
				///   wParam ���� ������ �׸��� �ε����� ����
				///   lParam ���� ������ �׸��� ���ڿ��� ������ ���ڿ��� ����
				/// - LB_GETCOUNT : �׸��� ������ ��ȯ
				/// 
				
				// ����Ʈ �ڽ��� �θ� �����쿡�� �����ϴ� ���� �ڵ�� ������ ����
				/// - LBN_DBLCLK : ���� Ŭ���� �̷������ ��
				/// - LBN_KILLFOCUS : ��Ŀ���� �Ҿ��� ��
				/// - LBN_SETFOCUS : ��Ŀ���� ����� �� 
				/// - LBN_SETCHANGE : ����ڰ� ������ ������ �� 
				/// - LBN_SELCANCEL : ����ڰ� ������ ������� ��
			}
		}

		// combobox ��Ʈ�� ����
		{
			hCombobox = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN,
				120, 40, 100, 200, hwnd, (HMENU)IDC_COMBOBOX, hInstance, NULL);
			// �޺� �ڽ� ��Ÿ��
			/// - CBS_DROPDOWN : edit ��ɰ� listbox ����� ������, listbox�� ��ĥ �� ����
			/// - CBS_SIMPLE : edit ��ɰ� listbox ����� ������, listbox�� �׻� ������ ����
			/// - CBS_DROPDOWNLIST : listbox �� ������, edit�� �Է��� �� ����

			for (int i = 0; i < _countof(Items); i++)			
				SendMessage(hCombobox, CB_ADDSTRING, 0, (LPARAM)Items[i]);
		}

		// scrollbar ��Ʈ�� ����
		{
			hsRed = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				70, 250, 400, 20, hwnd, (HMENU)IDC_SCRRED, hInstance, NULL);

			hsGreen = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				70, 280, 400, 20, hwnd, (HMENU)IDC_SCRGREEN, hInstance, NULL);

			hsBlue = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				70, 310, 400, 20, hwnd, (HMENU)IDC_SCRBLUE, hInstance, NULL);
		
			/// - ��ũ�� �� ��Ÿ��
			/// - SBS_VERT : ���� [WM_HSCROLL ���� �̺�Ʈ�� ó���մϴ�.]
			/// - SBS_HORZ : ���� [WM_VSCROLL ���� �̺�Ʈ�� ó���մϴ�.]
			
			// ��ũ�ѹ��� ������ ����
			/// - SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) : ��ũ�ѹ��� ������ ����
			/// - hwnd : ������ ������ ��ũ�ѹ� �ڵ��� ������
			/// - nBar : ��ũ�� ���� ���¸� ������
			///		- SB_HORZ : ���� ��ũ�� ��
			///		- SB_VERT : ���� ��ũ�� ��
			///		- SB_CTL : ������ ��ũ�ѹٸ� ��Ʈ��
			/// - nMinxPos, nMaxPos : �ּ�, �ִ� ���� �����մϴ�.
			/// - bRedraw : ���� ���� �� ��ũ�� �ٸ� �ٽ� �׸� �������� ���� ���θ� ����
			SetScrollRange(hsRed, SB_CTL, 0, 255, FALSE);
			SetScrollRange(hsGreen, SB_CTL, 0, 255, FALSE);
			SetScrollRange(hsBlue, SB_CTL, 0, 255, FALSE);

			// ��ũ�� ���� �ʱ� ��ġ�� ����
			/// - SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL bRedraw)
			/// - hwnd : ������ �������� ��ũ�ѹ� �ڵ��� ����
			/// - nBar : ��ũ�� ���� ���¸� ����
			/// - bRedraw : ���� ���� �� ��ũ�� �ٸ� �ٽ� �׸� �������� ���� ���θ� ����
			SetScrollPos(hsRed, SB_CTL, 0, FALSE);
			SetScrollPos(hsGreen, SB_CTL, 0, FALSE);
			SetScrollPos(hsBlue, SB_CTL, 0, FALSE);
		}

		// static ��Ʈ�� ����
		{
			CreateWindow(TEXT("static"), TEXT("R"), WS_CHILD | WS_VISIBLE, 50, 250, 20, 25, hwnd, NULL, hInstance, NULL);
			CreateWindow(TEXT("static"), TEXT("G"), WS_CHILD | WS_VISIBLE, 50, 280, 20, 25, hwnd, NULL, hInstance, NULL);
			CreateWindow(TEXT("static"), TEXT("B"), WS_CHILD | WS_VISIBLE, 50, 310, 20, 25, hwnd, NULL, hInstance, NULL);
		}
	}

	case WM_COMMAND:
	{
		// Edit Event
		if (LOWORD(wParam) == IDC_EDIT1)
		{
			// ���� �ڵ带 �˻��մϴ�.
			/// - EN_UPDATE    : ���ڿ��� ������ �� �߻�
			/// - EN_CHANGE    : ���ڿ��� ���� �ϰ� ���� �߻�
			/// - EN_HSCROLL   : ���� ��ũ�ѹ� �Է�
			/// - EN_VSCROLL   : ���� ��ũ�ѹ� �Է�
			/// - EN_KILLFOCUS : ��Ŀ�� �Ҿ��� ��
			/// - EN_SETFOCUS  : ��Ŀ�� ����� ��
			/// ��Ŀ�� : ���õǾ� Ȱ��ȭ���� �ǹ��մϴ�.

			TCHAR editString[256];

			// ����Ʈ�� ���ڿ��� ����Ǿ��ٸ�
			if (HIWORD(wParam) == EN_CHANGE)
			{

				// edit ��Ʈ�ѿ� �Էµ� ���� �����մϴ�.
				GetWindowText(hEdit1, editString, 256);
				/// - GetWindowText(HWND hwnd, LPTSTR lpstring, int nMaxCount)
				///   hwnd �� ���޵� �ڵ��� ĸ���� lpstring �� �����մϴ�.
				///   nMaxCount : ������ �� �ִ� �ִ� ���̸� �����մϴ�.
				// LOG(TEXT("editString : ") << editString);

				SetWindowText(hEdit2, editString);
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				// ����Ʈ ���ڿ��� ����
				LOG(TEXT("EN_KILLFOCUS"));

				GetWindowText(hEdit1, editString, 256);

				if (lstrlen(editString) == 0)
				{
					// ����Ʈ ���� ���¸� ������� ����
					editIsEmpty = TRUE;

					// ��Ʈ ���ڿ��� ǥ����
					SetWindowText(hEdit1, TEXT("��Ʈ"));
				}
				else
				{
					editIsEmpty = FALSE;
				}

			}
			else if (HIWORD(wParam) == EN_SETFOCUS)
			{
				LOG(TEXT("EN_SETFOCUS"));

				if (editIsEmpty)
					SetWindowText(hEdit1, NULL);
			}


		}

		// ListBox Event
		else if (LOWORD(wParam) == IDC_LISTBOX)
		{
			// ����ڰ� �׸��� �������� ��
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				// ���õ� �׸��� �ε����� ����
				int selectedItemIndex = SendMessage(hListbox, LB_GETCURSEL, 0, 0);

				TCHAR itemString[256];
				// �׸��� ���ڿ��� ����
				SendMessage(hListbox, LB_GETTEXT, (WPARAM)selectedItemIndex, (LPARAM)itemString);
				SetWindowText(hEdit2, itemString);
			}
		}

		// ComboBox Event
		else if (LOWORD(wParam) == IDC_COMBOBOX)
		{
			// ����ڿ� ���� ���õ� �׸��� ����ȴٸ�
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// ���õ� ����� �ε����� ����
				int selectedItemIndex = SendMessage(hCombobox, CB_GETCURSEL, 0, 0);

				// ���õ� �ؽ�Ʈ�� ����
				TCHAR itemString[256];
				SendMessage(hCombobox, CB_GETLBTEXT, selectedItemIndex, (LPARAM)itemString);

				// ����Ʈ ���� ����
				SetWindowText(hEdit2, itemString);
			}

			// CBN_EDITCHANGE : edit �κ��� ���ڿ��� ����� ��� �߻��ϴ� ���� �ڵ�
			else if (HIWORD(wParam) == CBN_EDITCHANGE)
			{
				// �޺� �ڽ��� ���ڿ��� ����
				TCHAR itemString[256];
				GetWindowText(hCombobox, itemString, _countof(itemString));

				// ����Ʈ ���� ����
				SetWindowText(hEdit2, itemString);
			}
		}

		return 0;
	}

	case WM_HSCROLL:
	{
		// SB_LINELEFT
		// SB_LINERIGHT
		// SB_PAGELEFT
		// SB_PAGERIGHT
		// SB_THUMBTRACK

		// ���� ���� ���� ��ũ�� ���� ��ġ�� ����
		/// - lParam : �̺�Ʈ�� �߻��� ��ũ�� ���� �ڵ��� ���޵�		
		int currentScrollPosition = GetScrollPos((HWND)lParam, SB_CTL);
		// GetScrollPos(HWND, int nBar) : hwnd�� ���޵� ��ũ�ѹ��� ��ġ�� ��ȯ��

		if (LOWORD(wParam) == SB_LINELEFT)
		{
			currentScrollPosition = max(0, currentScrollPosition - 1);			
		}
		else if (LOWORD(wParam) == SB_LINERIGHT)
		{
			currentScrollPosition = min(255, currentScrollPosition + 1);			
		}
		else if (LOWORD(wParam) == SB_PAGELEFT)
		{
			currentScrollPosition = max(0, currentScrollPosition - 10);
		}
		else if (LOWORD(wParam) == SB_PAGERIGHT)
		{
			currentScrollPosition = min(255, currentScrollPosition + 10);
		}
		else if (LOWORD(wParam) == SB_THUMBTRACK)
		{
			currentScrollPosition = HIWORD(wParam);
			// HIWORD(wParam) : ���� �Է��� ��ġ
		}

		// ������ ��ũ�� ���� ��ġ�� ������
		SetScrollPos((HWND)lParam, SB_CTL, currentScrollPosition, TRUE);

		// �̺�Ʈ�� �߻��� �ڵ鿡 ���� ������ ����
		if ((HWND)lParam == hsRed)
			colorR = currentScrollPosition;
		else if ((HWND)lParam == hsGreen)
			colorG = currentScrollPosition;
		else if ((HWND)lParam == hsBlue)
			colorB = currentScrollPosition;

		TCHAR colorString[256];
		// ���ڿ��� �ռ�
		wsprintf(colorString, TEXT("R[%d] G[%d] B[%d]"), colorR, colorG, colorB);
		SetWindowText(hEdit2, colorString);

		RECT rc = { 70, 340, 470, 540 };
		InvalidateRect(hwnd, &rc, FALSE);

		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH myBrush = CreateSolidBrush(RGB(colorR, colorG, colorB));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

		Rectangle(hdc, 70, 340, 470, 540);

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