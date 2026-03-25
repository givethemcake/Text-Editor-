#pragma once
#include "Page.h"

class Document {
private:
	int currentPage;
	int pageCount;
	Page** Pages;
	int currentDoc;

public:
	int getPageCount();
	Page* getPagePtr(int index);
	static Document* doc;
	Document(int PageCount=1);
	bool insertChar(wchar_t ch);
	void FlushWord(wchar_t* WordBuffer, int WordLen);
	int getCurrentPage();
	void addPage();
	~Document();
};