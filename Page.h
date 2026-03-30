#pragma once


struct Line {
	wchar_t* chars;
	int length;//index of current char
	int currentAlignment; //0 left 1 center 2 right

	Line();
	bool insertChar(wchar_t ch);
	~Line();
};

struct Col {

	Line* lines;
	int linesFilled; //index of current line
	Col();
	bool insertChar(wchar_t ch);
	~Col();
};


struct Page
{
	static int cols;
	static int lines;
	static int charsPerLine;
	static bool SetUpDone;
	static int CurrentSetUpVar;
	wchar_t* TableOfContents;
	bool searching;
	Col* colsPtr;
	int currentCol;

	//constructors
	
	Page();
	//Page(const Page& other);
	~Page();
	bool insertChar(wchar_t ch);
	int getColCount();
	Col* getColPtr();
	void initializeCols();
	void BuildTable();
	//Page(int cols, int lines);
	//Page(int cols, int lines, int ch); 

};