#include <Windows.h>
#include "WndDefines.h"

// ������ ��Ʈ��
/// - ����ڰ� ������ ���α׷��� �����ϰ� ���� �� ����� �� �ֵ��� �����쿡�� �����ϴ� ����
/// - CreateWindow() �� �̿��Ͽ� ������ �� ������, Ŭ�������� ��Ȯ�ϰ� �ۼ��ؾ� �մϴ�.

// PushButton, CheckBox, RadioButton
/// - Ŭ������ : button
/// - Ŭ���ϰų� üũ�Ͽ� �̺�Ʈ�� �߻���Ű��
///   ������ �� ������, �ƴ����� �����ϴ� �뵵�� ���˴ϴ�.
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

		// Message Queue ���� ���� ������ �޽����� ���ν����� �����մϴ�.
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// üũ�ڽ� �ڵ�
	static HWND hCheck1;

	static BOOL bDrawRect = FALSE;

	// ���� ��ư �ڵ�
	static HWND hRadio1, hRadio2, hRadio3, hRadio4, hRadio5, hRadio6;

	// �׸� ����
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
				TEXT("button"),								// Ŭ������
				TEXT("Hello"),								// ĸ�� ���ڿ�
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,		// ��ư ��Ÿ��
				20, 20,										// ��ư�� ������� ��ġ
				100, 25,									// ��ư�� �ʺ�, ����
				hwnd,										// �θ� ������ �ڵ�
				(HMENU)IDC_BUTTON_1,						// ��Ʈ�� ID
				hInstance,									// ���α׷� �ĺ���
				NULL);										// CREATESTRUCT ����ü�� ���� ������ ��
															// ������ ����� ���� �����Ƿ� NULL �� �����մϴ�.
			// ��ư ��Ÿ�� ����
			/// - BS_PUSHBUTTON			Ǫ�� ��ư
			///
			/// - BS_CHECKBOX			���� üũ �ڽ�
			/// - BS_3STATE				���� 3���� ���� üũ �ڽ�
			/// - BS_AUTOCHECKBOX		�ڵ� üũ �ڽ�
			/// - BS_AUTO3STATE			�ڵ� 3���� ���� üũ �ڽ�
			///
			/// - BS_RADIOBUTTON		���� ���� ��ư
			/// - BS_AUTORADIOBUTTON	�ڵ� ���� ��ư
			/// - BS_GROUPBOX			�׷� �ڽ�

			/// WS_CHILD				�ڽ� ������� �����մϴ�.
			/// WS_VISIBLE				�����츦 �����ϸ� ȭ�鿡 ����մϴ�.
			/// WS_GROUP				���� ��ưó�� �׷��� �̷�� ��Ʈ�ѵ��� �׷� ������ ���˴ϴ�.
			
			// ��ư �̺�Ʈ�� ó���ϴ� ���
			// Ŭ������ ��� WM_COMMAND ���� ó���� �� �ֽ��ϴ�.
		}

		// Check Box
		{
			// üũ�ڽ�
			hCheck1 = CreateWindow(TEXT("button"), TEXT("Draw Ellipse or Rect"),
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				20, 80, 160, 25, hwnd, (HMENU)IDC_CHECKBOX_1, hInstance, NULL);
		}

		// Radio Button
		{
			CreateWindow(TEXT("button"), TEXT("����"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				5, 200, 120, 110, hwnd, NULL, hInstance, NULL);
			CreateWindow(TEXT("button"), TEXT("����"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				145, 200, 120, 110, hwnd, NULL, hInstance, NULL);

			// ���� �׷��� ù ��°���� WS_GROUP�� ����
			hRadio1 = CreateWindow(TEXT("button"), TEXT("�簢��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				10, 215, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON1, hInstance, NULL);
			hRadio2 = CreateWindow(TEXT("button"), TEXT("��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				10, 245, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON2, hInstance, NULL);
			hRadio3 = CreateWindow(TEXT("button"), TEXT("��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				10, 275, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON3, hInstance, NULL);

			hRadio4 = CreateWindow(TEXT("button"), TEXT("����"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				150, 215, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON4, hInstance, NULL);
			hRadio5 = CreateWindow(TEXT("button"), TEXT("����"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				150, 245, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON5, hInstance, NULL);
			hRadio6 = CreateWindow(TEXT("button"), TEXT("�Ķ�"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				150, 275, 100, 30, hwnd, (HMENU)IDC_RADIO_BUTTON6, hInstance, NULL);

			// ù ��° ���� ��ư�� üũ��Ŵ
			CheckRadioButton(hwnd, IDC_RADIO_BUTTON1, IDC_RADIO_BUTTON3, IDC_RADIO_BUTTON1);
			CheckRadioButton(hwnd, IDC_RADIO_BUTTON4, IDC_RADIO_BUTTON6, IDC_RADIO_BUTTON1);
			/// - CheckRadioButton(HWND, hDlg, int firstID, int lastID, int checkID)
			/// - hDlg : ������ �ڵ�
			/// - firstID : ù ��° ���� ��ư
			/// - lastID : ������ ���� ��ư
			/// - checkID : ���ý�ų ���� ��ư
		}

		return 0;
	}

	case WM_COMMAND :
	{
		if (LOWORD(wParam) == IDC_BUTTON_1)
		{
			MessageBox(hwnd, TEXT("���� ���� �ʹ�."), TEXT("���� �����ּ���"), MB_OK);
		}
		else if (LOWORD(wParam) == IDC_CHECKBOX_1)
		{
			// üũ �ڽ��� üũ ���¸� Ȯ���մϴ�.
			/// - üũ�� �Ǿ����� �ʴٸ�
			if (SendMessage(hCheck1, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				/// - SendMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
				///   �޽����� �����쿡�� �����ϴ�.
				///   - hwnd : �޽����� ���� ������ �ڵ�
				///   - msg : ������ �޽���
				///   - wParam : �޽��� �ΰ� ����
				///   - lParam : �޽��� �ΰ� ����

				// üũ�ڽ��� ���´� BM_GETCHECK �� Ȯ���� �� ������,
				// BM_SETCHECK �� ������ �� �ֽ��ϴ�.
				// - Ȯ�� / ���� �� ����� �� �ִ� ��
				//   BST_UNCHECKED	   : üũ �Ǿ����� ����
				//   BST_CHECKED	   : üũ �Ǿ�����
				//   BST_INDETERMINATE : �� �� ���� ( ��Ȱ��ȭ )
				SendMessage(hCheck1, BM_SETCHECK, BST_CHECKED, 0);
				bDrawRect = TRUE;
			}
			// üũ�Ǿ��ִٸ� üũ ����
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

		// üũ�Ǿ��ִٸ� 200, 100, 400, 200 ��ġ�� �簢����,
		// üũ�Ǿ����� �ʴٸ� ���� �׷�������.
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