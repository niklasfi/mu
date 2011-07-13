#pragma once

typedef unsigned int uint;

#include <map>
#include <cmath>

class Wordinfo{
	/* aggregiert die relevanten Daten zu einem französischem Wort bei der
	 * Erstellung der einzelwortbasierten Phrasentabelle */
	public:
	uint singlecount;
	std::map<uint, uint> pairs;
	/* Pairs gibt zu einer id (1. Argument) den zugehörigen paircount (2.
	 * Argument) zurück */

	Wordinfo();

	uint get_translation_count(unsigned int relatedWord) const;
	
	double relFreq(unsigned int w) const;
	/* zu einem gegebenen englischen Wort die relFreq zu berechnen */
};
