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
const int InputSize = 500;
wchar_t Inputbuffer[InputSize];
const int WordBufferSize = 200;
wchar_t WordBuffer[WordBufferSize] = {};
int WordLen = 0;

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


            //set background color
            RECT rect;
            GetClientRect(hWnd, &rect);
            HBRUSH bgBrush = CreateSolidBrush(RGB(20, 20, 40));
            FillRect(hdc, &rect, bgBrush);
            DeleteObject(bgBrush);

            //set text settings
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));


            //set Font
            HFONT font = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Trebuchet MS");
            SelectObject(hdc, font);

            //============note============
            //switch back to the font given by tas
            //its the one under the set Font comment

       /*     HFONT font = CreateFont(
                16, 7, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, FIXED_PITCH,
                L"Courier New");*/


            //    //autoSave Test
            //static bool AutoSaveOnce = true;
            //if (Page::SetUpDone && AutoSaveOnce && Document::doc[Document::currentDoc].getPageCount()>1) {
            //    AutoSave();
            //    AutoSaveOnce = false;
            //}

            if (!Page::SetUpDone) {
                if(Page::CurrentSetUpVar==0)
                    TextOutW(hdc, 50, 50, L"Enter value for number of cols on a page: ", 43);
                else if(Page::CurrentSetUpVar == 1)
                    TextOutW(hdc, 50, 50, L"Enter value for number of Lines on a page: ", 44);
                else
                    TextOutW(hdc, 50, 50, L"Enter value for number of charachters per Line on a page: ", 59);
            }
            //L means wide String treat every char as 2 bytes
            // LPCWSTR doesnt work cuz  LPCWSTR is a pointer type
           // if(typed)

            //output the buffer 
            if(!Page::SetUpDone)
                TextOutW(hdc, 50, 100, Inputbuffer, InputSize);
           // else
                

            //=========================debug Info========================================
                  /*
                    if (Page::SetUpDone)
                    {
                      wchar_t*  numstr = intToStr(Page::cols);
                        TextOutW(hdc, 50, 100, numstr, getLength(numstr));
                        delete[] numstr;


                        numstr = intToStr(Page::lines);
                        TextOutW(hdc, 50, 150, numstr, getLength(numstr));
                       delete[] numstr;

                        numstr = intToStr(Page::charsPerLine);
                        TextOutW(hdc, 50, 200, numstr, getLength(numstr));
                        delete[] numstr;
                    }*/
              //=========================debug Info========================================

            //ouput the document

            if (Page::SetUpDone) {

                bool Search = Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->searching;
                int leftMargin = 50;//px
                int topMargin = 50;//px
                int windowWidth = rect.right - rect.left;
                int colWidth = (windowWidth - 2 * leftMargin) / Page::cols;
                //int colWidth = 350;
                int lineHeight = 20; 
                int pagePadding = 120;
                int pageHeight = lineHeight * Page::lines;
                int Bufferx, buffery;
                SIZE AHHH;


                int pageCount = Document::doc[Document::currentDoc].getPageCount();
                for (int i = 0; i < pageCount; i++)
                {
                    int colCount = Document::doc[Document::currentDoc].getPagePtr(i)->getColCount();

                    for (int j = 0; j < colCount; j++)
                    {
                        //int numberOfgetFails = 0;
                        Col* Coll = Document::doc[Document::currentDoc].getPagePtr(i)->getColPtr();
                        int lineCount = Coll[j].linesFilled;
                        for (int k = 0; k <=lineCount; k++)
                        {
                            
                           // bool ValidWriteAddres = false;
                            int x = leftMargin + j * colWidth;
                            int y = (topMargin + k * lineHeight) + i * (pageHeight + pagePadding);
                            

                            //handle alignment on a per line basis
                            int lineLen = getLength(Coll[j].lines[k].chars);
                            int alignMode = Coll[j].lines[k].currentAlignment;

                            wchar_t renderBuffer[500] = { 0 };
                            int bufferIndex = 0;

                            if (alignMode == 2) //right
                            {
                                int emptySpaces = Page::charsPerLine - lineLen;
                                if (emptySpaces < 0) emptySpaces = 0;

                                // add spaces
                                for (int space = 0; space < emptySpaces; space++) 
                                    renderBuffer[bufferIndex++] = L' ';

                                //add chars
                                for (int chars = 0; chars < lineLen; chars++) renderBuffer[bufferIndex++] = Coll[j].lines[k].chars[chars];
                            }
                            else if (alignMode == 1) //center
                            {
                                int emptySpaces = (Page::charsPerLine - lineLen) / 2;
                                if (emptySpaces < 0) emptySpaces = 0;

                                // add spaces
                                for (int space = 0; space < emptySpaces; space++)
                                    renderBuffer[bufferIndex++] = L' ';

                                //add chars
                                for (int chars = 0; chars < lineLen; chars++) renderBuffer[bufferIndex++] = Coll[j].lines[k].chars[chars];
                            }
                            else if(alignMode==0) // left aligned
                            {
                               
                                for (int chars = 0; chars < lineLen; chars++) renderBuffer[bufferIndex++] = Coll[j].lines[k].chars[chars];
                            }
                            else
                            {
                               // Stats temp; 
                                
                                //in justified alignment the total extra space needed
                                //is divided equally among all the words

                                int totalExtraNeeded = Page::charsPerLine - lineLen;
                                
                                if (totalExtraNeeded > 0) {
                                    int spaceCount = 0;

                                    for (int c = 0; c < lineLen; c++) {
                                        if (Coll[j].lines[k].chars[c] == L' ') spaceCount++;
                                    }

                                    int extraPerGap = spaceCount > 0 ? totalExtraNeeded / spaceCount : 0;
                                    int remainder = spaceCount > 0 ? totalExtraNeeded % spaceCount : 0;

                                    for (int c = 0; c < lineLen; c++) {
                                        renderBuffer[bufferIndex++] = Coll[j].lines[k].chars[c];

                                        if (Coll[j].lines[k].chars[c] == L' ') {//if space found 

                                            for (int s = 0; s < extraPerGap; s++) renderBuffer[bufferIndex++] = L' '; //add extra space to output

                                            if (remainder > 0) {
                                                renderBuffer[bufferIndex++] = L' ';
                                                remainder--;
                                            }//append extra spaces to the end of lines 

                                        }

                                    }//for loop
                                }//if need to justify 
                                else
                                {
                                    for (int chars = 0; chars < lineLen; chars++) renderBuffer[bufferIndex++] = Coll[j].lines[k].chars[chars];
                                }//if you dont need any extra space then just print left aligned 
                            }


                            if (Search)
                            {


                                TextOutW(hdc, 50, 300, L"Enter String to find\0", 22);
                                int searchBufferSize = 500;
                                wchar_t* searchBuffer=new wchar_t[searchBufferSize];
                                copyString(L"Hammad\0", searchBuffer, 500); 
                                int indexSize = 1;
                                int* indicies = new int[indexSize];
                                int offset = 0;
                                offset = findSubstring(Coll[j].lines[k].chars, searchBuffer);
                                int searchLength = getLength(searchBuffer);
                                int iteration = 1;
                                bool fuckThisShit = false;
                                indicies[0] = offset;


                                if(offset!=-1)//if we found a match keep looking for more matches
                                    while (offset+ searchLength <= Page::charsPerLine && offset!=-1)
                                    {
                                        offset += findSubstring(Coll[j].lines[k].chars+offset+searchLength, searchBuffer)+ offset + searchLength;
                             
                                    
                                        if ( offset != -1)
                                        {
                                           // if (fuckThisShit) //only resize if the first index is already full
                                                ResizeArr(indicies, indexSize);
                                            //makes sure size is exact


                                                indicies[indexSize - 1] =  offset;


                                            fuckThisShit = true;
                                        }
                                        iteration++;
                                    }


                                for(int i=0;i<indexSize;i++)
                                    if ( indicies[i] != -1)
                                    {
                                        //im actually gonna fking cry bro 
                                        SIZE size,fuckmyLife;

                                        GetTextExtentPoint32W(hdc, renderBuffer,  indicies[i], &size);
                                        GetTextExtentPoint32W(hdc, renderBuffer +  indicies[i], getLength(searchBuffer), &fuckmyLife);

                                        TextOutW(hdc, x, y, renderBuffer,  indicies[i]);
                                        SetBkMode(hdc, OPAQUE);
                                        SetBkColor(hdc, 0xFF7F7F);//pinkish red

                                        TextOutW(hdc, x + size.cx, y, renderBuffer +  indicies[i], getLength(searchBuffer));
                                        SetBkMode(hdc, TRANSPARENT);
                                        TextOutW(hdc, x + size.cx+ fuckmyLife.cx, y, renderBuffer +  indicies[i]+ getLength(searchBuffer),(bufferIndex)- ( indicies[i] + getLength(searchBuffer) ) ) ;
                                        
                                    }
                                delete searchBuffer;
                                delete[] indicies;
                                indicies = nullptr;
                                searchBuffer = nullptr;
                                //Search = false;
                            }
                            
                            TextOutW(hdc, x, y, renderBuffer, bufferIndex);
                            Bufferx = x;
                            buffery = y;
                            GetTextExtentPoint32W(hdc, renderBuffer, getLength(renderBuffer),&AHHH);
                            
                             /* if (!(getLength(Coll[j].lines[k].chars)>2))
                                    numberOfgetFails++;

                            if(numberOfgetFails==1)
                                TextOutW(hdc, x+1, y, Inputbuffer, InputSize);*/
                        }
                        
                        if(j==Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->currentCol)
                            TextOutW(hdc, Bufferx+AHHH.cx, buffery, Inputbuffer, getLength(Inputbuffer));
                    }
                

                }
                
                //Display stats and footer

                Stats Footer;
                int TotalPages = Document::doc[Document::currentDoc].getPageCount();
                int CharsNoSpaces = Footer.NumberOfChars - Footer.NumberOfSpaces;

                int ReadTimeSeconds = (Footer.NumberOfWords * 60) / 200;
                int ReadTimeMinutes = ReadTimeSeconds / 60;
                int ReadTimeRemaining = ReadTimeSeconds % 60;
               
                wchar_t StatsBuffer[500];
                int i = 0;
                int j = 0;
                wchar_t* NumStr = nullptr;

                const wchar_t* Str1 = L"Chars: ";
                j = 0;
                while (Str1[j] != L'\0') StatsBuffer[i++] = Str1[j++];

                NumStr = intToStr(Footer.NumberOfChars);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str2 = L" | Chars (No Spaces): ";
                j = 0;
                while (Str2[j] != L'\0') StatsBuffer[i++] = Str2[j++];

                NumStr = intToStr(CharsNoSpaces);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str3 = L" | Words: ";
                j = 0;
                while (Str3[j] != L'\0') StatsBuffer[i++] = Str3[j++];

                NumStr = intToStr(Footer.NumberOfWords);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str4 = L" | Sentences: ";
                j = 0;
                while (Str4[j] != L'\0') StatsBuffer[i++] = Str4[j++];

                NumStr = intToStr(Footer.NumberOfSentences);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str5 = L" | Pages: ";
                j = 0;
                while (Str5[j] != L'\0') StatsBuffer[i++] = Str5[j++];

                NumStr = intToStr(TotalPages);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str6 = L" | Est. Read Time: ";
                j = 0;
                while (Str6[j] != L'\0') StatsBuffer[i++] = Str6[j++];

                NumStr = intToStr(ReadTimeMinutes);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str7 = L"m ";
                j = 0;
                while (Str7[j] != L'\0') StatsBuffer[i++] = Str7[j++];

                NumStr = intToStr(ReadTimeRemaining);
                j = 0;
                while (NumStr[j] != L'\0') StatsBuffer[i++] = NumStr[j++];
                delete[] NumStr;

                const wchar_t* Str8 = L"space";
                j = 0;
                while (Str8[j] != L'\0') StatsBuffer[i++] = Str8[j++];

                StatsBuffer[i] = L'\0';

                int FooterX = 50;
                int FooterY = rect.bottom - 40;

                wchar_t CurrentPageNumber[12];
                
                j = 0;
                
                for (j = 0; j < 5; j++) CurrentPageNumber[j] = L'-';
                NumStr = intToStr(Document::doc[Document::currentDoc].getCurrentPage()+1);
                for (int k = 0; NumStr[k] != '\0'; k++) CurrentPageNumber[j++] = NumStr[k];
                for (j; j < 12; j++) CurrentPageNumber[j] = L'-';
                CurrentPageNumber[11] = '\0';
                delete[] NumStr;

                TextOutW(hdc, FooterX, FooterY, StatsBuffer, i);
                TextOutW(hdc, rect.right / 2, rect.bottom - 80, CurrentPageNumber, 12);


            }

            DeleteObject(font);
            EndPaint(hWnd, &ps);
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
            else if (wParam == L'F') Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->searching = true;
            else if (GetKeyState(VK_TAB) & 256)
            {
                if (GetKeyState(VK_SHIFT) & 256)
                {
                    Document::currentDoc--;
                    if (Document::currentDoc < 0)Document::currentDoc = 0;

                }
                else
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

        if (GetKeyState(VK_CONTROL) & 0x8000) {
            break;
        }

        static int Index = 0;
        wchar_t input = (wchar_t)wParam;
        static bool isIndexNegative = false;

        //parsing input
        if (input == VK_BACK) {
            
           /* if (WordLen > 0)
                WordBuffer[--WordLen] = L'\0';*/
           
            //WHY THE FUCK DOES IT NOT WORK HERE BUT WORK THERE 
            //ITS THE SAME FUCKING CODE 
            


            --Index;
            if (Index == -1)
            {
                Index = 0;
                isIndexNegative = true;
            }
            Inputbuffer[Index] = '\0';
                       if(isIndexNegative)
                Document::BackSpace();
                

           // Inputbuffer[Index + 1] = '\0';
        }
        else
        {
            isIndexNegative = false;
            Inputbuffer[Index++] = (wchar_t)wParam;
            if (Index == InputSize - 1) Index = 0;
        }
        
            Inputbuffer[Index + 1] = '\0';
            //Inputbuffer[Index + 2] = '\0';
        



           
       
        if (input == L'\r')//carriage return enter key
        {
            if (!Page::SetUpDone)
            {
                int size= strToInt(Inputbuffer);


                if (size < 0)
                {
                    ClearBuffer(Inputbuffer, InputSize);
                    copyString(L"Invalid input\0", Inputbuffer, InputSize);
                    return -1;
                }//handle invalid inputs

                if (Page::CurrentSetUpVar == 0)
                {
                    Page::cols = size;
                    Page::CurrentSetUpVar++;

                }
                else if (Page::CurrentSetUpVar == 1)
                {
                    Page::lines = size;
                    Page::CurrentSetUpVar++;
                }
                else
                {
                    Page::charsPerLine = size;
                    Page::SetUpDone = true;
                    Document::doc = new Document[Document::DocumentCount];
                }
                
            }//set up Page
            


       /* else
        {
            int k = 0;
            int l = 0;
            while (Inputbuffer[k] != '\0' && Inputbuffer[k] != '\r')
            {
                Wordbuffer[l++] = Inputbuffer[k++];
                if (Inputbuffer[k] == ' ' || Inputbuffer[k] == '\0' || Inputbuffer[k] == '\r') {

                    Wordbuffer[l] = '\0';

                    for (int i = 0; i < l; i++)
                        Document::doc[Document::currentDoc].insertChar(Wordbuffer[i], getLength(Wordbuffer + i));

                    if (Inputbuffer[k] == ' ') {
                        Document::doc[Document::currentDoc].insertChar(L' ', 1);
                        k++;
                    }

                    l = 0;
                    ClearBuffer(Wordbuffer, wordSize);
                }
            }
        }*/

            else
            {
                Page* CurrentPage = Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage());
                Col* CurrentCol = &CurrentPage->colsPtr[CurrentPage->currentCol];

                Document::doc[Document::currentDoc].FlushWord(WordBuffer, WordLen);
                Document::doc[Document::currentDoc].newLine(); //literal ball torture right here


                WordLen = 0;
                ClearBuffer(WordBuffer, WordBufferSize);
                ClearBuffer(Inputbuffer, InputSize);
            }

            ClearBuffer(Inputbuffer, InputSize);
            Index = 0;

        } //if enter key pressed


        else if (Page::SetUpDone)
        {
            Page* CurrentPage = Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage());
            Col* CurrentCol = &CurrentPage->colsPtr[CurrentPage->currentCol];
            Line* CurrentLine = &CurrentCol->lines[CurrentCol->linesFilled];

            if (input == L' ')
            {
                isIndexNegative = false;
                Document::doc[Document::currentDoc].FlushWord(WordBuffer, WordLen);

                if (CurrentLine->length < Page::charsPerLine)
                    Document::doc[Document::currentDoc].insertChar(L' ');

                WordLen = 0;
                ClearBuffer(WordBuffer, WordBufferSize);
               
                Index = 0;
                ClearBuffer(Inputbuffer, InputSize);
                
            }
            else if (input == VK_BACK)
            {
                if (WordLen > 0)
                    WordBuffer[--WordLen] = L'\0';//why 
               
            }
            else
            {
                
                WordBuffer[WordLen++] = input;
                WordBuffer[WordLen] = L'\0';
            }
        }
 
        //im gonna fking cry x2



        
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


