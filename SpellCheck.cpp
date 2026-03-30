#include"SpellCheck.h"
#include <windows.h>
#include"Helpers.h"

void ignoreChar(char*& buffer, char delim, int& length)
{
	int len = 0;
	while (*buffer != '\0')
	{
		if (*buffer == delim)
		{
			length = len;
			return;
		}
		buffer++;
		len++;
	}
}

void resizeArr(wchar_t**& cantbebothered, long long int& size)
{

	if (size == 0) {
		size++;
		cantbebothered = new wchar_t* [1];
		cantbebothered[0] = new wchar_t[100];
		return;
	}


	size++;
	wchar_t** fml = new wchar_t* [size];

	for (int i = 0; i < size - 1; i++)
	{
		fml[i] = cantbebothered[i];
	}

	fml[size - 1] = new wchar_t[100];
	delete[] cantbebothered;
	cantbebothered = fml;

}

SpellCheck::SpellCheck()
{
	this->Words = nullptr;
	this->WordCount = 0;
	
}

SpellCheck::~SpellCheck() {

	if (this->Words != nullptr)
	{
		for (int i = 0; i < WordCount; i++) delete[] this->Words[i];
		delete[] this->Words;
	}
}
void SpellCheck::ReadFileDic()
{
	wchar_t FileName[50] = L"OPTED-Dictionary.csv";
	HANDLE Dictionary=CreateFileW(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,nullptr);
	

	if (!Dictionary)
	{
		CloseHandle(Dictionary);
		return;
	}

	
	
	long long int FileSize= GetFileSize(Dictionary, NULL);
	char* readBuffer = new char[FileSize+1];
	readBuffer[FileSize] = '\0';
	unsigned long int BytesRead;

	if (!ReadFile(Dictionary,readBuffer,FileSize,&BytesRead,nullptr))
	{
		delete[] readBuffer;
		CloseHandle(Dictionary);
		return;

	}
	

	if (this->Words != nullptr)
	{
		for (int i = 0; i < WordCount; i++) delete[] this->Words[i];
		delete[] this->Words;
	}
	this->WordCount =0 ;
	this->Words = new wchar_t* [200000];
	for (int i = 0; i < 200000; i++)
		this->Words[i] = new wchar_t[50];
	//start parsing
	
	int fileIndex=0;
	do
	{		int  length = 0;
		ignoreChar(readBuffer, '\n', length);//skip first line
		readBuffer++;//now on first char of 
		fileIndex += length + 1;

		ignoreChar(readBuffer, ',', length); //at the end of the word buffer points to ',' rn
		fileIndex += length + 1;// points to comma 
		//resizeArr(this->Words, this->WordCount);
		
		if (this->WordCount >= 200000) break;

		for (int i = 0; i < length; i++) //change to WordCount-1 if resizeArr is reimplemented
			this->Words[this->WordCount][i] = (wchar_t)(readBuffer - length)[i];
		this->Words[this->WordCount][length] = L'\0';
		this->WordCount++;

	} while (fileIndex < FileSize);

	for (long long int currentWord = 1; currentWord < this->WordCount; currentWord++)
	{
		wchar_t* key = this->Words[currentWord];
		long long int compWord = currentWord - 1;
		while (compWord >= 0)
		{
			int i = 0, cmp = 0;
			while (Words[compWord][i] != L'\0' && key[i] != L'\0')
			{
				if (key[i] < Words[compWord][i]) 
				{ 
				  cmp = -1;
				  break; 
				}
				if (key[i] > Words[compWord][i]) 
				{ 
				  cmp = 1; 
				  break;
				}
				i++;
			}
			if (cmp == 0)
				cmp = (key[i] == L'\0') ? ((Words[compWord][i] == L'\0') ? 0 : -1) : 1;

			if (cmp >= 0) break;//found pos

			this->Words[compWord + 1] = this->Words[compWord];//shit arr right
			compWord--;
		}
		this->Words[compWord + 1] = key; //place key
	}


	return;

}

//bool SpellCheck::isWord(wchar_t* target)
//{
//	if (target[0] >= 'a' && target[0] <= 'z')
//		target[0] -= 32;
//
//	for (int i = 0; i < WordCount; i++)
//		if (areEqual(Words[i], target)) return true;
//
//	return false;
//}

bool SpellCheck::isWord(wchar_t* target)
{
	if (target[0] >= 'a' && target[0] <= 'z')
		target[0] -= 32;
	
	int start = 0;
	int end = WordCount - 1;

	while (start <= end)
	{
		int mid = (start + end) / 2;
		int i = 0;
		int result = 0;

		
		while (Words[mid][i] != L'\0' && target[i] != L'\0')
		{



			if (target[i] < Words[mid][i]) 
			{
				result = -1;
				break; 
			}
			if (target[i] > Words[mid][i]) 
			{ 
				result = 1; 
				break; 

			}
			i++;
		}

		if (result == 0)
		{
			if (target[i] == L'\0' && Words[mid][i] == L'\0') return true;
			else if ( target[i] == L'\0') result = -1; //taget is smaller than word must be to the left
			else result = 1;
		}
	
		if (result < 0) end = mid - 1; //erase the second have of arr
		else start = mid + 1;//earse the first half of arr

		
	}

	
	
	return false;
}