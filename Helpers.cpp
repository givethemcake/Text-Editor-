#include "Helpers.h"
#include "Document.h"
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





