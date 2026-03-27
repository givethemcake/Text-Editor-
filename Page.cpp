#include"Page.h"
#include "Helpers.h"
#include "Document.h"


Page::Page()
{
	colsPtr = nullptr;
	this->currentCol = 0;

	initializeCols();
}

//Page::Page(const Page& other)
//{ 
//	this->colsPtr=new Col[Page]
//}

Page::~Page () {
	if (colsPtr != nullptr) delete[] colsPtr;
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