#include "wordinfo.h"

Wordinfo::Wordinfo():singlecount(0){}

double Wordinfo::relFreq(Word w) const{
	//return -log(pairs[w.wordId()])+log(singlecount);
	return log(singlecount) - log(get_translation_count(w.wordId()));
}

uint Wordinfo::get_translation_count(Word relatedWord) const{
	return pairs.find(relatedWord)->second;
}
