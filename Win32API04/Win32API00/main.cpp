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


// Edit 컨트롤
/// - 사용자 입력받는 용도로 사용되는 컨트롤
/// - 클래스명 : edit

// ListBox 컨트롤
/// - 박스 내에 여러 항목들을 가지고 보여주는 용도로 사용되는 컨트롤
/// - 클래스명: listbox

// ComboBox 컨트롤
/// - 에디트 컨트롤이 추가된 Listbox
/// - 클래스명 : combobox

// Static 컨트롤
/// - 간단한 문구를 보여주기 위해 사용됨
/// - 클래스명 : static

// Scrollbar 컨트롤
/// - 큰 영역을 창에 모두 표시하지 못할 때 bar를 이용하여 모든 영역을 확인할 수 있도록 할 떄 사용함
/// - 클래스명 : scrollbar

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

		// Message Queue 에서 꺼내 가공한 메시지를 프로시저에 전달합니다.
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

		// edit 컨트롤 생성
		{
			hEdit1 = CreateWindow(TEXT("edit"), TEXT("input..."), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 10, 200, 25, hwnd, (HMENU)IDC_EDIT1, hInstance, NULL);

			hEdit2 = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY | ES_CENTER,
				220, 10, 200, 25, hwnd, (HMENU)IDC_EDIT2, hInstance, NULL);

			// 에디트에서 사용할 수 있는 스타일
			/// - ES_AUTOHSCROLL : 영역을 초과했을 때 자동으로 수평 스크롤 시킵니다.
			/// - ES_AUTOVSCROLL : 영역을 초과했을 때 자동으로 수직 스크롤 시킵니다.
			///                    (보통 ES_MULTILINE 과 함께 사용됩니다.)
			/// - ES_MULTILINE : 여러 줄 편집이 가능하도록 합니다.
			///
			/// - ES_LEFT : 좌측 정렬(기본값)
			/// - ES_CENTER : 중앙 정렬
			/// - ES_RIGHT : 우측 정렬
			/// 
			/// - ES_LOWERCASE : 입력한 영어를 소문자로 변환합니다.
			/// - ES_UPPERCASE : 입력한 영어를 대문자로 변환합니다.
			/// - ES_READONLY : 읽기 전용으로 사용되며 편집이 불가능합니다.
			/// - ES_NUMBER : 숫자만 입력할 수 있도록 합니다.
			/// 
			/// - WS_BORDER : 크기를 조절할 수 없는 단선으로 된 경계선을 추가합니다.
		}

		// listbox 컨트롤 생성
		{
			hListbox = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
				10, 40, 100, 200, hwnd, (HMENU)IDC_LISTBOX, hInstance, NULL);

			/// 리스트 박스 스타일
			/// - LBS_MULTIPLESEL : 기본 값은 하나이나, 여러 항목을 선택할 수 있도록 함
			/// - LBS_SORT : 추가된 항목을 자동으로 정렬
			/// - LBS_NOTIFY : 항목을 선택했을 경우 부모 윈도우에게 통지
			/// - LBS_STANARD : [LBS_MULTIPLESEL | LBS_SORT | LBS_NOTIFY]
					
			// 리스트 박스에 요소 추가
			// _countof : 배열의 길이 반환
			for (int i = 0; i < _countof(Items); i++)
			{
				SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)Items[i]);
				/// - 부모 윈도우가 리스트 박스 컨트롤을 조작할 때 사용할 수 있는 메세지는 다음과 같음
				/// - LB_ADDSTRING : 새로운 항목을 추가
				/// - LB_DELETESTRING : 항목을 제거함
				///   wParam에 제거할 항목의 인덱스를 전달
				///   남은 항목의 개수를 반환
				/// - LB_GETCURSEL : 선택된 항목의 인덱스를 반환
				/// - LB_GETTEXT : 특정한 요소의 문자열을 조사함
				///   wParam 에는 조사할 항목의 인덱스를 전달
				///   lParam 에는 조사할 항목의 문자열을 저장할 문자열을 전달
				/// - LB_GETCOUNT : 항목의 개수를 반환
				/// 
				
				// 리스트 박스가 부모 윈도우에게 전달하는 통지 코드는 다음과 같음
				/// - LBN_DBLCLK : 더블 클릭이 이루어졌을 때
				/// - LBN_KILLFOCUS : 포커스를 잃었을 때
				/// - LBN_SETFOCUS : 포커스를 얻었을 때 
				/// - LBN_SETCHANGE : 사용자가 선택을 변경할 때 
				/// - LBN_SELCANCEL : 사용자가 선택을 취소했을 때
			}
		}

		// combobox 컨트롤 생성
		{
			hCombobox = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN,
				120, 40, 100, 200, hwnd, (HMENU)IDC_COMBOBOX, hInstance, NULL);
			// 콤보 박스 스타일
			/// - CBS_DROPDOWN : edit 기능과 listbox 기능을 가지며, listbox를 펼칠 수 있음
			/// - CBS_SIMPLE : edit 기능과 listbox 기능을 가지며, listbox가 항상 펼쳐져 있음
			/// - CBS_DROPDOWNLIST : listbox 만 가지며, edit에 입력할 수 있음

			for (int i = 0; i < _countof(Items); i++)			
				SendMessage(hCombobox, CB_ADDSTRING, 0, (LPARAM)Items[i]);
		}

		// scrollbar 컨트롤 생성
		{
			hsRed = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				70, 250, 400, 20, hwnd, (HMENU)IDC_SCRRED, hInstance, NULL);

			hsGreen = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				70, 280, 400, 20, hwnd, (HMENU)IDC_SCRGREEN, hInstance, NULL);

			hsBlue = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				70, 310, 400, 20, hwnd, (HMENU)IDC_SCRBLUE, hInstance, NULL);
		
			/// - 스크롤 바 스타일
			/// - SBS_VERT : 수직 [WM_HSCROLL 에서 이벤트를 처리합니다.]
			/// - SBS_HORZ : 수평 [WM_VSCROLL 에서 이벤트를 처리합니다.]
			
			// 스크롤바의 범위를 설정
			/// - SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) : 스크롤바의 범위를 설정
			/// - hwnd : 범위를 설정할 스크롤바 핸들을 전달함
			/// - nBar : 스크롤 바의 형태를 전달함
			///		- SB_HORZ : 수평 스크롤 바
			///		- SB_VERT : 수직 스크롤 바
			///		- SB_CTL : 별도의 스크롤바를 컨트롤
			/// - nMinxPos, nMaxPos : 최소, 최대 값을 전달합니다.
			/// - bRedraw : 범위 설정 후 스크롤 바를 다시 그릴 것인지에 대한 여부를 전달
			SetScrollRange(hsRed, SB_CTL, 0, 255, FALSE);
			SetScrollRange(hsGreen, SB_CTL, 0, 255, FALSE);
			SetScrollRange(hsBlue, SB_CTL, 0, 255, FALSE);

			// 스크롤 바의 초기 위치를 설정
			/// - SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL bRedraw)
			/// - hwnd : 범위를 설ㅈ어할 스크롤바 핸들을 전달
			/// - nBar : 스크롤 바의 형태를 전달
			/// - bRedraw : 범위 설정 후 스크롤 바를 다시 그릴 것인지에 대한 여부를 전달
			SetScrollPos(hsRed, SB_CTL, 0, FALSE);
			SetScrollPos(hsGreen, SB_CTL, 0, FALSE);
			SetScrollPos(hsBlue, SB_CTL, 0, FALSE);
		}

		// static 컨트롤 생성
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
			// 통지 코드를 검사합니다.
			/// - EN_UPDATE    : 문자열을 변경할 때 발생
			/// - EN_CHANGE    : 문자열을 변경 하고 나서 발생
			/// - EN_HSCROLL   : 수평 스크롤바 입력
			/// - EN_VSCROLL   : 수직 스크롤바 입력
			/// - EN_KILLFOCUS : 포커스 잃었을 때
			/// - EN_SETFOCUS  : 포커스 얻었을 때
			/// 포커스 : 선택되어 활성화됨을 의미합니다.

			TCHAR editString[256];

			// 에디트의 문자열이 변경되었다면
			if (HIWORD(wParam) == EN_CHANGE)
			{

				// edit 컨트롤에 입력된 값을 조사합니다.
				GetWindowText(hEdit1, editString, 256);
				/// - GetWindowText(HWND hwnd, LPTSTR lpstring, int nMaxCount)
				///   hwnd 에 전달된 핸들의 캡션을 lpstring 에 저장합니다.
				///   nMaxCount : 저장할 수 있는 최대 길이를 전달합니다.
				// LOG(TEXT("editString : ") << editString);

				SetWindowText(hEdit2, editString);
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				// 에디트 문자열을 저장
				LOG(TEXT("EN_KILLFOCUS"));

				GetWindowText(hEdit1, editString, 256);

				if (lstrlen(editString) == 0)
				{
					// 에디트 내용 상태를 비움으로 설정
					editIsEmpty = TRUE;

					// 힌트 문자열을 표시함
					SetWindowText(hEdit1, TEXT("힌트"));
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
			// 사용자가 항목을 변경했을 때
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				// 선택된 항목의 인덱스를 얻음
				int selectedItemIndex = SendMessage(hListbox, LB_GETCURSEL, 0, 0);

				TCHAR itemString[256];
				// 항목의 문자열을 얻음
				SendMessage(hListbox, LB_GETTEXT, (WPARAM)selectedItemIndex, (LPARAM)itemString);
				SetWindowText(hEdit2, itemString);
			}
		}

		// ComboBox Event
		else if (LOWORD(wParam) == IDC_COMBOBOX)
		{
			// 사용자에 의해 선택된 항목이 변경된다면
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// 선택된 요소의 인덱스를 얻음
				int selectedItemIndex = SendMessage(hCombobox, CB_GETCURSEL, 0, 0);

				// 선택된 텍스트를 얻음
				TCHAR itemString[256];
				SendMessage(hCombobox, CB_GETLBTEXT, selectedItemIndex, (LPARAM)itemString);

				// 에디트 내용 설정
				SetWindowText(hEdit2, itemString);
			}

			// CBN_EDITCHANGE : edit 부분의 문자열이 변경된 경우 발생하는 통지 코드
			else if (HIWORD(wParam) == CBN_EDITCHANGE)
			{
				// 콤보 박스의 문자열을 얻음
				TCHAR itemString[256];
				GetWindowText(hCombobox, itemString, _countof(itemString));

				// 에디트 내용 설정
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

		// 현재 조절 중인 스크롤 바의 위치를 얻음
		/// - lParam : 이벤트가 발생한 스크롤 바의 핸들이 전달됨		
		int currentScrollPosition = GetScrollPos((HWND)lParam, SB_CTL);
		// GetScrollPos(HWND, int nBar) : hwnd에 전달된 스크롤바의 위치를 반환함

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
			// HIWORD(wParam) : 현재 입력한 위치
		}

		// 조절한 스크롤 바의 위치를 설정함
		SetScrollPos((HWND)lParam, SB_CTL, currentScrollPosition, TRUE);

		// 이벤트가 발생한 핸들에 따라 색상을 결정
		if ((HWND)lParam == hsRed)
			colorR = currentScrollPosition;
		else if ((HWND)lParam == hsGreen)
			colorG = currentScrollPosition;
		else if ((HWND)lParam == hsBlue)
			colorB = currentScrollPosition;

		TCHAR colorString[256];
		// 문자열을 합성
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