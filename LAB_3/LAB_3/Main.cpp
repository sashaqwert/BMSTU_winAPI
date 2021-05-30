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
		_T("Секундомер - Лабораторная работа №3"), 		// Заголовок окна 
		WS_VISIBLE, 		// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		305, 						// width 
		140, 						// Height 
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
		buttonStart = CreateWindow(L"button", L"Старт", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 60, 30, hWnd, (HMENU)BUTTON_START, NULL, NULL);
		buttonStop = CreateWindow(L"button", L"Стоп", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 75, 10, 60, 30, hWnd, (HMENU)BUTTON_STOP, NULL, NULL);
		buttonReset = CreateWindow(L"button", L"Сброс", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 140, 10, 60, 30, hWnd, (HMENU)BUTTON_RESET, NULL, NULL);
		buttonExit = CreateWindow(L"button", L"Выход", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 205, 10, 60, 30, hWnd, (HMENU)BUTTON_EXIT, NULL, NULL);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
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
		switch (wParam) //Обработка событий таймеров
		{
		case TIMER_1:
			increment_HH_MM_SS(); //Добавить 1 секунду к секундомеру
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

		static LOGFONT lf; //создаём экземпляр LOGFONT
		lf.lfCharSet = DEFAULT_CHARSET; //значение по умолчанию
		lf.lfPitchAndFamily = DEFAULT_PITCH; //значения по умолчанию
		lstrcpy(lf.lfFaceName, L"Times New Roman"); //копируем в строку название шрифта
		lf.lfHeight = 50; //высота
		lf.lfWidth = 25; //ширина
		lf.lfWeight = 40; //толщина
		lf.lfEscapement = 0; //шрифт без поворота

		HFONT hFont = CreateFontIndirect(&lf); //Cоздали шрифт
		SelectObject(hdc, hFont); //Он будет иметь силу только когда мы его выберем
		SetTextColor(hdc, RGB(0, 0, 0)); //зададим цвет текста
		SetBkColor(hdc, RGB(255, 255, 255)); //зададим цвет фона
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
		DeleteObject(hFont); //выгрузим из памяти объект шрифта
		EndPaint(hWnd, &ps);
	}
	case WM_RBUTTONUP:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 			// Завершение программы 
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
