#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_map>

#include "word.h"

class Lexicon{
	private:
	
	struct eqstr{
		bool operator()(const char* s1, const char* s2) const{
			return strcmp(s1,s2) == 0;
		}
	};
	
	Word::Language lang;
	std::vector<const char*> arr;
	std::unordered_map<const char*, uint, std::hash<const char*>, eqstr> map;
		
	public:
	
	class wordExistsError{};
	
	Lexicon(Word::Language);
	Word add(const char* word);
	Word getWord(const char* str);
	const char* getString(const Word& w);
};

