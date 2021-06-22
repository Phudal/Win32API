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


// 다이얼로그 박스
/// - 사용자에게 결정을 내리거나 추가 정보를 보여주는 등
///   사용자와 대화할 수 있는 작은 창을 의미합니다.
/// - 모달 다이얼로그
///   대화 상자가 활성화 되었을 때 다른 윈도우를 컨트롤 할 수 없습니다.
/// - 모델리스 다이얼로그
///   대화 상자가 활성화 되었을 때 다른 윈도우를 컨트롤 할 수 있습니다.

typedef enum tag_Gender { MAN, WOMAN } Gender;
typedef enum tag_Job { FIREFIGHTER, COOK, POLICE } Job;

// 사진을 띄울 위치
INT x;
INT y;

// 설정된 성별과 직업
Gender gender;
Job job;

// 비트맵 핸들
HBITMAP hBitmap0,
		hBitmap1,
		hBitmap2,
		hBitmap3,
		hBitmap4,
		hBitmap5;

HINSTANCE hInstance;

// 사진 하단에 띄울 문자열을 나타냅니다.
TCHAR str[256];

HBITMAP GetJobBmp();
LPCTSTR GetJobName();

// 다이얼로그에서 사용할 프로시저를 선언합니다.
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

		// Message Queue 에서 꺼내 가공한 메시지를 프로시저에 전달합니다.
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
				/// - DialogBox() : 모달 다이얼로그를 생성하는 매크로 함수
				/// - 내부적으로 CreateWindow() 함수를 사용하고 있으며, 조금 더 편하게
				///   창을 만들수 있도록 도와주는 매크로 함수입니다.
				/// - DialogBox() 함수를 통해 만들어진 다이얼로그는 EndDialog() 를 통해 종료할 수 있음.
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

		// 비트맵 출력
		BitBlt(hdc, x, y, bmpInfo.bmWidth, bmpInfo.bmHeight, memDC, 0, 0, SRCCOPY);
		
		// 문자열 출력
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

		// 비트맵 해제
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
		return TEXT("소방관");
	case COOK:
		return TEXT("요리사");
	case POLICE:
		return TEXT("경찰");
	}
	return TEXT("");
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	// 다이얼로그가 받는 첫 번째 메시지입니다.
	/// - 이곳에 초기화에 사용되는 구문을 작성합니다.
	case WM_INITDIALOG :
	{
		SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
		SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
		// - SetDlgItemInt(HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned)
		/// - 다이얼로그의 컨트롤에 정수 데이터를 설정합니다.
		/// - hDlg : 설정할 컨트롤을 소유하는 다이얼로그 핸들을 전달합니다.
		/// - nIDDlgItem : 설정할 컨트롤의 ID 를 전달합니다.
		/// - uValue : 컨트롤에 대입한 정수값을 전달합니다.
		///   타입은 UINT 형식이나, bSigned 에 따라 부호있는 정수가 될 수 있습니다.
		/// - bSigned : uValue 값이 부호 있는 정수 형식임을 나타냅니다.
		///   이 값이 TRUE 이며, uValue 가 음수라면 음수 기호가 나타나고
		///   FALSE 이면 uValue 는 부호 없는 정수 형식이 됩니다.
		
		SetDlgItemText(hwnd, IDC_EDIT_NAME, str);
		/// - 다이얼로그의 컨트롤에 문자열 데이터를 설정합니다.
		
		// 라디오 버튼 체크
		CheckRadioButton(hwnd, IDC_RADIO_FIREFIGHTER, IDC_RADIO_POLICE, IDC_RADIO_FIREFIGHTER + job);

		// 체크 박스 체크
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
			/// - 컨트롤의 텍스트를 읽어 정수형으로 변환해 리턴합니다.
			/// - hDlg : 컨트롤을 소유하는 다이얼로그 핸들을 전달합니다.
			/// - nIDDlgItem : 읽어올 컨트롤 ID 를 전달합니다.
			/// - lpTranslated : 변환의 성공 여부를 리턴받기 위한 매개 변수이며,
			///   리턴 받고자 할 때 BOOL 형식 변수 주소를 전달합니다.
			///   리턴 받을 필요가 없다면 NULL 을 전달합니다.
			/// - bSigned : 부호 있는 정수임을 전달합니다.
			
			LOG(TEXT("x = ") << x);
			LOG(TEXT("y = ") << y);

			// 라디오 버튼 체크 상태에 따라 처리합니다.
			if		(IsDlgButtonChecked(hwnd, IDC_RADIO_FIREFIGHTER))	job = FIREFIGHTER;
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO_COOK))			job = COOK;
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO_POLICE))		job = POLICE;

			// 체크 박스 체크 상태에 따라 처리합니다.
			// IDC_CHECK_MALE 이라는 ID 를 가진 컨트롤의 핸들을 얻습니다.
			HWND hCheckGender = GetDlgItem(hwnd, IDC_CHECK_MALE);
			gender = (SendMessage(hCheckGender, BM_GETCHECK, 0, 0) == BST_CHECKED) ? MAN : WOMAN;

			GetDlgItemText(hwnd, IDC_EDIT_NAME, str, _countof(str));
			/// - GetDlgItemText() : 컨트롤의 문자열을 읽습니다.


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
		/// - EndDialog(hDlg, nResult) : 모달 다이얼로그를 종료시킵니다.
		/// - hDlg : 종료시킬 다이얼로그 핸들
		/// - nResult : 대화상자의 종료 코드이며 이 값은 대화 상자를 생성한 함수의 리턴값으로 사용됩니다.

		return FALSE;
	}
	return FALSE;
}