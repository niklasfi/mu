#include "Lexicon.h"
#include <vector>
#include <string>
#include <iostream>

class sentencepool {

	public:
	enum target = {reference, guess};
	Lexicon elex;
	vector<vector<int>*> reference;
	vector<vector<int>*> guess;

	void addSentence(target, std::string);
}
