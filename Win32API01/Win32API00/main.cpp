#include <Windows.h>
#include "WndDefines.h"


// ������ �����
/// GDI(Graphic Device Interface) : ȭ�� ó���� �׷��� ���

// DC(Device Context)
/// - ��¿� �ʿ��� ��� ������ ������ ����ü
/// - â ���ο��� â�� �׸� ������ �ǹ���
/// - DC�� ����ģ â�� ȭ���� �������� �ʱ� ������, �� �κ��� �����ڰ� ���� ó���� �־�� ��
///   InvalidateRect() �Լ��� ȣ���Ͽ� WM_PAINT �޼����� �߻����� �׷��ְų�, ��� ���� ó���� ���־�� ��

// InvalidateRect(HWND, LPRECT, BOOL)
/// - ��ȿȭ ������ ������ �� �� ������ ó���� �޶�� ��ȣ�� ����
///   �̶� WM_PAINT�� FLAG�� �����ϰ� �˴ϴ�.
/// - ��ȿȭ ���� :
///   �����찡 �ٸ� �����쿡 ���� �������ٰ� �������ų� �ּ�ȭ ��Ų ��, �ִ�ȭ�� �ϴ� ���� �۾��� �����ϸ� �����
///   �ٽ� �׷��־�� �ϴ� ������ �ǹ���
/// - HWND : ���ŵǾ�� �� ������ ���ϰ� �ִ� �������� �ڵ��� ����
/// - LPRECT(*RECT) : ���ŵǾ�� �ϴ� ������ ������ ����ִ� RECT ����ü ������ �ּҸ� ����
/// - BOOL : ��ȿȭ ������ ���Ž�ų �� ��ȿȭ ������ ����� ������ �ϴ��� ���θ� ��Ÿ���� ����

// WM_PAINT
/// - Flag �� �޼���
///   (ó���Ǳ� ���� Message Queue�� ���� ���� �ƴ�, ó���Ǿ�� ���� �ǹ��ϴ� Flag ���� �����ϴ� �޼���)
/// - Flag �� �޼����� �ٸ� �޼����鿡 ���� ó�� �켱������ ����
/// - �� �޼����� �����Ǵ� ������ ��ȿȭ ������ ����� ����


// �ʹ� Ȯ��
// ������Ʈ �Ӽ����� ���� ���� - �����ڵ� ���
// ������Ʈ �Ӽ�, ��Ŀ���� �����ý��� - â ���

