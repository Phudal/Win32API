#include <Windows.h>
#include "WndDefines.h"

// 초반 확인
// 프로젝트 속성에서 문자 집합 - 유니코드 사용
// 프로젝트 속성, 링커에서 하위시스템 - 창 사용

// 발생한 메세지를 처리할 함수를 선언
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
/// - hwnd : 메세지를 받을 윈도우 핸들
/// - msg : 처리할 메세지가 전달됨
/// - wParam / lParam : 부가 정보

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int nCmdShow)
{
	/// - hInstance : 프로그램 인스턴스 핸들
	/// - hPrevInstance : 전에 실행시킨 같은 프로그램의 인스턴스 핸들 
	/// 전에 같은 프로그램이 실행되지 않았다면 NULL
	/// - lpszCmdParam : 명령행으로 입력된 프로그램 인수
	/// - nCmdShow : 프로그램이 실행될 형태
	

	// 윈도우 핸들
	HWND hWnd;

	// 윈도우 구조체 변수
	WNDCLASS wndClass;

	
/// - 윈도우 클래스 초기화
	
	// 윈도우 스타일 정의
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 특수한 목적으로 사용될 예약 영역
	wndClass.cbClsExtra = wndClass.cbWndExtra = 0;

	// 윈도우 배경 색상을 설정
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	
	// 사용될 커서, 아이콘을 지정
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	// 윈도우 클래스를 등록하는 프로그램의 번호
	wndClass.hInstance = hInstance;

	// 발생한 메세지를 처리할 함수를 등록
	wndClass.lpfnWndProc = WndProc;

	// 윈도우 클래스 이름 지정
	wndClass.lpszClassName = TEXT("WndClass");
	wndClass.lpszMenuName = NULL;

/// - 윈도우 클래스 등록
	RegisterClass(&wndClass);

/// - 윈도우 창 생성
	hWnd = CreateWindow(
		// CreaterWindow()
		// - 창을 생성하고 생성된 창의 핸들을 반환

		// 윈도우 클래스 이름
		wndClass.lpszClassName,

		// 윈도우 타이틀
		WND_TITLE,

		// 띄워진 창의 스타일
		WS_OVERLAPPEDWINDOW,

		// 창의 왼쪽 상단이 화면의 어느 곳에 위치하도록 할 것인지를 지정
		CW_USEDEFAULT, CW_USEDEFAULT,

		// 창의 너비와 높이
		WND_WIDTH, WND_HEIGHT,

		// 부모 윈도우 핸들, 메뉴 핸들
		NULL, NULL,

		// 프로그램 식별자
		wndClass.hInstance,

		// 여러개의 창을 띄울 때 파라미터를 전달할 구조체 변수에 대한 포인터
		NULL);

/// - 윈도우 창 출력
	ShowWindow(hWnd, nCmdShow);

	MSG msg;

/// - 메세지 루프 처리
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		// GetMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax)
		// - Message Queue에서 메세지를 가져오고 lpMsg에 저장
		//   그 후 가져온 메세지를 Message Queue에서 제거하며
		//   만약 가져온 메세지가 WM_QUIT 이라면 0을, 아니라면 1을 반환
		// - lpMsg : 메세지를 받을 변수
		// - hWnd : 메세지를 받을 창
		// - wMsgFilterMin, wMsgFilterMax : 메세지 필터링을 위한 매개 변수
		// Message Queue에 들어온 메세지를 처리
		
		// 발생한 메세지를 프로그램에서 사용할 수 있도록 가공
		TranslateMessage(&msg);

		// Message Queue에서 꺼내 가공한 메세지를 프로시저에 전달
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// 창이 생성되었을 때 맨 처음 한 번 발생하는 메세지
	case WM_CREATE: 
	{
		RECT wndRC = { 0, 0, WND_WIDTH, WND_HEIGHT };
		// RECT : left, top, right, bottom으로 이루어진 구조체
		AdjustWindowRect(&wndRC, WS_OVERLAPPEDWINDOW, FALSE);
		// AdjustWindowRect : 창의 크기에 창의 프레임이 포함되지 않도록 합니다.
		// - lpRect : 창 영역 크기를 전달
		// - dwStyle : 윈도우 스타일을 지정
		// - BOOL bMenu : 메뉴 사용 여부를 전달
		return 0;
	}
		// 창 종료 요청이 있다면 발생하는 메세지
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// 상단에서 처리하지 않은 메세지들을 기본적인 방식으로 처리합니다.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}