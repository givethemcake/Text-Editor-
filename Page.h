#pragma once


struct Line {
	wchar_t* chars;
	int length;//index of current char


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
	//Page(int cols, int lines);
	//Page(int cols, int lines, int ch); 

};