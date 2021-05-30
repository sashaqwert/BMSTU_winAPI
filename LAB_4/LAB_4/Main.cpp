#include <Windows.h>
#include <tchar.h>
#include <string>
#include<sstream>
#include<thread>

void changeCurrent();
bool endProgram = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int s1 = 19;
POINT Pt1[]{ {13, 19}, {13, 476}, {45, 470}, {59, 463}, {70, 454}, {86, 441}, {103, 414}, {131, 332}, {137, 290}, {141, 249}, {135, 198}, {113, 119}, {91, 83}, {86, 73}, {65, 57}, {56, 50}, {49, 49}, {45, 35}, {13, 19} }; // Заполняешь координатами точек
int s2 = 24;
POINT Pt2[]{ {13, 19}, {13, 466}, {28, 458}, {46, 420}, {48, 416}, {62, 392}, {76, 368}, {96, 337}, {112, 308}, {124, 272}, {127, 250}, {127, 228}, {124, 210}, {121, 187}, {114, 158}, {106, 140}, {97, 116}, {86, 95}, {73, 68}, {64, 54}, {55, 42}, {45, 35}, {30, 26}, {13, 19} }; // Заполняешь координатами точек

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
	hWnd = CreateWindow(WinName,									// Имя класса окна 
		_T("Лабораторная работа №4"), 								// Заголовок окна 
		WS_SYSMENU | WS_VISIBLE | WS_MINIMIZE | WS_MINIMIZEBOX, 	// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		700, 						// width 
		530, 						// Height 
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

		static LOGFONT lf; //создаём экземпляр LOGFONT
		lf.lfCharSet = DEFAULT_CHARSET; //значение по умолчанию
		lf.lfPitchAndFamily = DEFAULT_PITCH; //значения по умолчанию
		lstrcpy(lf.lfFaceName, L"Times New Roman"); //копируем в строку название шрифта
		lf.lfHeight = 20; //высота
		lf.lfWidth = 10; //ширина
		lf.lfWeight = 40; //толщина
		lf.lfEscapement = 0; //шрифт без поворота

		HFONT hFont = CreateFontIndirect(&lf); //Cоздали шрифт
		SelectObject(hdc, hFont); //Он будет иметь силу только когда мы его выберем
		SetTextColor(hdc, RGB(255, 0, 0)); //зададим цвет текста
		SetBkColor(hdc, RGB(255, 255, 255)); //зададим цвет фона

		//RAM
		TextOutW(hdc, 200, 150, L"Всего физической памяти (МБ): ", lstrlen(L"Всего физической памяти (МБ): "));
		{
			std::wostringstream ss;
			ss << RAMsize;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 550, 150, d1w, lstrlen(d1w));
		}
		SetTextColor(hdc, RGB(0, 0, 255)); //зададим цвет текста
		TextOutW(hdc, 200, 170, L"Свободно физической памяти (МБ): ", lstrlen(L"Свободно физической памяти (МБ): "));
		{
			std::wostringstream ss;
			ss << RAMfree;
			std::wstring dw1 = ss.str();
			LPCWSTR d1w = dw1.c_str();
			TextOutW(hdc, 590, 170, d1w, lstrlen(d1w));
		}

		DeleteObject(hFont); //выгрузим из памяти объект шрифта
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		endProgram = true;
		PostQuitMessage(0);
		break; 			// Завершение программы 
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI thread1_f(LPVOID t)
{
	/* Код второго потока */
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
	/* Код третьего потока */
	while (!endProgram)
	{
		Sleep(1000);
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		
		DWORDLONG RAM = statex.ullTotalPhys;
		RAM /= 1024 * 1024; //Перевод в мегабайты из байтов
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