// �߻��� �޼����� ó���� �Լ��� ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
/// - hwnd : �޼����� ���� ������ �ڵ�
/// - msg : ó���� �޼����� ���޵�
/// - wParam / lParam : �ΰ� ����

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int nCmdShow)
{
	/// - hInstance : ���α׷� �ν��Ͻ� �ڵ�
	/// - hPrevInstance : ���� �����Ų ���� ���α׷��� �ν��Ͻ� �ڵ� 
	/// ���� ���� ���α׷��� ������� �ʾҴٸ� NULL
	/// - lpszCmdParam : ��������� �Էµ� ���α׷� �μ�
	/// - nCmdShow : ���α׷��� ����� ����
	

	// ������ �ڵ�
	HWND hWnd;

	// ������ ����ü ����
	WNDCLASS wndClass;

	
/// - ������ Ŭ���� �ʱ�ȭ
	
	// ������ ��Ÿ�� ����
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// Ư���� �������� ���� ���� ����
	wndClass.cbClsExtra = wndClass.cbWndExtra = 0;

	// ������ ��� ������ ����
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	
	// ���� Ŀ��, �������� ����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	// ������ Ŭ������ ����ϴ� ���α׷��� ��ȣ
	wndClass.hInstance = hInstance;

	// �߻��� �޼����� ó���� �Լ��� ���
	wndClass.lpfnWndProc = WndProc;

	// ������ Ŭ���� �̸� ����
	wndClass.lpszClassName = TEXT("WndClass");
	wndClass.lpszMenuName = NULL;

/// - ������ Ŭ���� ���
	RegisterClass(&wndClass);

/// - ������ â ����
	hWnd = CreateWindow(
		// CreaterWindow()
		// - â�� �����ϰ� ������ â�� �ڵ��� ��ȯ

		// ������ Ŭ���� �̸�
		wndClass.lpszClassName,

		// ������ Ÿ��Ʋ
		WND_TITLE,

		// ����� â�� ��Ÿ��
		WS_OVERLAPPEDWINDOW,

		// â�� ���� ����� ȭ���� ��� ���� ��ġ�ϵ��� �� �������� ����
		CW_USEDEFAULT, CW_USEDEFAULT,

		// â�� �ʺ�� ����
		WND_WIDTH, WND_HEIGHT,

		// �θ� ������ �ڵ�, �޴� �ڵ�
		NULL, NULL,

		// ���α׷� �ĺ���
		wndClass.hInstance,

		// �������� â�� ��� �� �Ķ���͸� ������ ����ü ������ ���� ������
		NULL);

/// - ������ â ���
	ShowWindow(hWnd, nCmdShow);

	MSG msg;

/// - �޼��� ���� ó��
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		// GetMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax)
		// - Message Queue���� �޼����� �������� lpMsg�� ����
		//   �� �� ������ �޼����� Message Queue���� �����ϸ�
		//   ���� ������ �޼����� WM_QUIT �̶�� 0��, �ƴ϶�� 1�� ��ȯ
		// - lpMsg : �޼����� ���� ����
		// - hWnd : �޼����� ���� â
		// - wMsgFilterMin, wMsgFilterMax : �޼��� ���͸��� ���� �Ű� ����
		// Message Queue�� ���� �޼����� ó��
		
		// �߻��� �޼����� ���α׷����� ����� �� �ֵ��� ����
		TranslateMessage(&msg);

		// Message Queue���� ���� ������ �޼����� ���ν����� ����
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static RECT printArea = { 250,250,450,450 };
	static TCHAR drawString[256];
	static int endIndex = 0;

	switch (msg)
	{
		// â�� �����Ǿ��� �� �� ó�� �� �� �߻��ϴ� �޼���
	case WM_CREATE: 
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		// RECT : left, top, right, bottom���� �̷���� ����ü
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);
		// AdjustWindowRect : â�� ũ�⿡ â�� �������� ���Ե��� �ʵ��� �մϴ�.
		// - lpRect : â ���� ũ�⸦ ����
		// - dwStyle : ������ ��Ÿ���� ����
		// - BOOL bMenu : �޴� ��� ���θ� ����


		drawString[0] = '\0';
		return 0;
	}

	// ���� Ű�� ������ ��� �߻��ϴ� �޼���
	case WM_CHAR:
	{
		endIndex = lstrlen(drawString);

		// �Է� ���� �迭 ������ �ʰ��Ѵٸ� �޼��� �ڽ��� ���
		if (endIndex + 2 >= sizeof(drawString) / sizeof(drawString[0]))
		{
			MessageBox(hwnd, 
				TEXT("����� �� �ִ� ���ڿ� �Է� ������ �Ѿ���ϴ�."),
				TEXT("�Է� ���"),
				MB_ICONWARNING | MB_OK);
			/// - MessageBox(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
			///  �޼��� �ڽ��� ���ϴ�.
			/// - hwnd : �޼��� �ڽ��� �����ϴ� â�� �ڵ��� ����
			/// - lpText : �޼��� �ڽ� Ÿ�渣 ������ ����
			/// - lpCaption : �޼��� �ڽ� Ÿ��Ʋ ������ ����
			/// - uType : �޼��� �ڽ��� � ������ ��ư�� �������� ǥ���� �������� ����
			///		- Icon
			///		- MB_ICONWARNING : ��� ������
			///		- MB_ICONINFORMATION : ���� ������
			///		- MB_ICONQUESTION : ����ǥ ������  
			///		- MB_ICINSTOP : ���� ������
			/// 
			///		- Button
			///		- MB_OK : Ȯ�� ��ư
			///		- MB_ABORTRETRYIGNORE : �ߴ�, �ٽýõ�, ���� ��ư
			///		- MB_OKCANCLE : Ȯ��, ��� ��ư
			///		- MB_RETRYCANCEL : �ٽýõ�, ��� ��ư
			///		- MB_YESNO : ��, �ƴϿ� ��ư
			///		- MB_YESNOCANCEL : ��, �ƴϿ�, ��� ��ư
			return 0;
		}
		drawString[endIndex] = (TCHAR)wParam;
		drawString[endIndex + 1] = '\0';		

		// printArea ������ ��ȿȭ �������� ó����
		InvalidateRect(hwnd, &printArea, TRUE);
		return 0;
	}
	
	// ���콺 ������ Ŭ���� �߻����� ��� �߻��ϴ� �޼���
	case WM_RBUTTONDOWN:
	{
		HDC hdc =  GetDC(hwnd);
		/// - GetDC()
		/// - DC�� ������ �ִ� HDC�� ������ �� ��ȯ�ϴ� �Լ�
		/// - ReleaseDC()�� �̿��Ͽ� �ݵ�� �����ؾ� ��
		
		// ���� �׸�
		Ellipse(hdc, 100, 100, 700, 540);
		/// - ������ �׷��� ��¿� ���Ǵ� �Լ�
		/// - SetPixel(HDC hdc, int x, int y, COLORREF color)
		///   - x, y ���ġ�� �ȼ��� color �������� �����մϴ٤�.
		///     COLORREF : �ϳ��� ������ ��Ÿ���� ���� ����
		///		RGB(R, G, B) : ������ ��ҵ��� 0 ~ 255�� ���� ���� �� ����
		/// 
		/// - LineToEx(HDC hdc, int x, iny y) : MovetoEx()�Լ��� ���� �ű� ��ġ���� x, y ��ġ���� ���� ����			
		/// - MovetoEX(HDC hdc, int x, int y, LPPOINT lp) : �׸��� ������ ��ġ�� x, y ��ġ�� �ű�
		///   lp : �ű�� �� ��ġ�� ������ POINT ����ü ���� �ּ�
		/// 
		/// - Rectangle(HDC hdc,  int left, int top, int right, int bottom) : �簢���� �׸�

		// �Ҵ���� DC�� ����
		ReleaseDC(hwnd, hdc);
		return 0;
	}

	// ��ȿȭ ������ �߻��Ǿ��� �� �޼��� �߻�
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		/// PAINTSTURCT : â�� ������ �׸��� ���� ������ ���� ����ü		
		
		// DC�� �Ҵ����
		HDC hdc = BeginPaint(hwnd, & ps);
		/// - BeginPaint() �Լ��� WM_PAINT ������ ��� ����
		/// - BeginPaint() �� �Ҵ���� DC�� EndPaint() �Լ��� ���� �����ؾ� ��

		// ���� �׸�
		Ellipse(hdc, 100, 100, 700, 540);

		// �簢���� �׸�
		Rectangle(hdc, 400, 400, 700, 700);

		// ���� �׸��� ������ ��ǥ�� (300, 300)���� ����
		MoveToEx(hdc, 300, 300, NULL);

		// (400, 400) ���� ���� ����
		LineTo(hdc, 400, 400);

		// ����� ���ڿ��� ����
		LPCTSTR text1 = TEXT("Hello World!!");

		// ���ڿ��� ���
		TextOut(hdc, 100, 100, text1, lstrlen(text1));
		/// - TextOut(HDC hdc, int x, iny y, LPCTSTR lpString, int c)
		/// - x, y : �׸� ��ġ
		///   ���� �ɼ��� �������� �ʾҴٸ� �⺻������ ����� ���ڿ� ���� ���� ��ܺ��� �׷����� ��
		/// - lpString : ����� ���ڿ�
		/// - c : ����� ���ڿ� ����

		// TextOut�� ���ڿ� ��� ��带 ����
		SetTextAlign(hdc, TA_BOTTOM);
		/// - align : ���� �ɼ��� ������
		///   ����� �� �ִ� ������ ������ ����
		///   TA_TOP		: ������� ����
		///   TA_BOTTOM		: �ϴ����� ����
		///   TA_CENTER		: ���� �߾����� ����
		///   TA_LEFT		: �������� ����
		///   TA_RIGHT		: �������� ����
		///   TA_UPDATECP	: ���ڿ� ��� �� ��ġ�� ���ŵǵ��� ��
		///   TA_NOUPDATECP	: ���ڿ� ��� �� ��ġ�� ���ŵ��� �ʵ��� ��
		/// (���� �ɼ��� ����� ��� ��Ʈ������ '|' �� �Բ� ����� �� ����

		LPCTSTR text2 = TEXT("(100, 100) �� �ϴ����� �����Ͽ� ���ڿ��� ����մϴ�.");
		TextOut(hdc, 100, 100, text2, lstrlen(text2));

		MoveToEx(hdc, 550, 100, NULL);
		SetTextAlign(hdc, TA_UPDATECP | TA_BOTTOM);
		/// - ���ݺ��� ��½�Ű�� ���ڿ��� ������ ��ǥ�� �ϴ����� �����ϸ�
		/// - TextOut�� �� ��° �� ��°(x, y)�� ���� �ɼ� �缳�� ������ ������
		LPCTSTR text3 = TEXT("Hello");		
		TextOut(hdc, 0, 0, text3, lstrlen(text3));

		LPCTSTR text4 = TEXT("World");
		TextOut(hdc, 0, 0, text4, lstrlen(text4));

		// ���� �ɼ��� �缳��
		SetTextAlign(hdc, TA_NOUPDATECP | TA_TOP | TA_LEFT);
		LPCTSTR text5 = TEXT("Hello World");
		TextOut(hdc, 550, 100, text5, lstrlen(text5));

		// DrawText(HDC hdc, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format)
		// - Ư���� ���� (lprc) ������ ���ڿ��� ��µǵ��� �ϴ� �Լ�
		/// - lpchText : ����� ���ڿ�
		/// - cchText : ���ڿ� ����
		/// - lprc : ����� ����
		/// - format : ���ڿ� ��� �ɼ�
		///		DT_LEFT			: ���� ���� ����
		/// 	DT_RIGHT		: ���� ���� ����
		/// 	DT_CENTER		: ���� �߾� ����
		/// 	DT_BOTTOM		: RECT ���� �ϴܿ� ���ڿ��� ���
		/// 	DT_VCENTER		: RECT ���� ���� �߾ӿ� ���ڿ��� ���
		/// 	DT_WORDBREAK	: RECT ���� ���� ������ �ܾ� ������ �ڵ� ����ǵ��� ��		
		///			(�ܾ� ���� ���� : ����)
		/// 	DT_SINGLELINE	: �� �ٷ� ���
		/// 	DT_NOCLIP		: RECT ���� ��踦 ����� ���ڿ��� �ڸ��� �ʰ� �״�� ���
		

		HBRUSH myBrush = CreateSolidBrush(RGB(200, 200, 200));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		Rectangle(hdc, 250, 250, 450, 450);

		DeleteObject(SelectObject(hdc, oldBrush));
		
		DrawText(hdc, drawString, lstrlen(drawString), &printArea, DT_WORDBREAK);

		// �Ҵ���� DC�� ������
		EndPaint(hwnd, &ps);

		return 0;
	}

	// â ���� ��û�� �ִٸ� �߻��ϴ� �޼���
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// ��ܿ��� ó������ ���� �޼������� �⺻���� ������� ó���մϴ�.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}