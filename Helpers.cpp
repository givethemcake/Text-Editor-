#include "Helpers.h"
#include "Document.h"
#include "stats.h"
#include "SpellCheck.h"

int strToInt(const wchar_t* str) {
    int i = 0;
    int result = 0;
    int sign = 1;

    if (str[i] == '-') {
        sign = -1;
        i++;
    }

    while (str[i] != '\0'&& str[i]!='\r' && str[i]!='\n') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
        else {
            return result;//-1 fucks up autoSave
        }
        i++;
    }

    return result * sign;
}

void ClearBuffer(wchar_t* str, int size)
{
    for (int i = 0; i < size; i++) str[i] = '\0';
}

int getLength(const wchar_t* str)
{
    if (str == nullptr) return 0;

    int i = 0;
    while (str[i] != '\0') i++;
    return i;
}

void copyString(const wchar_t* str,wchar_t* Buffer,int size) {

    if (!str)
        return;

    for (int i = 0; i < size /*&& str[i-1]!='\0'*/; i++)
    {
        *(Buffer + i) = *(str + i);
        if (Buffer[i] == '\0') return;
    }


}

wchar_t* intToStr(int num)
{

    int temp = num;
    int i = 0;

    if (num == 0) {
        wchar_t* zero = new wchar_t[2];
        zero[0] = L'0';
        zero[1] = L'\0';
        return zero;
    }


    while (temp != 0)
    {
        i++;
        temp /= 10;
    }

    wchar_t* str = new wchar_t[i + 1];
    temp = i;
    i--;
    while (num != 0 && i > -1)
    {
        *(str + i) = num % 10 + L'0';
        num /= 10;
        i--;
    }

    str[temp] = L'\0';
    return str;
}
void SpellCheckFunc(HDC& hdc, wchar_t* renderBuffer, int bufferIndex, int x, int y)
{


    int i = 0;
    bool inWord = false;
    int wordStart = 0;
    int wordLen = 0;
    wchar_t wordBuf[100] = {};

    while (i < bufferIndex)
    {
        wchar_t ch = renderBuffer[i];

        if (ch != L' ' && ch != L'\0')
        {
            if (!inWord)
            {
                wordStart = i;
                wordLen = 0;
                inWord = true;
            }

            wordBuf[wordLen] = ch;
            wordLen++;
        }   
        else if (inWord) //we were in word but not anymore
        {
            wordBuf[wordLen] = L'\0';

            wchar_t lower[100] = {};
            for (int w = 0; w < wordLen; w++)
                lower[w] = (wordBuf[w] >= L'A' && wordBuf[w] <= L'Z') ? wordBuf[w] + 32 : wordBuf[w];
            lower[wordLen] = L'\0';
            
            if (!SpellCheck::dictionary->isWord(lower)) //if not a word do this
            {
                SIZE before, wordSize;
                GetTextExtentPoint32W(hdc, renderBuffer, wordStart, &before);
                GetTextExtentPoint32W(hdc, renderBuffer + wordStart, wordLen, &wordSize);

                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, RGB(180, 0, 0));
                TextOutW(hdc, x + before.cx, y, renderBuffer + wordStart, wordLen);
                SetBkMode(hdc, TRANSPARENT);
            }

            inWord = false;
            wordLen = 0;
            wordBuf[0] = L'\0';
        }

        i++;
    }
}
bool areEqual(const wchar_t* s1, const wchar_t* s2) {


    if (!s1 || !s2) //check for nullptrs
        return false;

    int size1 = getLength(s1);
    int size2 = getLength(s2);
    if (size1 != size2)
        return false;
    else
        for (int i = 0; i < size1; i++)
            if (*(s1 + i) != *(s2 + i))
                return false;


    return true;

}
int findSubstring(const wchar_t* str, wchar_t* substring) {

    int subSize = getLength(substring);
    int size = getLength(str);

    if (subSize == 0)
        return -1;

    bool foundMatch = false;
    int startIndex = 0;

    for (int i = 0; i < size; i++) {
        int j = 0;
        if (*(str + i) == *(substring + j)) {
            startIndex = i;
            for (j = 0; j < subSize; j++, i++) {

                if (*(str + i) != *(substring + j)) {
                    i -= 1;
                    break;

                }

                if (j + 1 == subSize)
                    foundMatch = true;
            }//j loop
        }//if statement

        if (foundMatch)
            return startIndex;
    }//i loop

    if (!foundMatch)
        return -1;


}
void ResizeArr(int*& arr, int& size)
{
    int* newArr = new int[size + 1];
    for (int i = 0; i < size; i++)
        newArr[i] = arr[i];
    size++;
    delete arr;
    arr = newArr;
}
void Searchfunc(HDC& hdc, wchar_t*& Inputbuffer, int& InputSize, Col* Coll, int i, int k, int j, wchar_t* renderBuffer, int& renderSize, int& x, int& y, int& bufferIndex)
{




    TextOutW(hdc, 50, 300, L"Enter String to find\0", 22);
    int searchBufferSize = 500;
    wchar_t* searchBuffer = new wchar_t[searchBufferSize];
    copyString(Inputbuffer, searchBuffer, 500);
    int indexSize = 1;
    int* indicies = new int[indexSize];
    int offset = 0;
    offset = findSubstring(Coll[j].lines[k].chars, searchBuffer);
    int searchLength = getLength(searchBuffer);
    int iteration = 1;
    bool fuckThisShit = false;
    indicies[0] = offset;


    if (offset != -1)//if we found a match keep looking for more matches
        while (offset + searchLength <= Page::charsPerLine && offset != -1)
        {
            //offset = (offset + searchLength >= lineLen) ? -1 : offset + searchLength + findSubstring(Coll[j].lines[k].chars + offset + searchLength, searchBuffer);
            int tempOffset = findSubstring(Coll[j].lines[k].chars + offset + searchLength, searchBuffer);
            if (tempOffset == -1) break;
            offset = offset + searchLength + tempOffset;
            if (offset != -1)
            {
                // if (fuckThisShit) //only resize if the first index is already full
                ResizeArr(indicies, indexSize);
                //makes sure size is exact


                indicies[indexSize - 1] = offset;


                fuckThisShit = true;
            }
            iteration++;
        }


    for (int i = 0; i < indexSize; i++)
        if (indicies[i] != -1)
        {
            //im actually gonna fking cry bro 
            SIZE size, fuckmyLife;

            GetTextExtentPoint32W(hdc, renderBuffer, indicies[i], &size);
            GetTextExtentPoint32W(hdc, renderBuffer + indicies[i], getLength(searchBuffer), &fuckmyLife);

            TextOutW(hdc, x, y, renderBuffer, indicies[i]);
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, 0xFF7F7F);//pinkish red

            TextOutW(hdc, x + size.cx, y, renderBuffer + indicies[i], getLength(searchBuffer));
            SetBkMode(hdc, TRANSPARENT);

            TextOutW(hdc, x + size.cx + fuckmyLife.cx, y, renderBuffer + indicies[i] + getLength(searchBuffer), (bufferIndex)-(indicies[i] + getLength(searchBuffer)));
        }
    delete searchBuffer;
    delete[] indicies;
    indicies = nullptr;
    searchBuffer = nullptr;
    //Search = false;
}
void SetUpPage(HDC& hdc)
{
    if (Page::CurrentSetUpVar == 0)
        TextOutW(hdc, 50, 50, L"Enter value for number of cols on a page: ", 43);
    else if (Page::CurrentSetUpVar == 1)
        TextOutW(hdc, 50, 50, L"Enter value for number of Lines on a page: ", 44);
    else
        TextOutW(hdc, 50, 50, L"Enter value for number of charachters per Line on a page: ", 59);
}
void paint(HDC& hdc,PAINTSTRUCT& ps, HWND& hWnd, wchar_t* Inputbuffer, int InputSize)
{
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


    if (!Page::SetUpDone) {
        SetUpPage(hdc);
    }
    if (!Page::SetUpDone)   //output the buffer 
        TextOutW(hdc, 50, 100, Inputbuffer, InputSize);

    //L means wide String treat every char as 2 bytes
    // LPCWSTR doesnt work cuz  LPCWSTR is a pointer type
   // if(typed)



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

            //  if(GetKeyState(VK_MENU) & 0x8000)
            //  Document::doc[Document::currentDoc].getPagePtr(i)->BuildTable();

            for (int j = 0; j < colCount; j++)
            {
                //int numberOfgetFails = 0;
                Col* Coll = Document::doc[Document::currentDoc].getPagePtr(i)->getColPtr();
                int lineCount = Coll[j].linesFilled;
                for (int k = 0; k <= lineCount; k++)
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
                    else if (alignMode == 0) // left aligned
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
                        Searchfunc(hdc, Inputbuffer, InputSize, Coll, i, k, j, renderBuffer, bufferIndex, x, y, bufferIndex);
                    }
                    else if(1)
                        SpellCheckFunc(hdc, renderBuffer, bufferIndex, x, y);

                    TextOutW(hdc, x, y, renderBuffer, bufferIndex);
                    Bufferx = x;
                    buffery = y;
                    GetTextExtentPoint32W(hdc, renderBuffer, getLength(renderBuffer), &AHHH);

                    /* if (!(getLength(Coll[j].lines[k].chars)>2))
                           numberOfgetFails++;

                   if(numberOfgetFails==1)
                       TextOutW(hdc, x+1, y, Inputbuffer, InputSize);*/
                }

                if (!Search && i == Document::doc[Document::currentDoc].getCurrentPage() && j == Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->currentCol)
                    TextOutW(hdc, Bufferx + AHHH.cx, buffery, Inputbuffer, getLength(Inputbuffer));
            }


        }

        Stats::PaintStats(hdc, ps, hWnd);
  
    }

    DeleteObject(font);
    EndPaint(hWnd, &ps);
}
void AutoSave()//dont fucking touch this func anymore 
{ //fuck saving space everyone has 1tb hard drives now anyways
    static int SaveCounter = 0;
    const wchar_t* IndexFileName = L"AutoSaveIndex.txt\0";
    HANDLE IndexFile = CreateFileW(IndexFileName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

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
        copyString((const wchar_t*)SaveCountStr, (wchar_t*)Lines[0], 100);
        wchar_t* buffer = new wchar_t[FileSize / 2 + 1]; //File contains w chars each contains 2 bytes 
        //File Size is the size of files in bytes
        // = chars*2
        for (int i = 0; i < FileSize / 2 + 1; i++) buffer[i] = '\0';
        unsigned long bytesRead;
        int linesRecorded = 0;


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


        int i = 0, j = 0;
        int lineNumber = 0;
        while (buffer[i] != '\0' && lineNumber < 11)
        {
            //Lines[lineNumber][j++] = buffer[i];

            if (buffer[i] == '\n')
            {
                Lines[lineNumber][j] = '\0';
                lineNumber++;
                j = 0;
                linesRecorded++;
            }
            else
            {
                Lines[lineNumber][j++] = buffer[i];
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
            copyString(Lines[1], deleteFile, 100);
            int i = 0;
            /*int check;
            do
            {
                deleteFile[i] = Lines[1][i++];
                 check = i - 1 >= 0 ? i - 1 : 0;

            } while (Lines[1][check] != '\0');*/


            if (!DeleteFileW((wchar_t*)deleteFile))
            {
                CloseHandle(IndexFile);
                return; //if delete fails return 
            }


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
        wchar_t* NumStr = intToStr(SaveCounter + 1);
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
                for (int currentLine = 0; currentLine < LinesFilled; currentLine++)
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

                    wchar_t* lineBuffer = new wchar_t[lineLen + 1];
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


                    WriteFile(SaveFile, lineBuffer, (lineLen + 1) * sizeof(wchar_t), &bytesWritten, NULL);
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
        const wchar_t* saves = intToStr(SaveCounter);
        copyString(saves, (wchar_t*)Lines[0], 100);
        delete[] saves;
        //this is fking garbage 
        for (int i = 0; i < linesRecorded; i++)
        {
            if (Lines[i][0] == L'\0') continue;
            unsigned long BytesWritten;
            int j = 0;
            if (j > 10) break;
            while (Lines[i][j] != '\0') j++;
            Lines[i][j] = '\n';

            WriteFile(IndexFile, Lines[i], (j + 1) * sizeof(wchar_t), &BytesWritten, NULL);
            Lines[i][j] = '\0';
        }

    }// orignal valid handle check 




    CloseHandle(IndexFile);
}
void ParseInput(WPARAM wParam,wchar_t* Inputbuffer, const int& InputSize, wchar_t* WordBuffer, int& WordLen, const int& WordBufferSize)
{
    if (GetKeyState(VK_CONTROL) & 0x8000) {
       // break;
        return;
    }


    bool Search = Document::doc != nullptr && Page::SetUpDone && Document::doc[Document::currentDoc].getPagePtr(Document::doc[Document::currentDoc].getCurrentPage())->searching;
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
        if (isIndexNegative)
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
            int size = strToInt(Inputbuffer);


            if (size < 0)
            {
                ClearBuffer(Inputbuffer, InputSize);
                copyString(L"Invalid input\0", Inputbuffer, InputSize);
                return;
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


    else if (Page::SetUpDone && !Search)
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

}
void buildTabeOfContents(HDC& hdc,HWND& hWnd)
{ 
    RECT rect;
    GetClientRect(hWnd, &rect);

    //st bk
    HBRUSH bgBrush = CreateSolidBrush(RGB(20, 20, 40));
    FillRect(hdc, &rect, bgBrush);
    DeleteObject(bgBrush);

    //set text settings
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    //set font
    HFONT font = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Trebuchet MS");
    SelectObject(hdc, font);

    Document* CurrentDoc = &(Document::doc[Document::currentDoc]);
    int PageCount = CurrentDoc->getPageCount();
    int x = rect.right / 3;
    int y = 100;

    SIZE FML;
    GetTextExtentPoint32W(hdc, L"TABLE OF CONTENTS\0", 19, &FML);

    TextOutW(hdc, x, y, L"TABLE OF CONTENTS\0", 19);
    TextOutW(hdc, x, y+FML.cy, L"-----------------------------------------\0", 43);
   
    for (int i = 0; i < PageCount; i++)
    {
        SIZE s;
        SIZE S_numstr;
        SIZE S_Format;
        SIZE space;
        wchar_t spacech = L' ';
        wchar_t* numstr=intToStr(i+1);
        wchar_t Format[20] = L"..............Page\0";\
        GetTextExtentPoint32W(hdc,numstr, getLength(numstr), &S_numstr);
        GetTextExtentPoint32W(hdc, Format, getLength(Format), &S_Format);
        GetTextExtentPoint32W(hdc, &spacech, 1, &space);




        CurrentDoc->getPagePtr(i)->BuildTable();
        GetTextExtentPoint32W(hdc, CurrentDoc->getPagePtr(i)->TableOfContents, getLength(CurrentDoc->getPagePtr(i)->TableOfContents),&s);
        TextOutW(hdc,x, y + ((i + 2) * s.cy),numstr,getLength(numstr));
        TextOutW(hdc, x+S_numstr.cx+space.cx, y + ((i + 2) * s.cy), CurrentDoc->getPagePtr(i)->TableOfContents, getLength(CurrentDoc->getPagePtr(i)->TableOfContents));
        TextOutW(hdc, x+S_numstr.cx+ space.cx+s.cx, y + ((i + 2) * s.cy), Format, 20);
        TextOutW(hdc, x + S_numstr.cx + space.cx + s.cx+ S_Format.cx, y + ((i + 2) * s.cy), numstr, getLength(numstr));

        delete[] numstr;
    }

   

    DeleteObject(font);
    //deletion is handled by buildtable and the destructor of the page
    
    return;
    
}