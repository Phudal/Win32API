#include <Windows.h>
#include "WndDefines.h"

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