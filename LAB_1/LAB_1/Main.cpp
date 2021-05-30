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

//Кисти
static HPEN pen1 = CreatePen(PS_SOLID, 3, 0x000000FF); //Красная
static HPEN pen2 = CreatePen(PS_SOLID, 4, 0x0000FF00); //Зелёная
static HPEN pen3 = CreatePen(PS_SOLID, 3, 0x00FF0000); //Синяя
static HPEN pen4 = CreatePen(PS_SOLID, 4, 0x00000000); //Чёрная
static HPEN pen5 = CreatePen(PS_SOLID, 3, 0x00FFFFFF); //Белая

int WINAPI _tWinMain(HINSTANCE This,		 // Дескриптор текущего приложения 
	HINSTANCE Prev, 	// В современных системах всегда 0 
	LPTSTR cmd, 		// Командная строка 
	int mode) 		// Режим отображения окна
{
	HWND hWnd;		// Дескриптор главного окна программы 
	MSG msg; 		// Структура для хранения сообщения 
	WNDCLASS wc; 	// Класс окна
	// Определение класса окна 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 				// Имя класса окна 
	wc.lpfnWndProc = WndProc; 					// Функция окна 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// Стиль окна 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 		// Стандартная иконка 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// Стандартный курсор 
	wc.lpszMenuName = NULL; 					// Нет меню 
	wc.cbClsExtra = 0; 						// Нет дополнительных данных класса 
	wc.cbWndExtra = 0; 						// Нет дополнительных данных окна 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 9); 	// Заполнение окна голубым цветом 


	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна 
	hWnd = CreateWindow(WinName,			// Имя класса окна 
		_T("Лабораторная работа №1"), 		// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		CW_USEDEFAULT, 				// width 
		CW_USEDEFAULT, 				// Height 
		HWND_DESKTOP, 				// Дескриптор родительского окна 
		NULL, 						// Нет меню 
		This, 						// Дескриптор приложения 
		NULL); 					// Дополнительной информации нет 

	ShowWindow(hWnd, mode); 	// Показать окно

	CreatePen(PS_SOLID, 3, rgbGreen);

	//Точка по умолчанию
	p_courcorPos = tp_defaultCoursorPosition;

	// Цикл обработки сообщений 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// Функция трансляции кодов нажатой клавиши 
		DispatchMessage(&msg); 		// Посылает сообщение функции WndProc() 
	}
	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static BOOL fDraw = FALSE;
	static POINT ptPrevious = { 0 };

	switch (message)		 // Обработчик сообщений
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
			//здесь можно вставить какие-нибудь функции рисования

			
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

			
			//заканчиваем рисовать
			EndPaint(hWnd, &ps);

			//обновляем окно
			//ValidateRect(hWnd, NULL);
			fDraw = false;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// Завершение программы 
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
