#pragma once

struct SpellCheck
{
	wchar_t* Words;
	long long int WordCount;
	static SpellCheck* dictionary;
	SpellCheck();
	void ReadFile();

};