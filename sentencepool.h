#include "lexicon.h"
#include <vector>
#include <string>
#include <iostream>

class SentencePool {

	public:
	enum target {ref, gue};
	Lexicon elex;
	std::vector< std::vector<int>* > reference;
	std::vector< std::vector<int>* > guess;

	void addSentence(target, std::string);
};
