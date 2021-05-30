#include <Windows.h>
#include <tchar.h>
#include <string>
#include<sstream>
#include<iomanip>
#include"Header.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

static int hh = 0;
static int mm = 0;
static int ss = 0;
HWND buttonStart = NULL;
HWND buttonStop = NULL;
HWND buttonReset = NULL;
HWND buttonExit = NULL;

static void reset_HH_MM_SS() {
	hh = 0;
	mm = 0;
	ss = 0;
}

static void increment_HH_MM_SS() {
	if (ss == 59) {
		if (mm == 59) {
			ss = 0;
			mm = 0;
			hh++;
		}
		else {
			ss = 0;
			mm++;
		}
	}
	else {
		ss++;
	}
}


int WINAPI _tWinMain(HINSTANCE This,		 // ���������� �������� ���������� 
	HINSTANCE Prev, 	// � ����������� �������� ������ 0 
	LPTSTR cmd, 		// ��������� ������ 
	int mode) 		// ����� ����������� ����
{
	HWND hWnd;		// ���������� �������� ���� ��������� 
	MSG msg; 		// ��������� ��� �������� ��������� 
	WNDCLASS wc; 	// ����� ����
	// ����������� ������ ���� 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 				// ��� ������ ���� 
	wc.lpfnWndProc = WndProc; 					// ������� ���� 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// ����� ���� 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 		// ����������� ������ 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// ����������� ������ 
	wc.lpszMenuName = NULL; 					// ��� ���� 
	wc.cbClsExtra = 0; 						// ��� �������������� ������ ������ 
	wc.cbWndExtra = 0; 						// ��� �������������� ������ ���� 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// ���������� ���� ����� ������ 


	// ����������� ������ ����
	if (!RegisterClass(&wc)) return 0;

	// �������� ���� 
	hWnd = CreateWindow(WinName,			// ��� ������ ���� 
		_T("���������� - ������������ ������ �3"), 		// ��������� ���� 
		WS_VISIBLE, 		// ����� ���� 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 ������� ���� 
		305, 						// width 
		140, 						// Height 
		HWND_DESKTOP, 				// ���������� ������������� ���� 
		NULL, 						// ��� ���� 
		This, 						// ���������� ���������� 
		NULL); 					// �������������� ���������� ��� 

	ShowWindow(hWnd, mode); 	// �������� ����

	// ���� ��������� ��������� 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// ������� ���������� ����� ������� ������� 
		DispatchMessage(&msg); 		// �������� ��������� ������� WndProc() 
	}
	return 0;
}

// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static BOOL fDraw = FALSE;
	static POINT ptPrevious = { 0 };

	switch (message)		 // ���������� ���������
	{
	case WM_CREATE:
		buttonStart = CreateWindow(L"button", L"�����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 60, 30, hWnd, (HMENU)BUTTON_START, NULL, NULL);
		buttonStop = CreateWindow(L"button", L"����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 75, 10, 60, 30, hWnd, (HMENU)BUTTON_STOP, NULL, NULL);
		buttonReset = CreateWindow(L"button", L"�����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 140, 10, 60, 30, hWnd, (HMENU)BUTTON_RESET, NULL, NULL);
		buttonExit = CreateWindow(L"button", L"�����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 205, 10, 60, 30, hWnd, (HMENU)BUTTON_EXIT, NULL, NULL);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case BUTTON_START:
			SetTimer(hWnd, TIMER_1, 1000, (TIMERPROC)NULL);
			break;
		case BUTTON_STOP:
			KillTimer(hWnd, TIMER_1);
			break;
		case BUTTON_RESET:
			reset_HH_MM_SS();
			InvalidateRect(hWnd, NULL, true);
			break;
		case BUTTON_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
	}
	case WM_LBUTTONUP:
		break;
	case WM_MBUTTONUP:
		break;
	case WM_TIMER:
		switch (wParam) //��������� ������� ��������
		{
		case TIMER_1:
			increment_HH_MM_SS(); //�������� 1 ������� � �����������
			InvalidateRect(hWnd, NULL, true);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		static LOGFONT lf; //������ ��������� LOGFONT
		lf.lfCharSet = DEFAULT_CHARSET; //�������� �� ���������
		lf.lfPitchAndFamily = DEFAULT_PITCH; //�������� �� ���������
		lstrcpy(lf.lfFaceName, L"Times New Roman"); //�������� � ������ �������� ������
		lf.lfHeight = 50; //������
		lf.lfWidth = 25; //������
		lf.lfWeight = 40; //�������
		lf.lfEscapement = 0; //����� ��� ��������

		HFONT hFont = CreateFontIndirect(&lf); //C������ �����
		SelectObject(hdc, hFont); //�� ����� ����� ���� ������ ����� �� ��� �������
		SetTextColor(hdc, RGB(0, 0, 0)); //������� ���� ������
		SetBkColor(hdc, RGB(255, 255, 255)); //������� ���� ����
		{
			std::wostringstream s;
			//s << std::setfill('0') << std::setw(2) << hh << " : " << std::setfill('0') << std::setw(2) << mm << " : " << std::setfill('0') << std::setw(2) << ss;
			if (hh < 10) s << "0";
			s << hh << " : ";
			if (mm < 10) s << "0";
			s << mm << " : ";
			if (ss < 10) s << "0";
			s << ss;
			std::wstring temp = s.str();
			LPWSTR time = (LPWSTR)temp.c_str();
			TextOutW(hdc, 10, 40, time, lstrlen(time));
		}
		DeleteObject(hFont); //�������� �� ������ ������ ������
		EndPaint(hWnd, &ps);
	}
	case WM_RBUTTONUP:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// ���������� ��������� 
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
