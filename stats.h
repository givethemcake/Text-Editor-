#pragma once
#include<windows.h>

struct Stats
{
    int NumberOfChars;
    int NumberOfWords;
    int NumberOfSpaces;
    int NumberOfSentences;

    Stats();
    int CountWords(wchar_t delim = L' ');
    int CountSentences(wchar_t delim = L'.');
    int CountChars();
    int CountSpaces(wchar_t delim = L' ');
    void BuildStats();
    static void PaintStats(HDC& hdc, PAINTSTRUCT& ps, HWND& hWnd);

};
