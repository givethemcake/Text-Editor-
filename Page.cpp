#include"Page.h"
#include "Helpers.h"
#include "Document.h"


Page::Page()
{
	colsPtr = nullptr;
	this->currentCol = 0;
	TableOfContents = nullptr;

	initializeCols();
}

//Page::Page(const Page& other)
//{ 
//	this->colsPtr=new Col[Page]
//}

Page::~Page () {
	if (colsPtr != nullptr) delete[] colsPtr;
	if (!this->TableOfContents) delete[] TableOfContents;
}

void Page::initializeCols()
{
	this->colsPtr = new Col[Page::cols];
	Page::searching = false;
	
}

Col::Col()
{
	this->lines = new Line[Page::lines];
	this->linesFilled = 0;
}

Col::~Col() {
	if (lines != nullptr) delete[] lines;
}

Line::Line()
{
	this->currentAlignment = Document::CurrentAlignment;
	this->chars = new wchar_t[Page::charsPerLine+1];
	for (int i = 0; i < Page::charsPerLine; i++)
		chars[i] = '\0';
	this->length = 0;
}

Line::~Line()
{
	if (chars) delete[] chars;
}

bool Line:: insertChar(wchar_t ch) {
	if (this->length == Page::charsPerLine) return false; 
	else
	{
		this->chars[length++] = ch;
		this->chars[length] = L'\0';
	}
	return true;
}

bool Col::insertChar(wchar_t ch) {

	while(!lines[this->linesFilled].insertChar(ch))
	{
		//if line insert fails
		if (this->linesFilled == Page::lines - 1) return false;
		else linesFilled++;
	
	}
	return true;
}

int Page::getColCount() { return Page::cols; }
Col* Page::getColPtr() { return colsPtr; }


bool Page::insertChar(wchar_t ch) {

	while (!colsPtr[this->currentCol].insertChar(ch))
	{
		//if col insert fails
		if (this->currentCol == Page::cols-1) return false;
		else currentCol++;

	}
	return true;
}

void CountWords(wchar_t* buffer,int& wordCount)
{
	int i = 0;
	bool inWord = false;
	wchar_t delim = L' ';

	while (buffer[i] != L'\0')
	{

		if (buffer[i] != delim)
		{
			if (!inWord)
			{
				wordCount++;
				inWord = true;
			}//do nothing if we were already inside a word
		}
		else // if space found
		{
			inWord = false;
		}

		i++;
	}
}

bool isTerminal(wchar_t* buffer)
{
	wchar_t delim = '.';
	int i = 0;
	int spaces = 0;
	while (buffer[i]!= L'\0')
	{
		if (buffer[i] == delim || buffer[i] == L'!' || buffer[i] == L'?')
		{
			wchar_t nextChar = buffer[i + 1];

			if (nextChar == L' ' || nextChar == L'\0')
				spaces++;
		}
		i++;
	}
	
	if (spaces > 0) return true; //found terminal 
	else return false;

}

bool hasComma(wchar_t* buffer) {
	int i = 0;
	while (buffer[i] != L'\0')
	{
		if (buffer[i++] == ',') return true;
	}
	return false;
}

bool isCapitalized(wchar_t* buffer) {

	int totalWords = 0;
	int capitals = 0;
	bool inWord = false;
	int i = 0;

	while (buffer[i] != L'\0') {
		if (buffer[i] != L' ') {

			if (!inWord) {
				totalWords++;
				if (buffer[i] >= 65 && buffer[i] <= 90)
					capitals++;
			
				inWord = true;
			}
		}
		else {
			inWord = false;
		}
		i++;
	}

	//this is so fucked

	return (capitals * 2 > totalWords) ? true : false;
}



int extractWords(wchar_t* buffer, int& stringlen)
{
	//int totalWords = 0;
	//int capitals = 0;
	//bool inWord = false;
	int i = 0;
	int startIndex=-1;
	int len=0;
	bool inWord = false;

	while (buffer[i] != L'\0') {
		
		if (buffer[i] != L' ')
		{
			if (!inWord)
			{
				startIndex = i;
				inWord = true;
			}
			len++;
		}
		else
		{
			stringlen = len;
			return startIndex;
		}

		i++;
	}
}

//wchar_t* operator+(const wchar_t* str1, const wchar_t* str2)
//{
//	return concatenate(str1, str2);
//}

bool caseComp(wchar_t* s1, const wchar_t* s2)
{
	if (!s1 || !s2) //check for nullptrs
		return false;

	int i = 0;
	while (s1[i] != '\0')
	{
		if (s1[i] >= 65 && s1[i] <= 90) s1[i] += 32;
		i++;
	}

	 i = 0;
	/*while (s2[i] != '\0')
	{
		if (s2[i] > 65 && s2[i] < 90) s2[i] += 32;
	}*/

	int size1 = getLength((const wchar_t*)s1);
	int size2 = getLength((const wchar_t*)s2);
	if (size1 != size2)
		return false;
	else
		for (int i = 0; i < size1; i++)
			if (*(s1 + i) != *(s2 + i))
				return false;


	return true;
}

