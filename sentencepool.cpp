#include "sentencepool.h"

void SentencePool::addSentence(target t, std::string s){

	std::string token;
	std::istringstream s_str(s, std::istringstream::in);
	std::vector<uint> vec_tmp;

	while(s_str >> token) {
		vec_tmp.push_back(elex.add(token));
	}

	if (t == ref)
		reference.push_back(&vec_tmp);
	else
		guess.push_back(&vec_tmp);
}
