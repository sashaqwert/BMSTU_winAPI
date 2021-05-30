#include <Windows.h>
#include <tchar.h>
#include <string>
#include<sstream>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

static HWND window2 = NULL;

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
		_T("���� 1 --> ������������ ������ �2"), 		// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ���� 
		64,				// x 
		64, 				// y	 ������� ���� 
		256, 						// width 
		135, 						// Height 
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
	case WM_LBUTTONUP:
		window2 = FindWindow(NULL, L"���� 2 --> ������������ ������ �2");
		InvalidateRect(hWnd, NULL, true);
		if (window2 == NULL) {
			MessageBox(hWnd, L"���� �2 �� �������", L"��������� ������� ����", MB_OK);
		}
		else {
			MessageBox(hWnd, L"���� �2 �������", L"��������� ������� ����", MB_OK);
		}
		break;
	case WM_MBUTTONUP:
		if (window2 == NULL) {
			MessageBox(hWnd, L"���������� ��������� ��������� � �������������� ����\n����������, ������� �� ���� �2.", L"������", MB_OK);
			break;
		}
		SendMessageW(window2, WM_USER + 1, (WPARAM)hWnd, NULL);
		break;
	case WM_RBUTTONUP:
		if (window2 == NULL) {
			MessageBox(hWnd, L"���������� ��������� ��������� � �������������� ����\n����������, ������� �� ���� �2.", L"������", MB_OK);
			break;
		}
		SendMessageW(window2, WM_USER + 2, NULL, NULL);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		TextOutW(hdc, 10, 10, L"��������� ������� ����: ", lstrlen(L"��������� ������� ����: "));
		{
			std::wostringstream ss;
			ss << std::hex << hWnd;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 10, 25, d1w, lstrlen(d1w));
		}
		TextOutW(hdc, 10, 50, L"��������� ������� ����: ", lstrlen(L"��������� ������� ����: "));
		{
			std::wostringstream ss;
			ss << std::hex << window2;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 10, 75, d1w, lstrlen(d1w));
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// ���������� ��������� 
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
