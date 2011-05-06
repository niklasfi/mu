#pragma once

typedef unsigned int uint;

#include "global.h"
#include "language.h"

class Word{
	uint storage;
public:
	Word(uint from = 0);
	Word(const Word&);
	Word(Language, uint id);

	operator uint();
	
	static const uint langbits = 2 << 30; //0xC0000000;
	static const uint lowestLangbit = 1 << 30; //0x40000000;
	uint wordId() const;
	Language getLanguage() const;
	
	class wordIdOutOfBoundsException{};
};
