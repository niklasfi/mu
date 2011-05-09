#include "lexicon.h"

Lexicon::Lexicon(Language lang):nextword(0),lang(lang){}
Lexicon::Lexicon(const Lexicon& l):nextword(l.nextword),arr(l.arr),map(l.map),lang(l.lang){}

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
	return Word(map[str]); // wenn nichts gefunden wird, dann kommt 0 -> yeti
}

std::string Lexicon::getString(const Word& w){
	if(w.getLanguage() != lang) throw wrongLanguageError();
	return arr[w.wordId()]; // wort muss im lexicon existieren, da es hier er kommt.
}

std::string Lexicon::getString(uint id){
	return getString(Word(lang,id));
}
