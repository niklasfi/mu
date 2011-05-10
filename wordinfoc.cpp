#include "wordinfoc.h"

uint WordinfoC::maxlen = 0;

WordinfoC::WordinfoC():singlecount(0){}
WordinfoC::WordinfoC(Wordinfo w):singlecount(w.singlecount){
	for(std::map<uint,uint>::iterator it = w.pairs.begin(); it!=w.pairs.end(); it++){
		add_translation(it->first,w.relFreq(it->first));
	}
}
WordinfoC::WordinfoC(uint singlecount):singlecount(singlecount),pairs(){}

void WordinfoC::add_translation(Word w, double rel_freq){
	if(
		   maxlen!=0
		//&& !pairs.empty()
		&& pairs.size() >= maxlen
		&& pairs.back().first <= rel_freq
	) return; //liste war schon voll und kosten für das wort zu groß

	plist::iterator it = pairs.begin();

	//sortiert einfügen:
	while(it != pairs.end() && it->first < rel_freq) it++; //so lange fortschreiten, wie

	pairs.insert(it,std::pair<double,uint>(rel_freq,w));

	if(maxlen != 0)
		while(maxlen < pairs.size()){
			pairs.pop_back();
		}
}

void WordinfoC::set_maxlen(uint size){
	if(maxlen != 0) throw maxlenAlreadySetException();
	maxlen = size;
}

void WordinfoC::set_singlecount(uint count){
	if(singlecount != 0) throw singleCountAlreadySetException();
	singlecount = count;
}

uint WordinfoC::get_singlecount() const{
	return singlecount;
}
