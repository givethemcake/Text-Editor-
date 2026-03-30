#pragma once
#include<Windows.h>
#include "Document.h"

int strToInt(const wchar_t* str);
void ClearBuffer(wchar_t* str, int size);
int getLength(const wchar_t* str);
void copyString(const wchar_t* str, wchar_t* Buffer, int size);
wchar_t* intToStr(int num);
bool areEqual(const wchar_t* s1, const wchar_t* s2);
int findSubstring(const wchar_t* str, wchar_t* substring);
void ResizeArr(int*& arr, int& size);
void paint(HDC& hdc, PAINTSTRUCT& ps, HWND& hWnd,wchar_t* Inputbuffer, int InputSize);
void SetUpPage(HDC& hdc);
void Searchfunc(HDC& hdc, wchar_t*& Inputbuffer, int& InputSize, Col* Coll, int i, int k, int j, wchar_t* renderBuffer, int& renderSize, int& x, int& y, int& bufferIndex);
void AutoSave();
void ParseInput(WPARAM wParam, wchar_t* Inputbuffer, const int& InputSize, wchar_t* WordBuffer, int& WordLen, const int& WordBufferSize);
void buildTabeOfContents(HDC& hdc, HWND& hWnd);