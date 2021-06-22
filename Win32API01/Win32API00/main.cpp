#include <Windows.h>
#include "WndDefines.h"


// 윈도우 입출력
/// GDI(Graphic Device Interface) : 화면 처리와 그래픽 담당

// DC(Device Context)
/// - 출력에 필요한 모든 정보를 가지는 구조체
/// - 창 내부에서 창을 그릴 영역을 의미함
/// - DC는 가려친 창의 화면을 보관하지 않기 때문에, 그 부분은 개발자가 직접 처리해 주어야 함
///   InvalidateRect() 함수를 호출하여 WM_PAINT 메세지를 발생시켜 그려주거나, 어떠한 예외 처리를 해주어야 함

// InvalidateRect(HWND, LPRECT, BOOL)
/// - 무효화 영역을 지정한 후 이 영역을 처리해 달라는 신호를 설정
///   이때 WM_PAINT의 FLAG를 설정하게 됩니다.
/// - 무효화 영역 :
///   윈도우가 다른 위도우에 의해 가려졌다가 보여지거나 최소화 시킨 후, 최대화를 하는 등의 작업을 수행하면 생기는
///   다시 그려주어야 하는 영역을 의미함
/// - HWND : 갱신되어야 할 영역을 지니고 있는 윈도우의 핸들을 전달
/// - LPRECT(*RECT) : 갱신되어야 하는 영역의 정보를 담고있는 RECT 구조체 변수의 주소를 전달
/// - BOOL : 무효화 영역을 갱신시킬 때 무효화 영역의 배경을 지워야 하는지 여부를 나타내는 변수

