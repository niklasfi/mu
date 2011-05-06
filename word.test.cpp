#include "word.h"

int main(){
	Word w1 = Word(yeti,4711);
	assert(w1 == (yeti * Word::lowestLangbit | 4711));

	Word w2 = Word(french,815);
	assert(w2 == (french * Word::lowestLangbit | 815));

	Word w3 = Word(english,123);
	assert(w3 == (english * Word::lowestLangbit | 123));
	return 0;
}
