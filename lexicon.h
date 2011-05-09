#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "language.h"
#include "word.h"

class Lexicon{
	private:

	uint nextword;
	std::vector<std::string> arr;
	std::unordered_map<std::string, uint> map;
	Language lang;

	public:

	class wordExistsError{};
	class wrongLanguageError{};

	Language getLanguage();
	Lexicon(Language l = (Language)0);
	Lexicon(const Lexicon& l);

	Word add(std::string str);
	Word getWord(std::string str);
	std::string getString(const Word& w);
	std::string getString(uint id);
};

