// Lyskryss.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lyskryss.h"
#include <list>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:


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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LYSKRYSS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LYSKRYSS));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LYSKRYSS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LYSKRYSS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT_PTR timerIDtrafficLight = 0;
    UINT_PTR timerIDCAR = 1;
    UINT_PTR timerIDSpawnCars = 2;
   
    RECT client;
    GetClientRect(hWnd, &client);

    RECT xVei = { client.left, client.bottom / 2 - 50, client.right, client.bottom / 2 + 50 }; // vei x akse koordinater
    RECT yVei = { client.right / 2 - 50, client.top, client.right / 2 + 50, client.bottom }; // vei y akse kooridnater

    RECT xLys = { yVei.left - 75, xVei.bottom, yVei.left, xVei.bottom + 25 }; // lys til x aksen
    RECT yLys = { yVei.left - 25, xVei.top - 75, yVei.left, xVei.top }; // lys til y aksen

    static list<int> yCars;
    static list<int> xCars;

    static int pw = 50;
    static int pn = 50;

    static int totalCars = 0;
    
    static int yStartPos = -50;
    static int xStartPos = -50;

    static int trafficState = 0;
   
    static int random = 0;
    
    switch (message)
    {
    case WM_CREATE:
        timerIDtrafficLight = SetTimer(hWnd, 0, 3000, 0); //Trafikklys ID = 0;
        timerIDCAR = SetTimer(hWnd, 1, 100, 0); //Bil ID = 1;
        timerIDSpawnCars = SetTimer(hWnd, 2, 1000, 0); //Spawn ID = 2;
        break;

    case WM_TIMER:
        if (wParam == timerIDSpawnCars) {
            random = rand() % 101;
            if (random < pn) {
                yCars.push_front(yStartPos);
                totalCars++;
            }

            if (random < pw) {
                xCars.push_front(xStartPos);
                totalCars++;
            }
        }

        if (wParam == timerIDtrafficLight) {
            if (trafficState == 5 || trafficState == 2) {
                SetTimer(hWnd, 0, 3000, 0);
                if (trafficState == 5) {
                    trafficState = 0;
                }
                else {
                    trafficState++;
                }
            }
            else {
                SetTimer(hWnd, 0, 1000, 0);
                trafficState++;
            }

            InvalidateRect(hWnd, 0, 1);
        }
        else if (wParam == timerIDCAR) {
            for (auto it = xCars.begin(); it != xCars.end(); ++it) {
                
                if (*it > yVei.left - 40 && *it < yVei.left - 20 && trafficState != 0) {
                }
                else {
                    if (next(it) != xCars.end() && (*next(it) - *it < 50)) {           
                    }
                    else {
                        *it += 5;
                        }
                    }
                
            }
            for (auto it = yCars.begin(); it != yCars.end(); ++it) {
                if (*it > xVei.top - 40 && *it < xVei.top - 20 && trafficState != 3) {

                }
                else {
                    if (next(it) != yCars.end() && *next(it) - *it < 50) {

                    }
                    else {
                        *it += 5;
                    }
                }
            }
            
            InvalidateRect(hWnd, 0, 1);
        }
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_UP:
            if (pn < 100) {
                pn+=10;
            }
            break;

        case VK_DOWN:
            if (pn > 0) {
                pn-=10;
            }
            break;

        case VK_LEFT:
            if (pw > 0) {
                pw-=10;
            }
            break;

        case VK_RIGHT:
            if (pw < 100) {
                pw+=10;
            }
            break;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Oppretter fargene
        HBRUSH rod = CreateSolidBrush(RGB(255, 0, 0));
        HBRUSH gul = CreateSolidBrush(RGB(255, 255, 0));
        HBRUSH gronn = CreateSolidBrush(RGB(0, 255, 0));
        HBRUSH blank = CreateSolidBrush(RGB(0, 0, 0));
        
        Rectangle(hdc, xVei.left, xVei.top, xVei.right, xVei.bottom); //tegner horisontal vei langs x akse
        Rectangle(hdc, yVei.left, yVei.top, yVei.right, yVei.bottom); //tegner vertikal vei langs y akse

        Rectangle(hdc, xLys.left, xLys.top, xLys.right, xLys.bottom); //trafikklys x akse
        Rectangle(hdc, yLys.left, yLys.top, yLys.right, yLys.bottom); //trafikklys y akse

        HGDIOBJ hOrg = SelectObject(hdc,blank);

        wchar_t yBuffer[32];
        wchar_t xBuffer[32];
        wchar_t carsBuffer[32];
        
        wsprintf(yBuffer, L"%d%%", pn);
        wsprintf(xBuffer, L"%d%%", pw);
        wsprintf(carsBuffer, L"Total Cars: %d", totalCars);
        
        TextOut(hdc, yVei.left-50, client.top+20, yBuffer, wcslen(yBuffer));
        TextOut(hdc, client.left+20, xVei.top-30, xBuffer, wcslen(xBuffer));
        TextOut(hdc, 20, 20, carsBuffer, wcslen(carsBuffer));
        

        //itererer gjennom lista og tegner rektangel(bil).
        for (int& x : xCars) {
            Rectangle(hdc, x, client.bottom/2+5, x + 30, client.bottom/2+25); //Tegner/flytter bil på x akse.
        }
        for (int& y : yCars) {
            Rectangle(hdc, client.right/2-25, y, client.right/2-5, y + 30); //tegner/flytter bil på y akse.
        }

        RECT yRed = { yLys.left, yLys.top, yLys.left + 25, yLys.top + 25 };
        RECT yYellow = {yLys.left, yLys.top + 25, yLys.left + 25, yLys.top + 50};
        RECT yGreen = {yLys.left, yLys.top + 50, yLys.left + 25, yLys.bottom};

        RECT xRed = {xLys.left, xLys.top, xLys.left + 25, xLys.bottom};
        RECT xYellow = {xLys.left + 25, xLys.top, xLys.left + 50, xLys.bottom};
        RECT xGreen = { xLys.left + 50, xLys.top, xLys.right, xLys.bottom };

        //Tildeler farger ettersom hvilken state trafikklyset er i.
        switch (trafficState) {
        case 0: //rødt lys y
            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, yRed.left, yRed.top, yRed.right, yRed.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yYellow.left, yYellow.top, yYellow.right, yYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yGreen.left, yGreen.top, yGreen.right, yGreen.bottom);

            // grønt x akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xRed.left, xRed.top, xRed.right, xRed.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xYellow.left, xYellow.top, xYellow.right, xYellow.bottom);

            hOrg = SelectObject(hdc, gronn);
            Ellipse(hdc, xGreen.left, xGreen.top, xGreen.right, xGreen.bottom);


            break;
        case 1: //rødt/gult lys y akse
            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, yRed.left, yRed.top, yRed.right, yRed.bottom);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, yYellow.left, yYellow.top, yYellow.right, yYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yGreen.left, yGreen.top, yGreen.right, yGreen.bottom);

            // gult x akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xRed.left, xRed.top, xRed.right, xRed.bottom);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, xYellow.left, xYellow.top, xYellow.right, xYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xGreen.left, xGreen.top, xGreen.right, xGreen.bottom);
            
            break;
        case 2: //gult lys y akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yRed.left, yRed.top, yRed.right, yRed.bottom);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, yYellow.left, yYellow.top, yYellow.right, yYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yGreen.left, yGreen.top, yGreen.right, yGreen.bottom);

            // rødt x akse
            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, xRed.left, xRed.top, xRed.right, xRed.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xYellow.left, xYellow.top, xYellow.right, xYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xGreen.left, xGreen.top, xGreen.right, xGreen.bottom);
            break;
        case 3: // grønt lys y akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yRed.left, yRed.top, yRed.right, yRed.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yYellow.left, yYellow.top, yYellow.right, yYellow.bottom);
            
            hOrg = SelectObject(hdc, gronn);
            Ellipse(hdc, yGreen.left, yGreen.top, yGreen.right, yGreen.bottom);
           
            //rødt Trafikklys x akse;

            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, xRed.left, xRed.top, xRed.right, xRed.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xYellow.left, xYellow.top, xYellow.right, xYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xGreen.left, xGreen.top, xGreen.right, xGreen.bottom);


            break;
        case 4:

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yRed.left, yRed.top, yRed.right, yRed.bottom);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, yYellow.left, yYellow.top, yYellow.right, yYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yGreen.left, yGreen.top, yGreen.right, yGreen.bottom);

            //rødt Trafikklys x akse;

            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, xRed.left, xRed.top, xRed.right, xRed.bottom);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, xYellow.left, xYellow.top, xYellow.right, xYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xGreen.left, xGreen.top, xGreen.right, xGreen.bottom);

            break;

        case 5:

            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, yRed.left, yRed.top, yRed.right, yRed.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yYellow.left, yYellow.top, yYellow.right, yYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, yGreen.left, yGreen.top, yGreen.right, yGreen.bottom);

            //rødt Trafikklys x akse;

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xRed.left, xRed.top, xRed.right, xRed.bottom);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, xYellow.left, xYellow.top, xYellow.right, xYellow.bottom);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, xGreen.left, xGreen.top, xGreen.right, xGreen.bottom);

            break;


        default:
            break;
        }

        SelectObject(hdc, hOrg);
        DeleteObject(rod);
        DeleteObject(gul);
        DeleteObject(gronn);
        DeleteObject(blank);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Message handler for about box.
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