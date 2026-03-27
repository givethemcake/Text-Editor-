#include "Document.h"


Document::Document(int PageCount) {
	
	this->currentPage = 0;
	this->pageCount = PageCount;
	this->Pages = new Page*[PageCount];
	for (int i = 0; i < pageCount; i++)
		Pages[i] = new Page;
}

void Document::addPage()
{
	pageCount++;
	Page** newPages = new Page*[pageCount];

	for (int i = 0; i < pageCount - 1; i++) newPages[i] = Pages[i];
	newPages[pageCount - 1] = new Page();
	delete[] Pages;
	Pages = newPages;
	newPages = nullptr;
	
}

void Document::BackSpace()
{
  
        Document* CurrentDoc = &Document::doc[Document::currentDoc];
        Page* currentPage = CurrentDoc->getPagePtr(CurrentDoc->getCurrentPage());
        Col* CurrentCol = currentPage->getColPtr() + currentPage->currentCol;
        Line* currenLine = CurrentCol->lines + CurrentCol->linesFilled;



        if (currenLine->length > 0) {
           currenLine->length--;
            currenLine->chars[currenLine->length] = '\0';
        }
        else if (CurrentCol->linesFilled > 0)
        {
            CurrentCol->linesFilled--;
            currenLine = CurrentCol->lines + CurrentCol->linesFilled;
            if (currenLine->length > 0) {
                currenLine->length--;
                currenLine->chars[currenLine->length] = '\0';
            }
        }
        else if (currentPage->currentCol > 0)
        {
            currentPage->currentCol--;
            CurrentCol = currentPage->getColPtr() + currentPage->currentCol;
            if (CurrentCol->linesFilled > 0)
            {
                CurrentCol->linesFilled--;
                currenLine = CurrentCol->lines + CurrentCol->linesFilled;
                if (currenLine->length > 0) {
                    currenLine->length--;
                    currenLine->chars[currenLine->length] = '\0';
                }
            }
        }
    
}

bool Document::insertChar(wchar_t ch) {

	while (!Pages[currentPage]->insertChar(ch))
	{
		//if col insert fails
		if (this->currentPage == pageCount - 1) addPage();
		currentPage++;

	}
	return true;
}

int Document::getPageCount() { return pageCount; }
Page* Document::getPagePtr(int index) { return Pages[index]; }


int Document::getCurrentPage() { return currentPage; }

void Document::FlushWord(wchar_t* WordBuffer, int WordLen) {
    if (WordLen == 0) return;

    Page* CurrentPage = Pages[currentPage];
    Col* CurrentCol = &CurrentPage->colsPtr[CurrentPage->currentCol];
    Line* CurrentLine = &CurrentCol->lines[CurrentCol->linesFilled];

   
    if (CurrentLine->length + WordLen > Page::charsPerLine) //if no more space on current line
	{
       
        newLine();
    }

   
    for (int i = 0; i < WordLen; i++)
        insertChar(WordBuffer[i]);
}

void Document::newLine()
{
    

    Page* CurrentPage = Pages[currentPage];
    Col* CurrentCol = &CurrentPage->colsPtr[CurrentPage->currentCol];
    Line* CurrentLine = &CurrentCol->lines[CurrentCol->linesFilled];



        if (CurrentCol->linesFilled < Page::lines - 1) //if lines on current cols are not max
        {
            CurrentCol->linesFilled++;
        }
        else
        {
            //if col is filled move to next col
            if (CurrentPage->currentCol < Page::cols - 1) //if not at last col
            {
                CurrentPage->currentCol++;
            }
            else
            {
                //if cols filled move to next page
                addPage();
                currentPage++;
            }
        }
    //handle alignment

        Page* NewPage = Pages[currentPage];
        Col* NewCol = NewPage->colsPtr+NewPage->currentCol;
        NewCol->lines[NewCol->linesFilled].currentAlignment = Document::CurrentAlignment;
}

Document::~Document()
{
	for (int i = 0; i < pageCount; i++) delete Pages[i];
	delete[] Pages;
	this->Pages = nullptr;
}