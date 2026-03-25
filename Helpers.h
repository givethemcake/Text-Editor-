#pragma once

int strToInt(const wchar_t* str);
void ClearBuffer(wchar_t* str, int size);
int getLength(const wchar_t* str);
void copyString(const wchar_t* str, wchar_t* Buffer, int size);
wchar_t* intToStr(int num);
bool areEqual(const wchar_t* s1, const wchar_t* s2);
void AutoSave();//def in q7.cpp


