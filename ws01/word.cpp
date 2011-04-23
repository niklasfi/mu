#include "word.h"

uint Word::nextE = 1;
uint Word::nextF = 1;

Word::Word(Word::Language l){
	if(l == french)
		_id = nextF++;
	else
		_id = english + nextE++;
}

Word::Word(uint _id):_id(_id){}

Word::Language Word::language() const{
	return (Word::Language) (_id & langbit);
}

uint Word::id() const{
	return idbits & _id;
}

Word::operator uint() const{
	return _id;
}

bool Word::NotAWord() const{
	return !id();
}

std::ostream& Word::operator<<(std::ostream& os){
	os << (language() == english ? "e" : "f");
	os << std::setw(10)<< std::setfill('0') << id();
	return os;
}
