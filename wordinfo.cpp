#include "wordinfo.h"

Wordinfo::Wordinfo():singlecount(0){}

double Wordinfo::relFreq(Word w){
	return -log(pairs[w.wordId()])+log(singlecount);
}