// WM_PAINT
/// - Flag 성 메세지
///   (처리되기 위해 Message Queue에 들어가는 것이 아닌, 처리되어야 함을 의미하는 Flag 만을 설정하는 메세지)
/// - Flag 성 메세지는 다른 메세지들에 비해 처리 우선순위가 낮음
/// - 이 메세지가 설정되는 시점은 무효화 영역이 생기는 시점


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
	static RECT printArea = { 250,250,450,450 };
	static TCHAR drawString[256];
	static int endIndex = 0;

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


		drawString[0] = '\0';
		return 0;
	}

	// 문자 키가 눌렸을 경우 발생하는 메세지
	case WM_CHAR:
	{
		endIndex = lstrlen(drawString);

		// 입력 도중 배열 범위를 초과한다면 메세지 박스를 띄움
		if (endIndex + 2 >= sizeof(drawString) / sizeof(drawString[0]))
		{
			MessageBox(hwnd, 
				TEXT("허용할 수 있는 문자열 입력 범위를 넘어섰습니다."),
				TEXT("입력 경고"),
				MB_ICONWARNING | MB_OK);
			/// - MessageBox(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
			///  메세지 박스를 띄웁니다.
			/// - hwnd : 메세지 박스를 소유하는 창의 핸들을 전달
			/// - lpText : 메세지 박스 타잍르 제목을 전달
			/// - lpCaption : 메세지 박스 타이틀 제목을 전달
			/// - uType : 메세지 박스에 어떤 종류의 버튼과 아이콘을 표시할 것인지를 지정
			///		- Icon
			///		- MB_ICONWARNING : 경고 아이콘
			///		- MB_ICONINFORMATION : 정보 아이콘
			///		- MB_ICONQUESTION : 물음표 아이콘  
			///		- MB_ICINSTOP : 위험 아이콘
			/// 
			///		- Button
			///		- MB_OK : 확인 버튼
			///		- MB_ABORTRETRYIGNORE : 중단, 다시시도, 무시 버튼
			///		- MB_OKCANCLE : 확인, 취소 버튼
			///		- MB_RETRYCANCEL : 다시시도, 취소 버튼
			///		- MB_YESNO : 예, 아니오 버튼
			///		- MB_YESNOCANCEL : 예, 아니오, 취소 버튼
			return 0;
		}
		drawString[endIndex] = (TCHAR)wParam;
		drawString[endIndex + 1] = '\0';		

		// printArea 영역을 무효화 영역으로 처리함
		InvalidateRect(hwnd, &printArea, TRUE);
		return 0;
	}
	
	// 마우스 오른쪽 클릭이 발생했을 경우 발생하는 메세지
	case WM_RBUTTONDOWN:
	{
		HDC hdc =  GetDC(hwnd);
		/// - GetDC()
		/// - DC를 가지고 있는 HDC를 생성한 후 반환하는 함수
		/// - ReleaseDC()를 이용하여 반드시 해제해야 함
		
		// 원을 그림
		Ellipse(hdc, 100, 100, 700, 540);
		/// - 간단한 그래픽 출력에 사용되는 함수
		/// - SetPixel(HDC hdc, int x, int y, COLORREF color)
		///   - x, y 우ㅢ치의 픽셀을 color 색상으로 설정합니다ㅓ.
		///     COLORREF : 하나의 색상을 나타내기 위한 형식
		///		RGB(R, G, B) : 각각의 요소들을 0 ~ 255의 값을 가질 수 있음
		/// 
		/// - LineToEx(HDC hdc, int x, iny y) : MovetoEx()함수를 통해 옮긴 위치부터 x, y 위치까지 선을 긋음			
		/// - MovetoEX(HDC hdc, int x, int y, LPPOINT lp) : 그리기 시작할 위치를 x, y 위치로 옮김
		///   lp : 옮기기 전 위치를 저장할 POINT 구조체 변수 주소
		/// 
		/// - Rectangle(HDC hdc,  int left, int top, int right, int bottom) : 사각형을 그림

		// 할당받은 DC를 해제
		ReleaseDC(hwnd, hdc);
		return 0;
	}

	// 무효화 영역이 발생되었을 때 메세지 발생
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		/// PAINTSTURCT : 창의 영역을 그리기 위한 정보를 담은 구조체		
		
		// DC를 할당받음
		HDC hdc = BeginPaint(hwnd, & ps);
		/// - BeginPaint() 함수는 WM_PAINT 에서만 사용 가능
		/// - BeginPaint() 로 할당받은 DC는 EndPaint() 함수를 통해 해제해야 함

		// 원을 그림
		Ellipse(hdc, 100, 100, 700, 540);

		// 사각형을 그림
		Rectangle(hdc, 400, 400, 700, 700);

		// 선을 그리기 시작할 좌표를 (300, 300)으로 설정
		MoveToEx(hdc, 300, 300, NULL);

		// (400, 400) 까지 선을 긋음
		LineTo(hdc, 400, 400);

		// 출력할 문자열을 저장
		LPCTSTR text1 = TEXT("Hello World!!");

		// 문자열을 출력
		TextOut(hdc, 100, 100, text1, lstrlen(text1));
		/// - TextOut(HDC hdc, int x, iny y, LPCTSTR lpString, int c)
		/// - x, y : 그릴 위치
		///   정렬 옵션을 설정하지 않았다면 기본적으로 출력할 문자열 기준 좌측 상단부터 그려지게 됨
		/// - lpString : 출력할 문자열
		/// - c : 출력할 문자열 길이

		// TextOut의 문자열 출력 모드를 설정
		SetTextAlign(hdc, TA_BOTTOM);
		/// - align : 정렬 옵션을 전달함
		///   사용할 수 있는 값들은 다음과 같음
		///   TA_TOP		: 상단으로 정렬
		///   TA_BOTTOM		: 하단으로 정렬
		///   TA_CENTER		: 수평 중앙으로 정렬
		///   TA_LEFT		: 좌측으로 정렬
		///   TA_RIGHT		: 우측으로 정렬
		///   TA_UPDATECP	: 문자열 출력 후 위치가 갱신되도록 함
		///   TA_NOUPDATECP	: 문자열 출력 후 위치가 갱신되지 않도록 함
		/// (여러 옵션을 사용할 경우 비트연산자 '|' 와 함께 사용할 수 있음

		LPCTSTR text2 = TEXT("(100, 100) 을 하단으로 설정하여 문자열을 출력합니다.");
		TextOut(hdc, 100, 100, text2, lstrlen(text2));

		MoveToEx(hdc, 550, 100, NULL);
		SetTextAlign(hdc, TA_UPDATECP | TA_BOTTOM);
		/// - 지금부터 출력시키는 문자열을 설정한 좌표를 하단으로 설정하며
		/// - TextOut의 두 번째 세 번째(x, y)는 정렬 옵션 재설정 전까지 무시함
		LPCTSTR text3 = TEXT("Hello");		
		TextOut(hdc, 0, 0, text3, lstrlen(text3));

		LPCTSTR text4 = TEXT("World");
		TextOut(hdc, 0, 0, text4, lstrlen(text4));

		// 정렬 옵션을 재설정
		SetTextAlign(hdc, TA_NOUPDATECP | TA_TOP | TA_LEFT);
		LPCTSTR text5 = TEXT("Hello World");
		TextOut(hdc, 550, 100, text5, lstrlen(text5));

		// DrawText(HDC hdc, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format)
		// - 특정한 영역 (lprc) 내에서 문자열이 출력되도록 하는 함수
		/// - lpchText : 출력할 문자열
		/// - cchText : 문자열 길이
		/// - lprc : 출력할 영역
		/// - format : 문자열 출력 옵션
		///		DT_LEFT			: 수평 좌측 정렬
		/// 	DT_RIGHT		: 수평 우측 정렬
		/// 	DT_CENTER		: 수평 중앙 정렬
		/// 	DT_BOTTOM		: RECT 영역 하단에 문자열을 출력
		/// 	DT_VCENTER		: RECT 영역 수직 중앙에 문자열을 출력
		/// 	DT_WORDBREAK	: RECT 영역 우측 끝에서 단어 단위로 자동 개행되도록 함		
		///			(단어 구분 단위 : 공백)
		/// 	DT_SINGLELINE	: 한 줄로 출력
		/// 	DT_NOCLIP		: RECT 영역 경계를 벗어나도 문자열을 자르지 않고 그대로 출력
		

		HBRUSH myBrush = CreateSolidBrush(RGB(200, 200, 200));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		Rectangle(hdc, 250, 250, 450, 450);

		DeleteObject(SelectObject(hdc, oldBrush));
		
		DrawText(hdc, drawString, lstrlen(drawString), &printArea, DT_WORDBREAK);

		// 할당받은 DC를 해제함
		EndPaint(hwnd, &ps);

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