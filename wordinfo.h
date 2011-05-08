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

	uint get_translation_count(Word relatedWord) const;

	double relFreq(Word w) const;
};
