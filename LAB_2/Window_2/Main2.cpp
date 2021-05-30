#include <Windows.h>
#include <tchar.h>
#include <sstream>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

/*
* Для выключения
* shutdown /s /t 600 /c "1234567890"
*/

static HWND window1 = NULL;


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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// Заполнение окна белым цветом 


	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна 
	hWnd = CreateWindow(WinName,			// Имя класса окна 
		_T("Окно 2 --> Лабораторная работа №2"), 		// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна 
		320,						// x 
		64, 						// y	 Размеры окна 
		256, 						// width 
		135, 						// Height 
		HWND_DESKTOP, 				// Дескриптор родительского окна 
		NULL, 						// Нет меню 
		This, 						// Дескриптор приложения 
		NULL); 					// Дополнительной информации нет 

	ShowWindow(hWnd, mode); 	// Показать окно

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
	case WM_USER + 1:
		MessageBox(hWnd, L"Сообщение получено\n", L"WM_USER + 1", MB_OK);
		window1 = (HWND)wParam;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_USER + 2:
		system("shutdown /s /t 600 /c \"Этот пк (RDP сервер) выключится через 10 минут. Введите shutdown /a в поиск для отмены\"");
		break;
	case WM_LBUTTONUP:
		if (window1 == NULL) {
			MessageBox(hWnd, L"NULL", L"Дескриптор первого окна", MB_OK);
		}
		else {
			MessageBox(hWnd, L"Был получен ранее", L"Дескриптор первого окна", MB_OK);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		TextOutW(hdc, 10, 10, L"Дескриптр первого окна: ", lstrlen(L"Дескриптр первого окна: "));
		{
			std::wostringstream ss;
			ss << std::hex << window1;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 10, 25, d1w, lstrlen(d1w));
		}
		TextOutW(hdc, 10, 50, L"Дескриптр второго окна: ", lstrlen(L"Дескриптр первого окна: "));
		{
			std::wostringstream ss;
			ss << std::hex << hWnd;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 10, 75, d1w, lstrlen(d1w));
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// Завершение программы 
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
