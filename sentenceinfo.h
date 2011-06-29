#pragma once
#include "cost.h"

struct SentenceInfo{
	std::vector<uint> sentence;
	Cost cost;
	std::vector< std::pair <uint,uint> >* bleu; // matchcount, gramcount

	
	SentenceInfo(vector<uint> sentence, Cost cost, std::vector< std::pair <uint,uint> >* bleu=NULL): sentence(sentence), cost(cost), bleu(bleu){}

};
