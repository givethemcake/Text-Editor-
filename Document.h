#pragma once
#include "Page.h"

class Document {
private:
	int currentPage;
	int pageCount;
	Page** Pages;
	


public:

	static Document* doc;//gonna have to change this maybe make it point to current doc 
	static int CurrentAlignment;// 0 left 1 center 2 right
	static int DocumentCount;
	static int maxDocs;
	static int currentDoc;

	int getPageCount();
	Page* getPagePtr(int index);
	Document(int PageCount=1);
	bool insertChar(wchar_t ch);
	void FlushWord(wchar_t* WordBuffer, int WordLen);
	int getCurrentPage();
	void addPage();
	void newLine();
	static void BackSpace();
	~Document();
};