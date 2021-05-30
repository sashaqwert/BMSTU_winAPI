// withRES.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "withRES.h"
#include <tchar.h>
#include <string>
#include<sstream>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

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

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WITHRES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WITHRES));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WITHRES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WITHRES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 305, 150, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
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
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case BUTTON_START:
                SetTimer(hWnd, TIMER_1, 1000, (TIMERPROC)NULL);
                break;
            case ID_START:
                SetTimer(hWnd, TIMER_1, 1000, (TIMERPROC)NULL);
                break;
            case BUTTON_STOP:
                KillTimer(hWnd, TIMER_1);
                break;
            case ID_STOP:
                KillTimer(hWnd, TIMER_1);
                break;
            case BUTTON_RESET:
                reset_HH_MM_SS();
                InvalidateRect(hWnd, NULL, true);
            case ID_RESET:
                reset_HH_MM_SS();
                break;
            case BUTTON_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
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
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
