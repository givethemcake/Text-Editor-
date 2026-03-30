#include"stats.h"
#include"Document.h"
#include "Helpers.h"
#include "Windows.h"

int Stats::CountSentences(wchar_t delim )
{
        if (!Page::SetUpDone) return -1;

        //count spaces
        int spaces = 0;

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
                for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
                {
                    int i = 0;
                    while (colsptr[currentCol].lines[currentLine].chars[i] != L'\0')
                    {
                        if (colsptr[currentCol].lines[currentLine].chars[i] == delim || colsptr[currentCol].lines[currentLine].chars[i] == L'!' || colsptr[currentCol].lines[currentLine].chars[i] == L'?')
                        {
                            wchar_t nextChar = colsptr[currentCol].lines[currentLine].chars[i + 1];

                            if (nextChar == L' ' || nextChar == L'\0')
                                spaces++;
                        }
                        i++;
                    }
                    //spaces++;//catch last word for each line 
                    //strictly only count full stops to indicate end of sentences
                }

            }

        }

     return spaces;
}


int Stats::CountWords(wchar_t delim )
{

    if (!Page::SetUpDone) return -1;

    //count spaces
    int spaces = 0;



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
            for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
            {
                int i = 0;
                bool inWord = false;
                while (colsptr[currentCol].lines[currentLine].chars[i] != L'\0')
                {

                    if (colsptr[currentCol].lines[currentLine].chars[i] != delim)
                    {
                        if (!inWord)
                        {
                            spaces++;
                            inWord = true;
                        }//do nothing if we were already inside a word
                    }
                    else // if space found
                    {
                        inWord = false;
                    }

                    i++;
                }
                //spaces++;//catch last word for each line 
            }

        }

    }

    return spaces;
}


int Stats::CountChars()
{

    if (!Page::SetUpDone) return -1;

    //count chars
    int chars = 0;

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
            for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
            {
                chars += colsptr[currentCol].lines[currentLine].length ;
                //colsptr[currentCol].lines[currentLine].length stores index of last char hence +1
                //nvm +1 fucks it
            }

        }

    }

    return chars;

}
int Stats::CountSpaces(wchar_t delim )
{

    if (!Page::SetUpDone) return -1;

    //count spaces
    int spaces = 0;


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
            for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
            {
                int i = 0;
                while (colsptr[currentCol].lines[currentLine].chars[i] != L'\0')
                {
                    if (colsptr[currentCol].lines[currentLine].chars[i] == delim) spaces++;
                    i++;
                }
                //spaces++;//catch last word for each line 
            }

        }

    }

    return spaces;
}

Stats::Stats()
{
    BuildStats();

}

void Stats::BuildStats()
{
    /*Display a persistent statistics panel showing real-time metrics. - Characters (with and without spaces)
    - Words
    - Sentences
    - Pages (based on the document's actual page count)
    - Estimated reading time (assume an average reading speed of 200 words per minute)
    These statistics update instantly as the user types or performs any modification to the text.*/
    //could make word parsing better 

    this-> NumberOfChars = CountChars();
    this->NumberOfWords = CountWords();
    this->NumberOfSpaces = CountSpaces();
    this->NumberOfSentences = CountSentences();

}

void Stats:: PaintStats(HDC& hdc, PAINTSTRUCT& ps, HWND& hWnd)
{
    //Display stats and footer

    RECT rect;
    GetClientRect(hWnd, &rect);

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

    const wchar_t* Str8 = L"s\0";
    j = 0;
    while (Str8[j] != L'\0') StatsBuffer[i++] = Str8[j++];

    StatsBuffer[i] = L'\0';

    int FooterX = 50;
    int FooterY = rect.bottom - 40;

    wchar_t CurrentPageNumber[12];

    j = 0;

    for (j = 0; j < 5; j++) CurrentPageNumber[j] = L'-';
    NumStr = intToStr(Document::doc[Document::currentDoc].getCurrentPage() + 1);
    for (int k = 0; NumStr[k] != '\0'; k++) CurrentPageNumber[j++] = NumStr[k];
    for (j; j < 12; j++) CurrentPageNumber[j] = L'-';
    CurrentPageNumber[11] = '\0';
    delete[] NumStr;




    wchar_t CurrentDoc[12] = { '\0' };
    const wchar_t FormatStr[5] = L"Doc[";


    for (j = 0; j < 4; j++) CurrentDoc[j] = FormatStr[j];
    NumStr = intToStr(Document::currentDoc + 1);
    for (int k = 0; NumStr[k] != '\0'; k++) CurrentDoc[j++] = NumStr[k];
    CurrentDoc[j++] = L'/';
    delete[] NumStr;
    NumStr = intToStr(Document::maxDocs);
    for (int k = 0; NumStr[k] != '\0'; k++) CurrentDoc[j++] = NumStr[k];
    delete[] NumStr;
    CurrentDoc[j++] = L']';
    CurrentDoc[11] = '\0';


    TextOutW(hdc, FooterX, FooterY, StatsBuffer, i);
    TextOutW(hdc, rect.right / 8, rect.bottom - 80, CurrentDoc, 12);
    TextOutW(hdc, rect.right / 2, rect.bottom - 80, CurrentPageNumber, 12);


}
