#include "sentencepool.h"
#include "language.h"

SentencePool::SentencePool():elex(english){}
SentencePool::~SentencePool(){
	for(uint i = 0; i < reference.size(); i++)
		delete reference[i];
	for(uint j = 0; j < guess.size(); j++)
		delete guess[j];
}
void SentencePool::addSentence(target t, std::string s){

	std::string token;
	std::istringstream s_str(s);
	std::vector<uint>* vec_tmp = new std::vector<uint>();

	while(s_str >> token) {
		vec_tmp->push_back(elex.getWord_or_add(token));
	}

	if (t == ref)
		reference.push_back(vec_tmp);
	else
		guess.push_back(vec_tmp);
}
