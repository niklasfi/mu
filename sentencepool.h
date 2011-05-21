#include "lexicon.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class SentencePool {

	public:
	enum target {ref, gue};
	Lexicon elex;
	std::vector< std::vector<uint>* > reference;
	std::vector< std::vector<uint>* > guess;

	void addSentence(target, std::string);
};
