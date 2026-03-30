#pragma once

struct SpellCheck
{
	wchar_t** Words;
	long long int WordCount;
	static SpellCheck* dictionary;
	SpellCheck();
	~SpellCheck();
	bool isWord(wchar_t* target);
	void ReadFileDic();

};