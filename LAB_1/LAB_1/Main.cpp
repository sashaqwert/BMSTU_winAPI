#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//const COLORREF rgbRed = 0x000000FF;
const COLORREF rgbGreen = 0x0000FF00;
//const COLORREF rgbBlue = 0x00FF0000;
//const COLORREF rgbBlack = 0x00000000;
//const COLORREF rgbWhite = 0x00FFFFFF;

TCHAR WinName[] = _T("MainFrame");
static POINT p_courcorPos;
static tagPOINT tp_defaultCoursorPosition = { 50l, 50l };

//�����
static HPEN pen1 = CreatePen(PS_SOLID, 3, 0x000000FF); //�������
static HPEN pen2 = CreatePen(PS_SOLID, 4, 0x0000FF00); //������
static HPEN pen3 = CreatePen(PS_SOLID, 3, 0x00FF0000); //�����
static HPEN pen4 = CreatePen(PS_SOLID, 4, 0x00000000); //׸����
static HPEN pen5 = CreatePen(PS_SOLID, 3, 0x00FFFFFF); //�����

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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 9); 	// ���������� ���� ������� ������ 


	// ����������� ������ ����
	if (!RegisterClass(&wc)) return 0;

	// �������� ���� 
	hWnd = CreateWindow(WinName,			// ��� ������ ���� 
		_T("������������ ������ �1"), 		// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ���� 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 ������� ���� 
		CW_USEDEFAULT, 				// width 
		CW_USEDEFAULT, 				// Height 
		HWND_DESKTOP, 				// ���������� ������������� ���� 
		NULL, 						// ��� ���� 
		This, 						// ���������� ���������� 
		NULL); 					// �������������� ���������� ��� 

	ShowWindow(hWnd, mode); 	// �������� ����

	CreatePen(PS_SOLID, 3, rgbGreen);

	//����� �� ���������
	p_courcorPos = tp_defaultCoursorPosition;

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
	case WM_LBUTTONDOWN:
		fDraw = TRUE;
		ptPrevious.x = p_courcorPos.x;
		ptPrevious.y = p_courcorPos.y;
		break;
	case WM_LBUTTONUP:
		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);
		break;
	case WM_RBUTTONDOWN:
		GetCursorPos(&p_courcorPos);
		ScreenToClient(hWnd, &p_courcorPos);
		break;
	case WM_PAINT:
		if (fDraw)
		{
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			//����� ����� �������� �����-������ ������� ���������

			
			SelectObject(hdc, pen1);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x + 0, p_courcorPos.y + 20);
			SelectObject(hdc, pen2);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x + 0, p_courcorPos.y - 20);
			SelectObject(hdc, pen3);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x + 20, p_courcorPos.y + 0);
			SelectObject(hdc, pen4);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x - 20, p_courcorPos.y + 0);
			SelectObject(hdc, pen5);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x + 14, p_courcorPos.y - 14);
			SelectObject(hdc, pen1);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x + -14, p_courcorPos.y + 14);
			SelectObject(hdc, pen2);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x - 20, p_courcorPos.y + 8);
			SelectObject(hdc, pen3);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x - 6, p_courcorPos.y + 16);
			SelectObject(hdc, pen4);
			MoveToEx(hdc, p_courcorPos.x, p_courcorPos.y, NULL);
			LineTo(hdc, p_courcorPos.x + 16, p_courcorPos.y - 6);

			
			//����������� ��������
			EndPaint(hWnd, &ps);

			//��������� ����
			//ValidateRect(hWnd, NULL);
			fDraw = false;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// ���������� ��������� 
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
