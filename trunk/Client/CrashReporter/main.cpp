//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: main.cpp
// Project: Client.CrashReporter
// Author(s): FRi
// License: See LICENSE in root directory
//
//==============================================================================
#include "main.h"

using namespace std;
HINSTANCE hInst;
string
	windowTitle,
	windowClass,
	statusText,
	sNickName;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
CHttpClient			* g_pHttpClient;

#define BACKGROUND 9999
#define BUTTON_SEND 9997
#define BUTTON_CLOSE 9998

#ifdef IDI_SMALL
#undef IDI_SMALL
#endif

#define IDI_SMALL 108
String reportData;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	windowTitle = "IV:MP CrashReporter";
	windowClass = "IVMPCRASH";
	MyRegisterClass(hInstance);

	reportData = lpCmdLine;

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = {0};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = CreatePatternBrush(LoadBitmap( hInstance, MAKEINTRESOURCE( BACKGROUND ) ) );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= windowClass.c_str();
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
HWND hMainWindow;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; 

   hMainWindow = CreateWindow(windowClass.c_str(), windowTitle.c_str(), WS_CAPTION | WS_SYSMENU | BS_BITMAP,
      CW_USEDEFAULT, 0, 450, 400, NULL, NULL, hInstance, NULL);

   if (!hMainWindow)
      return FALSE;

   ShowWindow(hMainWindow, nCmdShow);
   UpdateWindow(hMainWindow);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			string st;

			HANDLE hBitmap = LoadBitmap( hInst, MAKEINTRESOURCE( 9998 ) );
			HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
			HWND dwPointer = CreateWindowEx(NULL,"Button", "Close", WS_CHILD | WS_VISIBLE | BS_USERBUTTON | BS_BITMAP, 280, 320, 150, 31, hWnd, (HMENU)BUTTON_CLOSE, hInst, NULL);
			SendMessage(dwPointer, WM_SETFONT, (WPARAM) hFont, TRUE);
			SendMessage (dwPointer, BM_SETIMAGE, (WPARAM) IMAGE_BITMAP,(LPARAM)(HANDLE) hBitmap);

			hBitmap = LoadBitmap( hInst, MAKEINTRESOURCE( 9997 ) );
			hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
			dwPointer = CreateWindowEx(NULL,"Button", "Send", WS_CHILD | WS_VISIBLE | BS_USERBUTTON | BS_BITMAP, 10, 320, 150, 31, hWnd, (HMENU)BUTTON_SEND, hInst, NULL);
			SendMessage(dwPointer, WM_SETFONT, (WPARAM) hFont, TRUE);
			SendMessage (dwPointer, BM_SETIMAGE, (WPARAM) IMAGE_BITMAP,(LPARAM)(HANDLE) hBitmap);
			hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
			dwPointer = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", reportData.Get(), WS_CHILD | WS_VISIBLE, 10, 75, 420, 215, hWnd , NULL, hInst, NULL);
			SendMessage(dwPointer, WM_SETFONT, (WPARAM) hFont, TRUE);
		}
        case WM_PAINT:
        {
             HDC hdc;
             PAINTSTRUCT ps;
             
			 hdc = BeginPaint( hMainWindow, &ps );
             HWND textHwnd = GetWindow( hMainWindow, GW_CHILD );

             if ( textHwnd != NULL )
             {
                 HDC textHdc = GetDC( textHwnd );
                 SetBkMode( textHdc, TRANSPARENT );

				RECT rc;
				GetClientRect(hWnd,&rc);
				SetBkMode(hdc,TRANSPARENT);
				HFONT hFont=CreateFont(22,0,0,0,FW_BOLD,0,0,0,0,0,0,2,0,"Arial");
				HFONT hTmp=(HFONT)SelectObject(hdc,hFont);
				SetTextColor(hdc,RGB(255,255,255));

                 TextOut( hdc, 15,25, "Unfortunately IV:MP has crashed :/", strlen("Unfortunately IV:MP has crashed :/") );
             }
             
             EndPaint( hMainWindow, &ps );
        }
		case WM_CTLCOLORSTATIC:
		{
			SetTextColor((HDC)wParam,RGB(128,255,128));
			SetBkMode((HDC)wParam,TRANSPARENT);
			return (LRESULT)GetStockObject(BLACK_BRUSH);
		}
		
		case WM_COMMAND:
		{
			int wmId, wmEvent;
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
				case BUTTON_CLOSE:
				{
					PostQuitMessage(0);
				}
				case BUTTON_SEND:
				{
						g_pHttpClient = new CHttpClient();
						g_pHttpClient->SetRequestTimeout(10000);
						g_pHttpClient->SetHost("crashreporter.iv-multiplayer.com");

						String strPostPath("/sendreport.php");

						//String strReportData("Crash data");

						g_pHttpClient->SendReport(strPostPath,reportData);
						MessageBoxA(NULL,"Thanks for reporting your crash!","Your crash was successfully reported", MB_OK | MB_ICONASTERISK);
						PostQuitMessage(0);
				}
			}
		}
		break;
		case WM_DESTROY:
		{
			if(hWnd == hMainWindow)
				PostQuitMessage(0);
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

	}

	return 0;
}
