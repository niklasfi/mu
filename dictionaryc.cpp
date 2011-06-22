#include "dictionaryc.h"

DictionaryC::DictionaryC():flex(french),elex(english){}
DictionaryC::DictionaryC(const Dictionary& d):flex(d.flex),elex(d.elex),fwa(d.fwa.size()){
	for(uint i = 0; i < d.fwa.size(); i++){
		WordinfoC* finfo = &fwa[i];
		finfo->set_singlecount(d.fwa[i].singlecount);
		for(std::map<uint, uint>::const_iterator it = d.fwa[i].pairs.begin(); it!= d.fwa[i].pairs.end(); it++)
			finfo->add_translation(it->first, d.fwa[i].relFreq(it->first));
	}
}

void DictionaryC::add_translation(std::string f, std::string e, double rel_freq){
	Word ewd, fwd;
	if (!(fwd = flex.getWord(f))){
		fwd = flex.add(f);
		Wordinfo winf;
		fwa.push_back(winf);
	}
	if(!(ewd = elex.getWord(e)))
		ewd = elex.add(e);
	fwa[fwd.wordId()].add_translation(ewd,rel_freq);
}

void DictionaryC::add_word(std::string f, std::string e, double rel_freq){
	add_translation(f,e,rel_freq);
}

void DictionaryC::add_translation(Word f, Word e, double rel_freq){
	fwa[f].add_translation(e,rel_freq);
}

void DictionaryC::read_line_singlewordExtract(std::string line){
	std::istringstream ist(line);
	uint f_ind, e_ind; double freq;
	std::string f, e;
	std::string trunc;
	
	//f_freq e_freq # f_word # e_word
	ist >> freq >> trunc >> trunc >> f >> trunc >> e;
	add_translation(f,e,freq);
}

void DictionaryC::printall(){
	for(uint i = 0; i < fwa.size(); i++){
		for(plist::const_iterator pit = fwa[i].pairs.begin(); pit != fwa[i].pairs.end(); pit++){
			Word fword(french,i);
			Word eword(english,pit -> second);
			std::cout << pit-> first;
			//std::cout << "@" << fwa[fword].pairs[(*pit).first] << "/" << fwa[fword].singlecount;
			//std::cout << "@" << ewa[eword].pairs[(*pit).first] << "/" << ewa[eword].singlecount;
			std::cout << " # " << flex.getString(fword) << " # ";
			std::cout << elex.getString(eword) << "\n";
		}
	}
}
