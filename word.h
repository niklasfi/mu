#pragma once

typedef unsigned int uint;

#include "global.h"
#include "language.h"

class Word{
	/* Word ist eine transparent nach unsigned int konvertierbare Klasse. 
	 * Sie dient im allgemeinen dazu in der ID eines Wortes zu Codieren, 
	 * zu welcher Sprache es gehört. Alternativ aber auch immer unsigned
	 * int verwendet werden, da diese Klasse die implizite Konvertierung
	 * nach unsigned int zulässt */
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
