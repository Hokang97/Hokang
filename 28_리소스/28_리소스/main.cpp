#include <Windows.h>
#include<stdio.h>
#include "resource.h"
#pragma warning(disable : 4996)
#define IDC_EDIT_TEXT 110
HWND g_hEdit;
// 함수 호출 방법
// __stdcall : 매개변수의 개수가 고정된 함수    / 크기 작다  / callee (피호출자) stack 정리
// __cdecl   : 매개변수의 개수가 가변적인 함수  / 크기 크다  / caller (호출자)   stack 정리

HCURSOR g_hCur1, g_hCur2, g_hCur3, g_hNow;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Msg;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	// 1. 윈도우 클래스 설정/초기화
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;	// 특수한 목적으로 사용될 예약 영역
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;		// 윈도우 클래스를 등록하는 프로그램의 번호
	WndClass.lpfnWndProc = WndProc;		// 프로시저 설정
	WndClass.lpszClassName = TEXT("WndClass");
	WndClass.lpszMenuName = NULL;

	// 2. 윈도우 클래스 등록
	RegisterClass(&WndClass);

	// 3. 윈도우 창 생성
	hWnd = CreateWindow(WndClass.lpszClassName, TEXT("윈도우타이틀"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 640, NULL, NULL, hInstance, NULL);


	// 4. 윈도우 창 활성화/표시
	ShowWindow(hWnd, SW_SHOW);

	// 5. 메세지 루프/처리
	// GetMessage()
	// PeekMessage()

	HACCEL hAccel = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &Msg)) {

			TranslateMessage(&Msg);		// 키보드 입력 메세지를 프로그램에서 사용할 수 있도록 가공
			DispatchMessage(&Msg);		// 메세지 큐에서 꺼낸 메세지를 프로시저에 전달
		}
	}

	return (int)Msg.wParam;





}

// 콜백함수 매개변수
// hwnd - 메세지를 받을 윈도우 핸들러
// msg	- 어떤 종류의 메세지인지 확일할 수 있는 값
// wParam	- 메세지의 부가 정보 (메세지에 따라 다름)
// lParam	- 메세지의 부가 정보 (메세지에 따라 다름)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR title[128];
	
	FILE* fp;
	TCHAR str[128];
	switch (msg)
	{
	case WM_CREATE:
		LoadString(g_hInst, IDS_GAME_TITLE, title, 128);	// 문자열 불러오기
		SetWindowText(hwnd, title);		// 문자열 타이틀 지정

		g_hCur1 = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_CURSOR1));
		g_hCur2 = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_CURSOR2));
		g_hCur3 = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_CURSOR3));

		g_hNow = g_hCur1;

		g_hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 10, 220, 25, hwnd, (HMENU)IDC_EDIT_TEXT, g_hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("불러오기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 40, 100, 25, hwnd, (HMENU)0, g_hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("저장하기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			120, 40, 100, 25, hwnd, (HMENU)1, g_hInst, NULL);
		return 0;

	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(g_hNow);
		}
		return 0;

	case WM_LBUTTONDOWN:
		if (g_hNow == g_hCur1) g_hNow = g_hCur2;
		else if (g_hNow == g_hCur2) g_hNow = g_hCur3;
		else g_hNow = g_hCur1;
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
			fp = fopen("output.txt", "rt");
			fwscanf(fp, TEXT("%s%"), str);
			SetDlgItemText(hwnd, IDC_EDIT_TEXT, str);
			fclose(fp);
			MessageBox(hwnd, TEXT("텍스트를 불러왔"), TEXT("알림"), MB_OK);

			break;

		case 1:
			fp = fopen("output.txt", "wt");
			GetDlgItemText(hwnd, IDC_EDIT_TEXT, str, 128);
			fwprintf(fp, TEXT("%s"), str);
			fclose(fp);
			MessageBox(hwnd, TEXT("텍스트를 저장했습니다"), TEXT("알림"), MB_OK);
			break;


		case IDR_COPY:
			MessageBox(hwnd, TEXT("복사"), TEXT("알림"), MB_OK);
			break;

		case IDR_CUT:

			MessageBox(hwnd, TEXT("삭제"), TEXT("알림"), MB_OK);
			break;

		case IDR_PASTE:

			MessageBox(hwnd, TEXT("붙여넣기"), TEXT("알림"), MB_OK);
			break;

		case IDR_UNDO:

			MessageBox(hwnd, TEXT("되돌리기"), TEXT("알림"), MB_OK);
			break;
		}
		return 0;

	case WM_DESTROY:
		// WM_QUIT 메세지를 발송
		PostQuitMessage(0);
		return 0;
	}

	// 그 외의 메세지는 윈도우에서 알아서 처리하도록 설정
	return DefWindowProc(hwnd, msg, wParam, lParam);

}






