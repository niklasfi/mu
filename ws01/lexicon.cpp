#include "lexicon.h"

Lexicon::Lexicon(Word::Language lang):lang(lang){}

Word Lexicon::add(const char* str){
	#ifdef DEBUG
		if(map[str] != 0) throw wordExistsError();
	#endif
	
	Word w = Word(lang);
	
	map[str] = w;
	arr.push_back(str);
	return w;
}

Word Lexicon::getWord(const char* str){
		return Word(map[str] | lang); 
		/* | lang stellt sicher, dass die Sprache auch dann noch stimmt, wenn kein
			 wort gefunden wurde */
}

const char* Lexicon::getString(const Word& w){
	return arr[w];
}
