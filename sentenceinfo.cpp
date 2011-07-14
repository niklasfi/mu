struct SentenceInfo;

#include "sentenceinfo.h"

SentenceInfo::SentenceInfo():bleu(0){}
SentenceInfo::SentenceInfo(std::vector<unsigned int> sentence, Cost cost, BleuInfo* bleu): sentence(sentence), cost(cost), bleu(bleu){}
SentenceInfo::~SentenceInfo(){
	delete bleu;
}
