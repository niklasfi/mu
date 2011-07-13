#include "wordinfo.h"

Wordinfo::Wordinfo():singlecount(0){}

double Wordinfo::relFreq(unsigned int w) const{
	//return -log(pairs[w.wordId()])+log(singlecount);
	return log(singlecount) - log(get_translation_count(w));
}

uint Wordinfo::get_translation_count(unsigned int relatedWord) const{
	return pairs.find(relatedWord)->second;
}