wchar_t* concatenate(const wchar_t* str1, const wchar_t* str2) {

	if (!str1 && !str2)// if both are nullptrs
		return nullptr;


	int sizeStr1 = getLength(str1);
	int sizeStr2 = getLength(str2);

	int size = sizeStr1 + sizeStr2;

	wchar_t* concatString = new wchar_t[size + 1];

	int i = 0;

	//----copy string 1
	for (i; i < sizeStr1; i++)
		*(concatString + i) = *(str1 + i);



	//----copy string 2
	int j = 0;
	for (j; j < sizeStr2; i++, j++)
		*(concatString + i) = *(str2 + j);


	*(concatString + size) = L'\0';
	return concatString;
}//ahhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

void Page::BuildTable() 
{
	if (!this->TableOfContents)
		delete[] TableOfContents;

	int bufferSize = Page::charsPerLine;
	wchar_t* buffer = new wchar_t[bufferSize];
	wchar_t** Words = new wchar_t* [4];
	bool isStop = false;
	copyString(this->colsPtr->lines->chars,buffer,bufferSize);	
	//mkc iska edgecases ki 
	int points = 0;
	int wordCount=0;
	int charCount = this->colsPtr->lines->length;
	CountWords(buffer, wordCount);

	if (wordCount < 9) points += 2;
	if (charCount < 60) points++;
	if (!isTerminal(buffer)) points += 2;
	if (!hasComma(buffer))points++;
	if (isCapitalized(buffer)) points += 2;
	if (points >= 5) {
		this->TableOfContents = buffer;
		return;
	}
	else
	{
		//extract the first 4 words 
		int wordCount = 0;
		CountWords(buffer,wordCount);
		int startIndex[4] = { 0 };
		
		if (wordCount >= 4)
		{
			int offset=0;
			for (int i = 0; i < 4; i++)
			{
				int length=0;
				extractWords(buffer + offset, length);
				Words[i] = new wchar_t[length+1];
				copyString(buffer + offset,Words[i],length);
				Words[i][length] = L'\0';
				offset += length+1;//+1 for space

			}
		}
		else
		{
			Line* fuck = (this->colsPtr->lines);
			int WordsRecorded = wordCount;
			do {
				

				int offset = 0;
				for (int i = WordsRecorded-wordCount; i <wordCount; i++)
				{
					int length=0;
					extractWords(buffer + offset, length);
					Words[i] = new wchar_t[length+1];
					copyString(buffer + offset, Words[i], length);
					Words[i][length] = L'\0';
					offset += length + 1;//+1 for space

				}
				WordsRecorded += wordCount;
				fuck++;
				copyString(fuck->chars, buffer, bufferSize);
				CountWords(buffer, wordCount);

			} while (WordsRecorded<4);
		}
	}//fuck my life (else block)

	//build this->TableOfContents

	if (caseComp(Words[3], L"was")) isStop = true;
	else if (caseComp(Words[3], L"is")) isStop = true;
	else if (caseComp(Words[3], L"and")) isStop = true;
	else if (caseComp(Words[3], L"the")) isStop = true;
	int stopCondition = isStop ? 3 : 4;

	int size = 0;
	for (int i = 0; i < stopCondition; i++) size += getLength(Words[i]);

	//TableOfContents = new wchar_t[size-4];//acount for all the null chars;
	for (int i = 0; i < stopCondition; i++)
	{
		if (Words[i][0] >= 97 && Words[i][0] <= 122)
			Words[i][0] -= 32;
	}

	this->TableOfContents = Words[0];
	wchar_t* temp = TableOfContents;
	TableOfContents = concatenate(TableOfContents, L" ");
	delete[] temp;
	for (int i = 1; i < stopCondition; i++)
	{
		wchar_t* temp = TableOfContents;
		TableOfContents = concatenate(TableOfContents, Words[i]);
		delete[] temp; //deletes Words[i] on first run
		temp = TableOfContents;
		TableOfContents = concatenate(TableOfContents, L" ");
		delete[] temp;
	}

	for (int i = 1; i < 4; i++) delete Words[i];
	delete Words;
	delete buffer;

}

//Page::Page(int cols, int lines, int ch){
//	this->cols = cols;
//	this->lines = lines;
//	charsPerLine = ch;
//}
//
//Page::Page(int cols, int lines) {
//	this->cols = cols;
//	this->lines = lines;
//	charsPerLine = 40;
//}
//
//Page::Page(int cols) {
//	this->cols = cols;
//	lines = 20;
//	charsPerLine = 40;
//}