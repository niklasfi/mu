#pragma once
#include "cost.h"

struct SentenceInfo{
	std::vector<unsigned int> sentence;
	Cost cost;
	std::vector< std::pair <unsigned int,unsigned int> >* bleu; // matchcount, gramcount

	SentenceInfo():bleu(0){}
	SentenceInfo(std::vector<unsigned int> sentence, Cost cost, std::vector< std::pair <unsigned int,unsigned int> >* bleu=0): sentence(sentence), cost(cost), bleu(bleu){}
};