struct SentenceInfo;

#pragma once
#include "cost.h"

#include "bleu.h" //sonst gehen die forward declarations kaputt

struct SentenceInfo{
	std::vector<unsigned int> sentence;
	Cost cost;
	BleuInfo* bleu; // matchcount, gramcount

	SentenceInfo();
	SentenceInfo(std::vector<unsigned int> sentence, Cost cost, BleuInfo* bleu=0);
	~SentenceInfo();
};
