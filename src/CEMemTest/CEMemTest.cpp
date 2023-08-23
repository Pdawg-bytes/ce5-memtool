// CEMemTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CEMemTest.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;				// current instance
HWND				g_hWndMenuBar;			// menu bar handle
HWND				g_hWnd;					// main window handle
HWND				g_hPokeValue;			// poke value textbox handle
HWND				g_hPeekText;			// peek textbox handle
HWND				g_hPokeText;			// poke textbox handle

uintptr_t			g_targetPokeAddress;	// target poke address
unsigned char		g_pokeData[4];			// poke data
wchar_t				g_pokeAddressText[256]; // poke window text
wchar_t				g_peekAddressText[256]; // peek window text
uintptr_t			g_targetPeekAddress;	// target peek address

int					t_x; //test

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE, LPTSTR);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL PokeMemory(uintptr_t address, const void* data, size_t size)
{
	if(IsBadWritePtr((void*)address, size))
	{
		// thanks samuel for this one :)
		// it works well enough.
		MessageBox(g_hWnd, L"Invalid memory address for poke.", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	memcpy((void*)address, data, size);
	return true;
}
BOOL PeekMemory(uintptr_t address, void* destination, size_t size)
{
	if(IsBadReadPtr((const void*)address, size))
	{
		// thanks samuel for this one too :)
		// it works well enough.
		MessageBox(g_hWnd, L"Invalid memory address for peek.", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	memcpy(destination, (const void*)address, size);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	
	t_x = 75;
	printf("address of x is %p\n", &t_x);
	MSG msg;

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CEMEMTEST));

	HWND hPokeButton = CreateWindow(
		L"BUTTON", 
		L"Poke", 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 120, 60, 20,
		g_hWnd,
		(HMENU)1,
		g_hInst,
		NULL);

	g_hPokeValue = CreateWindow(
		L"EDIT",
		L"Input poke value",
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 90, 150, 25,
		g_hWnd,
		(HMENU)6,
		NULL,
		NULL);

	g_hPeekText = CreateWindow(
		L"EDIT",
		L"Input peek address",
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 200, 150, 25,
		g_hWnd,
		(HMENU)4,
		NULL,
		NULL);

	g_hPokeText = CreateWindow(
		L"EDIT",
		L"Input poke address",
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 50, 150, 25,
		g_hWnd,
		(HMENU)2,
		NULL,
		NULL);

	HWND hPeekButton = CreateWindow(
		L"BUTTON", 
		L"Peek", 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 230, 60, 20,
		g_hWnd,
		(HMENU)3,
		g_hInst,
		NULL);


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
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
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CEMEMTEST));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
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
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable

    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_CEMEMTEST, szWindowClass, MAX_LOADSTRING);

    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
    // is created is not taken into account). So we resize the window after creating it
    // if a menubar is present
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	g_hWnd = hWnd;

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent, dataLength;
    PAINTSTRUCT ps;
    HDC hdc;

    static SHACTIVATEINFO s_sai;
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
				case 1:
					GetWindowText(g_hPokeText, g_pokeAddressText, sizeof(g_pokeAddressText) / sizeof(g_pokeAddressText[0]));
					g_targetPokeAddress = wcstoul(g_pokeAddressText, NULL, 16);
					if (PokeMemory(g_targetPokeAddress, g_pokeData, sizeof(g_pokeData)))
					{
						wchar_t formattedPokeBytes[32];
						wsprintf(formattedPokeBytes, L"%02X %02X %02X %02X",
								 g_pokeData[0], g_pokeData[1], g_pokeData[2], g_pokeData[3]);
						MessageBox(g_hWnd, formattedPokeBytes, L"Poked bytes into memory!", MB_OK);
					}
					break;
				case 3:
					GetWindowText(g_hPeekText, g_peekAddressText, sizeof(g_peekAddressText) / sizeof(g_peekAddressText[0]));
					g_targetPeekAddress = wcstoul(g_peekAddressText, NULL, 16);
					unsigned char peekBuffer[4];
					if (PeekMemory(g_targetPeekAddress, peekBuffer, sizeof(peekBuffer)))
					{	
						wchar_t formattedPeekBytes[32];
						wsprintf(formattedPeekBytes, L"%02X %02X %02X %02X",
								 peekBuffer[0], peekBuffer[1], peekBuffer[2], peekBuffer[3]);
						MessageBox(g_hWnd, formattedPeekBytes, L"Peeked bytes", MB_OK);
					}
					break;
				case 6:
					wchar_t dataText[256];
					GetWindowText(g_hPokeValue, dataText, sizeof(dataText) / sizeof(dataText[0]));
					dataLength = swscanf(dataText, L"%hhx %hhx %hhx %hhx",
											 &g_pokeData[0], &g_pokeData[1], &g_pokeData[2], &g_pokeData[3]);
					break;
            }
            break;
        case WM_CREATE:
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

            // Initialize the shell activate info structure
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code here...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;

        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            {
                // Create a Done button and size it.  
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

    }
    return (INT_PTR)FALSE;
}