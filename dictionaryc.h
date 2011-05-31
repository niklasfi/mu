#pragma once

#include <string>
#include <vector>
#include "wordinfoc.h"
#include "language.h"
#include "word.h"
#include "lexicon.h"
#include "dictionary.h"
#include <sstream>

typedef unsigned int uint;

class DictionaryC{
	/* DictionaryC ermöglicht das speichern aller relevanten daten für die
	 * einzelwortbasierte Übersetzung. Es ist so ausgelegt, dass es schnell 
	 * eine einzelwortbasierte Phrasentabelle einlesen und effizient 
	 * speichern kann. Hierbei ist zu beachten, dass nicht wie bei 
	 * Dictionary die Wordinfo Klasse, sondern die WordinfoC Klasse 
	 * verwendet wird. */
	public:

	std::vector<WordinfoC> fwa; //frenchWordArray

	Lexicon flex;
	Lexicon elex;

	void add_translation(Word f, Word e, double rel_freq);

	DictionaryC();
	DictionaryC(std::string s);
	DictionaryC(const Dictionary& d);
	
	void read_line_singlewordExtract(std::string line);
	/* verarbeitet eine einzelne Zeile

	void add_translation(std::string f, std::string e, double rel_freq); //tut das, was add_word eigentlich tun sollte
	void add_word(std::string f, std::string e, double rel_freq); //evtl ist translation hier eher angebracht, deshalb sollte darüber nachgedacht werden diese Funktion umzubenennen
	
	void printall();
};
