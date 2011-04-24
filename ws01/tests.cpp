#include "tests.h"

int main(){
	
	assert(word());
	assert(lexicon());
	return 0;
}

bool word(){
	Word w1 = Word(yeti,4711);
	assert(w1 == (yeti * Word::lowestLangbit | 4711));
	
	Word w2 = Word(french,815);
	assert(w2 == (french * Word::lowestLangbit | 815));

	Word w3 = Word(english,123);
	assert(w3 == (english * Word::lowestLangbit | 123));
	return true;
}

bool lexicon(){
	Lexicon elex(english);
	
	std::string str;
	
	Word kword = elex.add(str="cat");
	Word mword = elex.add(str="mouse");
	Word dword = elex.add(str="dog");
	
	assert(elex.getString(kword) == "cat");
	assert(elex.getString(mword) == "mouse");
	assert(elex.getString(dword) == "dog");
	
	assert(elex.getWord("cat") == kword);
	assert(elex.getWord("mouse") == mword);
	assert(elex.getWord("dog") == dword);
	
	Lexicon flex(french);

	kword = flex.add(str="chat");
	mword = flex.add(str="sourie");
	dword = flex.add(str="chien");
	
	assert(flex.getString(kword) == "chat");
	assert(flex.getString(mword) == "sourie");
	assert(flex.getString(dword) == "chien");
	
	assert(flex.getWord("chat") == kword);
	assert(flex.getWord("sourie") == mword);
	assert(flex.getWord("chien") == dword);
	
	return true;
}
