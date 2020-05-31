// brick.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "brick.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int RandPos()
{
    return rand() % 501;
}

void DrawBrick(HDC hdc, int x, int y, int s, int bplayer)
{
    HBRUSH brush;
    brush = bplayer ? CreateSolidBrush(RGB(255, 0, 0)) : CreateSolidBrush(RGB(0, 255, 0));
    RECT rc;
    rc.top = y - s / 2;
    rc.left = x - s / 2;
    rc.bottom = y + s / 2;
    rc.right = x + s / 2;
    FillRect(hdc, &rc, brush);
    DeleteObject(brush);
}

int IsCollider(int x1, int y1, int x2, int y2, int s)
{
    if (x1 + s<x2 || x1 - s>x2)
        return 0;
    if (y1 + s<y2 || y1 - s>y2)
        return 0;
    return 1;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    xP = RandPos();
    yP = RandPos();
    xE = RandPos();
    yE = RandPos();
    score = 0;
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BRICK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BRICK));

    MSG msg;

    // 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BRICK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BRICK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_TIMER:
        {
            rawTimerCounter++;
            if ( (timecount > 0) && ((rawTimerCounter % 10) == 0) )
            {
                timecount -= 1;
            }

            switch (direction)
            {
                case DIR_LEFT:
                    xP -= xP < 0 ? 0 : 15;
                    break;
                case DIR_RIGHT:
                    xP += xP > 500 ? 0 : 15;
                    break;
                case DIR_UP:
                    yP -= yP < 0 ? 0 : 15;
                    break;
                case DIR_DOWN:
                    yP += yP > 500 ? 0 : 15;
                    break;
            }
            if (IsCollider(xE, yE, xP, yP, size))
            {
                score++;
                xE = RandPos(), yE = RandPos();
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_LEFT:
                direction = DIR_LEFT;
                //xP -= xP < 0 ? 0 : 10;
                break;
            case VK_RIGHT:
                direction = DIR_RIGHT;
                //xP += xP > 500 ? 0 : 10;
                break;
            case VK_UP:
                direction = DIR_UP;
               // yP -= yP < 0 ? 0 : 10;
                break;
            case VK_DOWN:
                direction = DIR_DOWN;
               // yP += yP > 500 ? 0 : 10;
                break;
            }
            //if (IsCollider(xE, yE, xP, yP, size))
            //{
            //    score++;
            //    xE = RandPos(), yE = RandPos();
           // }
             InvalidateRect(hWnd, NULL, TRUE);
        } 
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            SetTimer(hWnd, 1, 100, NULL);
            WCHAR str[16];
            if (timecount <= 0)
            {
                swprintf(str, 16, L"游戏结束，你的得分：%d", score);
                TextOut(hdc, 0, 0, str, wcslen(str));
            }
            else
            {
                swprintf(str, 16, L"时间：%d", timecount);
                TextOut(hdc, 60, 0, str, wcslen(str));
                swprintf(str, 16, L"得分：%d", score);
                TextOut(hdc, 0, 0, str, wcslen(str));
                DrawBrick(hdc, xE, yE, size, 0);
                DrawBrick(hdc, xP, yP, size, 1);
               
            }
            EndPaint(hWnd, &ps);
        }
            break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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
