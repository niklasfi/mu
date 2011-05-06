#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "wordinfo.h"
#include "language.h"
#include "word.h"
#include "lexicon.h"

typedef unsigned int uint;

class Dictionary{
	std::vector<Wordinfo> fwa; //frenchWordArray
	std::vector<Wordinfo> ewa;
	
	Lexicon flex;
	Lexicon elex;
	
	public:
	
	Dictionary();
	
	void add(std::string f, std::string e);
	
	void printall();
};