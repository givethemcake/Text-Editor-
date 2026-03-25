#include"stats.h"
#include"Document.h"

int Stats::CountSentences(wchar_t delim )
{
        if (!Page::SetUpDone) return -1;

        //count spaces
        int spaces = 0;

        int pageCount = Document::doc->getPageCount();
        for (int currentPage = 0; currentPage < pageCount; currentPage++)
        {
            Col* colsptr = Document::doc->getPagePtr(currentPage)->getColPtr();
            int colNumber = Page::cols;
            //colsptr is a ptr to all cols in the current page

            for (int currentCol = 0; currentCol < colNumber; currentCol++)
            {
                Line* Lines = colsptr[currentCol].lines;
                int LinesFilled = colsptr[currentCol].linesFilled;
                for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
                {
                    int i = 0;
                    while (colsptr[currentCol].lines[currentLine].chars[i] != L'\0')
                    {
                        if (colsptr[currentCol].lines[currentLine].chars[i] == delim || colsptr[currentCol].lines[currentLine].chars[i] == L'!' || colsptr[currentCol].lines[currentLine].chars[i] == L'?')
                        {
                            wchar_t nextChar = colsptr[currentCol].lines[currentLine].chars[i + 1];

                            if (nextChar == L' ' || nextChar == L'\0')
                                spaces++;
                        }
                        i++;
                    }
                    //spaces++;//catch last word for each line 
                    //strictly only count full stops to indicate end of sentences
                }

            }

        }

     return spaces;
}


int Stats::CountWords(wchar_t delim )
{

    if (!Page::SetUpDone) return -1;

    //count spaces
    int spaces = 0;



    int pageCount = Document::doc->getPageCount();
    for (int currentPage = 0; currentPage < pageCount; currentPage++)
    {
        Col* colsptr = Document::doc->getPagePtr(currentPage)->getColPtr();
        int colNumber = Page::cols;
        //colsptr is a ptr to all cols in the current page

        for (int currentCol = 0; currentCol < colNumber; currentCol++)
        {
            Line* Lines = colsptr[currentCol].lines;
            int LinesFilled = colsptr[currentCol].linesFilled;
            for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
            {
                int i = 0;
                bool inWord = false;
                while (colsptr[currentCol].lines[currentLine].chars[i] != L'\0')
                {

                    if (colsptr[currentCol].lines[currentLine].chars[i] != delim)
                    {
                        if (!inWord)
                        {
                            spaces++;
                            inWord = true;
                        }//do nothing if we were already inside a word
                    }
                    else // if space found
                    {
                        inWord = false;
                    }

                    i++;
                }
                //spaces++;//catch last word for each line 
            }

        }

    }

    return spaces;
}


int Stats::CountChars()
{

    if (!Page::SetUpDone) return -1;

    //count chars
    int chars = 0;

    int pageCount = Document::doc->getPageCount();
    for (int currentPage = 0; currentPage < pageCount; currentPage++)
    {
        Col* colsptr = Document::doc->getPagePtr(currentPage)->getColPtr();
        int colNumber = Page::cols;
        //colsptr is a ptr to all cols in the current page

        for (int currentCol = 0; currentCol < colNumber; currentCol++)
        {
            Line* Lines = colsptr[currentCol].lines;
            int LinesFilled = colsptr[currentCol].linesFilled;
            for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
            {
                chars += colsptr[currentCol].lines[currentLine].length ;
                //colsptr[currentCol].lines[currentLine].length stores index of last char hence +1
                //nvm +1 fucks it
            }

        }

    }

    return chars;

}
int Stats::CountSpaces(wchar_t delim )
{

    if (!Page::SetUpDone) return -1;

    //count spaces
    int spaces = 0;


    int pageCount = Document::doc->getPageCount();
    for (int currentPage = 0; currentPage < pageCount; currentPage++)
    {
        Col* colsptr = Document::doc->getPagePtr(currentPage)->getColPtr();
        int colNumber = Page::cols;
        //colsptr is a ptr to all cols in the current page

        for (int currentCol = 0; currentCol < colNumber; currentCol++)
        {
            Line* Lines = colsptr[currentCol].lines;
            int LinesFilled = colsptr[currentCol].linesFilled;
            for (int currentLine = 0; currentLine <= LinesFilled; currentLine++)
            {
                int i = 0;
                while (colsptr[currentCol].lines[currentLine].chars[i] != L'\0')
                {
                    if (colsptr[currentCol].lines[currentLine].chars[i] == delim) spaces++;
                    i++;
                }
                //spaces++;//catch last word for each line 
            }

        }

    }

    return spaces;
}

Stats::Stats()
{
    BuildStats();

}

void Stats::BuildStats()
{
    /*Display a persistent statistics panel showing real-time metrics. - Characters (with and without spaces)
    - Words
    - Sentences
    - Pages (based on the document's actual page count)
    - Estimated reading time (assume an average reading speed of 200 words per minute)
    These statistics update instantly as the user types or performs any modification to the text.*/
    //could make word parsing better 

    this-> NumberOfChars = CountChars();
    this->NumberOfWords = CountWords();
    this->NumberOfSpaces = CountSpaces();
    this->NumberOfSentences = CountSentences();

}