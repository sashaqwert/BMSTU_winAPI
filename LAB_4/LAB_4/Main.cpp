#include <Windows.h>
#include <tchar.h>
#include <string>
#include<sstream>
#include<thread>

void changeCurrent();
bool endProgram = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int s1 = 19;
POINT Pt1[]{ {13, 19}, {13, 476}, {45, 470}, {59, 463}, {70, 454}, {86, 441}, {103, 414}, {131, 332}, {137, 290}, {141, 249}, {135, 198}, {113, 119}, {91, 83}, {86, 73}, {65, 57}, {56, 50}, {49, 49}, {45, 35}, {13, 19} }; // ���������� ������������ �����
int s2 = 24;
POINT Pt2[]{ {13, 19}, {13, 466}, {28, 458}, {46, 420}, {48, 416}, {62, 392}, {76, 368}, {96, 337}, {112, 308}, {124, 272}, {127, 250}, {127, 228}, {124, 210}, {121, 187}, {114, 158}, {106, 140}, {97, 116}, {86, 95}, {73, 68}, {64, 54}, {55, 42}, {45, 35}, {30, 26}, {13, 19} }; // ���������� ������������ �����

POINT* currentPolyagon = Pt1;
int* currentCountOfPoints = &s1;
DWORDLONG RAMsize = 0;
DWORDLONG RAMfree = 0;


HANDLE thread1 = NULL;
HANDLE thread2 = NULL;
DWORD WINAPI thread1_f(LPVOID t);
DWORD WINAPI thread2_f(LPVOID t);

DWORD dwIDThread;

TCHAR WinName[] = _T("MainFrame");

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
	hWnd = CreateWindow(WinName,									// ��� ������ ���� 
		_T("������������ ������ �4"), 								// ��������� ���� 
		WS_SYSMENU | WS_VISIBLE | WS_MINIMIZE | WS_MINIMIZEBOX, 	// ����� ���� 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 ������� ���� 
		700, 						// width 
		530, 						// Height 
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
		thread1 = CreateThread(NULL, 0, &thread1_f, hWnd, 0, NULL);
		thread2 = CreateThread(NULL, 0, &thread2_f, hWnd, 0, NULL);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		MoveToEx(hdc, 10, 10 , NULL);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
		HBRUSH old = (HBRUSH)SelectObject(hdc, hBrush);
		Polygon(hdc, currentPolyagon, (const int)(*currentCountOfPoints));
		DeleteObject(SelectObject(hdc, old));

		static LOGFONT lf; //������ ��������� LOGFONT
		lf.lfCharSet = DEFAULT_CHARSET; //�������� �� ���������
		lf.lfPitchAndFamily = DEFAULT_PITCH; //�������� �� ���������
		lstrcpy(lf.lfFaceName, L"Times New Roman"); //�������� � ������ �������� ������
		lf.lfHeight = 20; //������
		lf.lfWidth = 10; //������
		lf.lfWeight = 40; //�������
		lf.lfEscapement = 0; //����� ��� ��������

		HFONT hFont = CreateFontIndirect(&lf); //C������ �����
		SelectObject(hdc, hFont); //�� ����� ����� ���� ������ ����� �� ��� �������
		SetTextColor(hdc, RGB(255, 0, 0)); //������� ���� ������
		SetBkColor(hdc, RGB(255, 255, 255)); //������� ���� ����

		//RAM
		TextOutW(hdc, 200, 150, L"����� ���������� ������ (��): ", lstrlen(L"����� ���������� ������ (��): "));
		{
			std::wostringstream ss;
			ss << RAMsize;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 550, 150, d1w, lstrlen(d1w));
		}
		SetTextColor(hdc, RGB(0, 0, 255)); //������� ���� ������
		TextOutW(hdc, 200, 170, L"�������� ���������� ������ (��): ", lstrlen(L"�������� ���������� ������ (��): "));
		{
			std::wostringstream ss;
			ss << RAMfree;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 590, 170, d1w, lstrlen(d1w));
		}

		DeleteObject(hFont); //�������� �� ������ ������ ������
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		endProgram = true;
		PostQuitMessage(0);
		break; 			// ���������� ��������� 
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI thread1_f(LPVOID t)
{
	/* ��� ������� ������ */
	while (!endProgram)
	{
		Sleep(1000);
		changeCurrent();
		HWND hWnd = (HWND)t;
		InvalidateRect(hWnd, NULL, true);
	}
	return 0;
}

DWORD WINAPI thread2_f(LPVOID t)
{
	/* ��� �������� ������ */
	while (!endProgram)
	{
		Sleep(1000);
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		
		DWORDLONG RAM = statex.ullTotalPhys;
		RAM /= 1024 * 1024; //������� � ��������� �� ������
		RAMsize = RAM;
		DWORDLONG freeRAM = statex.ullAvailPhys;
		freeRAM /= 1024 * 1024;
		RAMfree = freeRAM;
	}
	return 0;
}

void changeCurrent() {
	if (*currentCountOfPoints == s1) {
		currentCountOfPoints = &s2;
		currentPolyagon = Pt2;
	}
	else
	{
		currentCountOfPoints = &s1;
		currentPolyagon = Pt1;
	}
}