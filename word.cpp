#include "word.h"

Word::Word(uint from):storage(from){}
Word::Word(const Word& w):storage(w.storage){}
Word::Word(Language l, uint id){
	storage = id | l * lowestLangbit;
}
Word::operator uint(){
	return storage;
}

uint Word::wordId() const{
	return storage & (~langbits);
}

Language Word::getLanguage() const{
	return Language(storage >> 30);
}
