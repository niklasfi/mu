#pragma once

#include "lexicon.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class SentencePool {
	/*
	 * sentencepool speichert jeweils eine liste von referenz- und hypo-
	 * thesenübersetzungen, um diese dann später bewertungsfunktionen wie
	 * bleu, WER und PER zur verfügung stellen zu können */
	public:
	enum target {ref, gue};
	Lexicon elex;
	std::vector< std::vector<uint>* > reference; 
	std::vector< std::vector<uint>* > guess;

	SentencePool();
	~SentencePool();
	void addSentence(target, std::string); 
	/* fügt den übergebenen Satz in den entsprechenden Vektor ein. Es ist
	 * darauf zu achten, dass SentencePool nicht garantiert, dass gleich
	 * viele Sätze in reference und guess sind. */
};
