#include "lexicon.h"


int main(){
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

	return 0;
}


