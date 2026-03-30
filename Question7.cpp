// Question7.cpp : Defines the entry point for the application.
//
//fucking hate this assignment so much 
#include "framework.h"
#include "Question7.h"
#include "Page.h"
#include "Helpers.h"
#include"Document.h"
#include "SpellCheck.h"
#include "stats.h"

using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//setting up all static vars used in Page for setup
int Page::cols=0;
int Page::lines=0;
int Page::charsPerLine=0;
bool Page::SetUpDone = false;
int Page::CurrentSetUpVar = 0;
int Document::CurrentAlignment = 0; //left aligned by default
SpellCheck* SpellCheck::dictionary = new SpellCheck;

int Document::DocumentCount = 10;
Document* Document::doc = nullptr;
int Document::currentDoc=0;
int Document::maxDocs = 10;


//should make these statics 
const int InputSize = 500;
wchar_t Inputbuffer[InputSize];
const int WordBufferSize = 200;
wchar_t WordBuffer[WordBufferSize] = {};
int WordLen = 0;
bool displayTBC=false;

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
    LoadStringW(hInstance, IDC_QUESTION7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUESTION7));

    MSG msg;
    SpellCheck::dictionary->ReadFileDic();
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUESTION7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUESTION7);
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

   SetTimer(hWnd, 2007, 60000, NULL);

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
    switch (message)
    {
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
    case WM_PAINT:
        {
      
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
           
           
            if (displayTBC==true && Page::SetUpDone)
            {
                buildTabeOfContents(hdc,hWnd);
                EndPaint(hWnd, &ps);
                break;
            }
            paint(hdc, ps, hWnd, Inputbuffer, InputSize);

         
        }
        break;


    case WM_KEYDOWN:
    {
        if (GetKeyState(VK_CONTROL) & 256) //check the high order byte for being held down 
        {
         
            if (wParam == L'L') Document::CurrentAlignment = 0;
            else if (wParam == L'E')Document::CurrentAlignment = 1;
            else if (wParam == L'R')Document::CurrentAlignment = 2;
            else if (wParam == L'J') Document::CurrentAlignment = 3;//justified
            else if (wParam == 'T') displayTBC = !displayTBC;
            else if (wParam == 'D')
            {
                SpellCheck::dictionary->ReadFileDic();

                
            }
            else if (wParam == L'F')//search
            {
                Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->searching = !Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->searching;
                if (!Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->searching)
                    ClearBuffer(Inputbuffer, InputSize);
            }
            else if (GetKeyState(VK_TAB) & 256)
            {
                if (GetKeyState(VK_SHIFT) & 256)//shift doc back
                {
                    Document::currentDoc--;
                    if (Document::currentDoc < 0)Document::currentDoc = 0;

                }
                else//shift doc frw
                {
                    Document::currentDoc++;
                    if (Document::currentDoc > 9)Document::currentDoc = 9;
                }
            }
          
       
            
        }
        else if (GetKeyState(VK_ESCAPE) & 256) exit(0);
     
        
        
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_CHAR:
    {

       
        ParseInput(wParam, Inputbuffer, InputSize, WordBuffer,  WordLen,  WordBufferSize);

        
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_TIMER:
    {
        if(wParam==2007)
            if (Page::SetUpDone  && Document::doc != nullptr) AutoSave();
            
        break;
    }
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



