#include "lexicon.h"

Lexicon::Lexicon(Language lang):nextword(0),lang(lang){}
Lexicon::Lexicon(const Lexicon&):nextword(0){
	throw lexiconMustNotBeCopiedError();
}

Language Lexicon::getLanguage(){return lang;}

Word Lexicon::add(std::string str){
	#ifdef DEBUG
		if(map[str] != 0) throw wordExistsError();
	#endif
	
	Word w = Word(lang * Word::lowestLangbit + (nextword++));
	
	map[str] = w;
	arr.push_back(str);
	return w;
}

Word Lexicon::getWord(std::string str){
		return Word(lang,map[str]);
}

std::string Lexicon::getString(const Word& w){
	return arr[w.wordId()];
}
