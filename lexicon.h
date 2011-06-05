#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "language.h"
#include "word.h"

class Lexicon{
	/* Lexicon liefert eine Bijektion zwischen der Darstellung eines Wortes
	 * als String und als unsigned integer (bzw Word) */
	private:

	uint nextword;
	std::vector<std::string> arr;
	std::unordered_map<std::string, uint> map;
	Language lang;

	public:

	class wordExistsError{};
	/* Dieser fehler kann nur geworfen werden, wenn die Prärozessorvariable
	 * DEBUG gesetzt ist. */
	class wrongLanguageError{};
	/* Wird geworfen, wenn z.B. versucht wird in ein Lexicon der Sprache
	 * Englisch ein Wort der Sprache Französisch einzufügen */

	Language getLanguage();
	Lexicon(Language l = (Language)0);
	Lexicon(const Lexicon& l);

	Word add(std::string str);
	Word getWord_or_add(std::string str);
	/* überprüft zunächst, ob das Wort bereits im Lexicon existiert. In
	 * diesem Falle liefert die Funktion das die ID bzw das Word des Wortes.
	 * anderenfalls wird das Wort eingelegt und das Word des angelegten
	 * Wortes zurückgegeben */
	Word getWord(std::string str);
	std::string getString(const Word& w);
	std::string getString(uint id);
};

