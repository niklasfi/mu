#include "dictionary.h"

#include <iomanip>

Dictionary::Dictionary():flex(french),elex(english){}

void Dictionary::add(std::string f, std::string e){
	Word ewd, fwd;
	if (!(fwd = flex.getWord(f))){
		fwd = flex.add(f);
		Wordinfo winf;
		fwa.push_back(winf);
	}
	if (!(ewd = elex.getWord(e))){
		ewd = elex.add(e);
		Wordinfo winf;
		ewa.push_back(winf);
	}
	
	fwa[fwd.wordId()].singlecount ++;
	ewa[ewd.wordId()].singlecount ++;
	
	fwa[fwd.wordId()].pairs[ewd.wordId()] ++;
	ewa[ewd.wordId()].pairs[fwd.wordId()] ++;
}

void Dictionary::printall(){
	uint wit = 0;
	for(std::vector<Wordinfo>::iterator _wit = fwa.begin(); _wit < fwa.end(); _wit ++){
		for(std::map<uint,uint>::iterator pit = fwa[wit].pairs.begin(); pit != fwa[wit].pairs.end(); pit++){
			Word fword(french,wit);
			Word eword(english,(*pit).first);
			std::cout << fwa[fword].relFreq(eword);
			std::cout << " " << ewa[eword].relFreq(fword);
			//std::cout << "@" << fwa[fword].pairs[(*pit).first] << "/" << fwa[fword].singlecount;
			//std::cout << "@" << ewa[eword].pairs[(*pit).first] << "/" << ewa[eword].singlecount;
			std::cout << " # " << flex.getString(fword) << " # ";
			std::cout << elex.getString(eword) << "\n";
		}
		wit++;
	}
}

//for(uint wit = 0; &(fwa[wit]) < fwa.end(); wit ++)
