#pragma once

typedef unsigned int uint;

#include <map>
#include "word.h"
#include <cmath>

class Wordinfo{
	public:
	uint singlecount;
	std::map<uint, uint> pairs;
	
	Wordinfo();
	double relFreq(Word w);
};
