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
   

    static list<int> yPos;
    static list<int> xPos;

    static int xCar = 50;
    static int yCar = 50;

    static int totalCars = 0;
    
    static int y = -50;
    static int x = 0;

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
            if (random < yCar) {
                yPos.push_front(y);
                totalCars++;
            }

            if (random < xCar) {
                xPos.push_front(x);
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
            for (auto it = xPos.begin(); it != xPos.end(); ++it) {

                if (*it > 370 && *it < 390 && trafficState != 0) {
                }
                else {
                    if (next(it) != xPos.end() && (*next(it) - *it < 50)) {           
                    }
                    else {
                        *it += 5;
                        }
                    }
                
            }
            for (auto it = yPos.begin(); it != yPos.end(); ++it) {
                if (*it > 130 && *it < 150 && trafficState != 3) {

                }
                else {
                    if (next(it) != yPos.end() && *next(it) - *it < 50) {

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
            if (yCar < 100) {
                yCar+=10;
            }
            break;
        case VK_DOWN:
            if (yCar > 0) {
                yCar-=10;
            }
            break;
        case VK_LEFT:
            if (xCar < 100) {
                xCar-=10;
            }
            break;
        case VK_RIGHT:
            if (xCar > 0) {
                xCar+=10;
            }
            break;
        }
        
    //case WM_LBUTTONDOWN:
    //{
    //    if (yCar < 100) {
    //        yCar++;
    //        xCar--;
    //    }

    //    InvalidateRect(hWnd, 0, 1);

    //    break;
    //}
    //case WM_RBUTTONDOWN:
    //{
    //    if (xCar < 100) {
    //        xCar++;
    //        yCar--;
    //    }

    //    InvalidateRect(hWnd, 0, 1);

    //    break;
    //}
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

        Rectangle(hdc, 400, 100, 425, 175); //Trafikklys y akse
        Rectangle(hdc, 350, 275, 425, 300); //trafiklys x akse

        RECT xVei = { 0,175,2000,275 }; // vei x akse koordinater
        RECT yVei = { 425,0,525,1000 }; // vei y akse kooridnater
        
        Rectangle(hdc, xVei.left, xVei.top, xVei.right, xVei.bottom); //tegner horisontal vei langs x akse
        Rectangle(hdc, yVei.left, yVei.top, yVei.right, yVei.bottom); //tegner vertikal vei langs y akse

        HGDIOBJ hOrg = SelectObject(hdc,blank);

        //int a = 16;
        wchar_t yBuffer[32];
        wchar_t xBuffer[32];
        wchar_t carsBuffer[32];
        
        wsprintf(yBuffer, L"%d%%", yCar);
        wsprintf(xBuffer, L"%d%%", xCar);
        wsprintf(carsBuffer, L"Total Cars: %d", totalCars);
        
        TextOut(hdc, 390, 20, yBuffer, wcslen(yBuffer));
        TextOut(hdc, 20, 150, xBuffer, wcslen(xBuffer));
        TextOut(hdc, 20, 20, carsBuffer, wcslen(carsBuffer));
        

        //itererer gjennom lista og tegner rektangel(bil).
        for (int& x : xPos) {
            Rectangle(hdc, x, 230, x + 30, 250); //Tegner/flytter bil på x akse.
        }
        for (int& y : yPos) {
            Rectangle(hdc, 450, y, 470, y + 30); //tegner/flytter bil på y akse.
        }

        //Tildeler farger ettersom hvilken state trafikklyset er i.
        switch (trafficState) {
        case 0: //rødt lys y
            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, 400, 100, 425, 125);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 125, 425, 150);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 150, 425, 175);

            // grønt x akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 350, 275, 375, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 375, 275, 400, 300);

            hOrg = SelectObject(hdc, gronn);
            Ellipse(hdc, 400, 275, 425, 300);


            break;
        case 1: //rødt/gult lys y akse
            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, 400, 100, 425, 125);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, 400, 125, 425, 150);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 150, 425, 175);

            // gult x akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 350, 275, 375, 300);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, 375, 275, 400, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 275, 425, 300);
            
            break;
        case 2: //gult lys y akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 100, 425, 125);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, 400, 125, 425, 150);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 150, 425, 175);

            // rødt x akse
            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, 350, 275, 375, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 375, 275, 400, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 275, 425, 300);
            break;
        case 3: // grønt lys y akse
            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 100, 425, 125);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 125, 425, 150);
            
            
            hOrg = SelectObject(hdc, gronn);
            Ellipse(hdc, 400, 150, 425, 175);
           
            //rødt Trafikklys x akse;

            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, 350, 275, 375, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 375, 275, 400, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 275, 425, 300);


            break;
        case 4:

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 100, 425, 125);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, 400, 125, 425, 150);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 150, 425, 175);

            //rødt Trafikklys x akse;

            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, 350, 275, 375, 300);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, 375, 275, 400, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 275, 425, 300);

            break;

        case 5:

            hOrg = SelectObject(hdc, rod);
            Ellipse(hdc, 400, 100, 425, 125);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 125, 425, 150);


            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 150, 425, 175);

            //rødt Trafikklys x akse;

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 350, 275, 375, 300);

            hOrg = SelectObject(hdc, gul);
            Ellipse(hdc, 375, 275, 400, 300);

            hOrg = SelectObject(hdc, blank);
            Ellipse(hdc, 400, 275, 425, 300);

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