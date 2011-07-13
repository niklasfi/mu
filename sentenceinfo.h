#pragma once
#include "cost.h"

class BleuInfo; //#include "bleu.h" sonst gehen die forward declarations kaputt

struct SentenceInfo{
	std::vector<unsigned int> sentence;
	Cost cost;
	BleuInfo* bleu; // matchcount, gramcount

	SentenceInfo():bleu(0){}
	SentenceInfo(std::vector<unsigned int> sentence, Cost cost, BleuInfo* bleu=0): sentence(sentence), cost(cost), bleu(bleu){}
};