/**/

void AutoSave()//dont fucking touch this func anymore 
{ //fuck saving space everyone has 1tb hard drives now anyways
    static int SaveCounter = 0;
    const wchar_t* IndexFileName = L"AutoSaveIndex.txt\0";
    HANDLE IndexFile = CreateFileW(IndexFileName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,nullptr);

    if (IndexFile != INVALID_HANDLE_VALUE)
    {
        unsigned long FileSize = GetFileSize(IndexFile, NULL); //what in the fk does high order double word even fucking mean
        // returns FileSize in bytes
        // unsigned Long =Dword 

        if (FileSize == INVALID_FILE_SIZE)
        {
            CloseHandle(IndexFile);
            return;
        }

        wchar_t Lines[11][100] = { 0 };

        wchar_t* SaveCountStr = intToStr(SaveCounter);
        copyString((const wchar_t*)SaveCountStr, (wchar_t*)Lines[0],100);
        wchar_t* buffer = new wchar_t[FileSize / 2+1]; //File contains w chars each contains 2 bytes 
                                                     //File Size is the size of files in bytes
                                                     // = chars*2
        unsigned long bytesRead;
        int linesRecorded=0;

    
        if (FileSize == 0) {
            linesRecorded = 1; //make sure code doesnt shit itself when first creating index
        }

        if (!ReadFile(IndexFile, buffer, FileSize, &bytesRead, NULL)) //both params cant be null but it works for some reason
        {
            delete[] buffer;
            CloseHandle(IndexFile);
            return;
        }
        if (FileSize > 0) {
            buffer[FileSize / 2] = L'\0';
        }


        int i = 0,j=0;
        int lineNumber = 0;
        while (buffer[i] != '\0' && lineNumber < 11)
        {
            Lines[lineNumber][j++] = buffer[i];

            if (buffer[i] == '\n') 
            {
              Lines[lineNumber][j] = '\0';
              lineNumber++;
              j = 0;
              linesRecorded++;
            }


            i++;
        }//after this loop lines contain the names of files in Index and lines[0] stores autoSaveCount

        if (j > 0) {
            Lines[lineNumber][j] = '\0';
            linesRecorded++;
        }

        SaveCounter = strToInt((const wchar_t*)Lines[0]);

        if (linesRecorded == 11) //delete oldest file 
        {
            wchar_t deleteFile[100];
            
            int i = 0;
            int check;
            do
            {
                deleteFile[i] = Lines[1][i++];
                 check = i - 1 >= 0 ? i - 1 : 0;

            } while (Lines[1][check] != '\0');


            if (!DeleteFileW((wchar_t*)deleteFile)) return; //if delete fails return 


            i = 1;
            while (i < 10) //shift arr
            {
                copyString((const wchar_t*)Lines[i + 1], (wchar_t*)Lines[i], 100);
                i++;
            }
            linesRecorded = 10;
        }

        //save current doc[currentDoc] 

        

       

        
        wchar_t autoSaveName[100] = L"autosave_";
        wchar_t* NumStr = intToStr(SaveCounter+1);
        SaveCounter++;
        i = 9;

        for (int k = 0; NumStr[k] != L'\0'; k++)
            autoSaveName[i++] = NumStr[k]; //doesnt fking work 
        delete[] NumStr;

        autoSaveName[i++] = L'.';
        autoSaveName[i++] = L't';
        autoSaveName[i++] = L'x';
        autoSaveName[i++] = L't';
        autoSaveName[i] = L'\0';


        HANDLE SaveFile = CreateFileW((wchar_t*)autoSaveName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        

        int pageCount = Document::doc[Document::currentDoc].getPageCount();

        for (int currentPage = 0; currentPage < pageCount; currentPage++)
        {
            Col* colsptr = Document::doc[Document::currentDoc].getPagePtr(currentPage)->getColPtr();
            int colNumber = Page::cols;
            //colsptr is a ptr to all cols in the current page
            
            for (int currentCol = 0; currentCol < colNumber; currentCol++)
            {
                Line* Lines = colsptr[currentCol].lines;
                int LinesFilled = colsptr[currentCol].linesFilled;
                for(int currentLine=0;currentLine<LinesFilled;currentLine++)
                {
                    unsigned long bytesWritten;
                    int i = 0;
                    int lineLen = getLength(Lines[currentLine].chars);

                    if (lineLen == 0) {
                        unsigned long bytesWritten;
                        wchar_t newline = L'\n';
                        WriteFile(SaveFile, &newline, sizeof(wchar_t), &bytesWritten, NULL);
                        continue; 
                    }

                    wchar_t* lineBuffer = new wchar_t[lineLen+1];
                    lineBuffer[lineLen] = L'\0';
                    copyString(Lines[currentLine].chars, lineBuffer, lineLen);
                   
                    while (lineBuffer[i] != '\0')
                    {
                        i++;
                        if (lineBuffer[i] == '\0') {
                            lineBuffer[i] = '\n'; 
                            break;
                        }
                    }


                    WriteFile(SaveFile, lineBuffer,(lineLen+1)*sizeof(wchar_t), &bytesWritten, NULL);
                    delete[] lineBuffer;
                }

            }

        }


        CloseHandle(SaveFile);
        int k;
        for (k = 0; autoSaveName[k] != '\0'; k++)
            Lines[linesRecorded][k] = autoSaveName[k];
        Lines[linesRecorded][k] = '\0';
        linesRecorded++;


        //this is fucked
        SetFilePointer(IndexFile, 0, NULL, FILE_BEGIN);
        SetEndOfFile(IndexFile); //wipe file 
        const wchar_t* saves= intToStr(SaveCounter);
        copyString(saves, (wchar_t*)Lines[0], 100);
        delete[] saves;
        //this is fking garbage 
        for (int i = 0; i < linesRecorded; i++)
        {
            if (Lines[i][0] == L'\0') continue;
            unsigned long BytesWritten;
            int j= 0;
            if (j > 10) break;
            while (Lines[i][j] != '\0') j++;
            Lines[i][j] = '\n';

            WriteFile(IndexFile, Lines[i], (j + 1)*sizeof(wchar_t), &BytesWritten, NULL);
            Lines[i][j] = '\0';
        }

    }// orignal valid handle check 

    


    CloseHandle(IndexFile);
}

