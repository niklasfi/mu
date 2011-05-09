#include "dictionaryc.h"

void conversion(){
	Dictionary d;
	d.add("hallo","hi");
	d.add("hallo","howdy");
	d.add("hallo","g'day");
	d.add("hallo","good");
	d.add("hallo","evening");
	d.add("willkommen","hi");
	d.add("hallo","hi");
	d.add("gruzi","hi");

	Word hallo = d.flex.getWord("hallo");
	Word willkommen = d.flex.getWord("willkommen");
	Word gruzi = d.flex.getWord("gruzi");

	Word hi = d.elex.getWord("hi");
	Word howdy = d.elex.getWord("howdy");
	Word gday = d.elex.getWord("g'day");
	Word good = d.elex.getWord("good");
	Word evening = d.elex.getWord("evening");

	d.fwa[hallo].singlecount = 1e6; //sorgt dafür, dass division kaputt geht, wenn nicht log()-log() gerechnet wird.

	DictionaryC dc(d);

	uint hallosln[] = {hi,howdy,gday,good};
	uint willkommensln[] = {hi};
	uint gruzisln[] = {hi};

	uint fwords[3] = {hallo,willkommen,gruzi};
	uint* solutions[3] = {hallosln,willkommensln, gruzisln};

	for(uint i = 0; i < 3; i++){
		assert(dc.fwa[fwords[i]].get_singlecount() == d.fwa[fwords[i]].singlecount);
		uint* ids = solutions[i];
		for(std::list<std::pair<double,uint>>::iterator it=dc.fwa[fwords[i]].pairs.begin(); it != dc.fwa[fwords[i]].pairs.end();it++){
			/*std::cout << "first: " << it-> first << ", second: " << it -> second << ", foobar: " << d.fwa[fwords[i]].relFreq(it->second) 
				<< "\n\tdelta: " << (it->first - d.fwa[fwords[i]].relFreq(it->second))
				<< "\n\tdelt2: "<< (it->first - (log(d.fwa[i].singlecount)-log(d.fwa[fwords[i]].pairs[it->second]))) << "\n";*/
			assert(fabs(it->first - d.fwa[fwords[i]].relFreq(it->second)) < 1e-14); //wieso ist das hier nicht genau?
		}
	}
}

void readline(){
	DictionaryC dc;
	dc.read_line_singlewordExtract("9.6764 9.85506 # le # to");
	dc.read_line_singlewordExtract("0.389607 1.55587 # le # the");
	dc.read_line_singlewordExtract("7.59696 1.9548 # le # organized");
	dc.read_line_singlewordExtract("8.98325 4.66108 # le # her");
	dc.read_line_singlewordExtract("10.3695 5.86647 # le # strategic");
	
	
	
	Word le = dc.flex.getWord("le");
	
	Word to = dc.elex.getWord("to");
	Word the = dc.elex.getWord("the");
	Word organized = dc.elex.getWord("organized");
	Word her = dc.elex.getWord("her");
	Word strategic = dc.elex.getWord("strategic");
	
	assert(le!=0);
	assert(to!=0);
	assert(organized!=0);
	assert(her!=0);
	assert(strategic!=0);
	
	uint sln_ids[4]={the,organized,her,to};
	double sln_vals[4] = {0.389607, 7.59696, 8.98325, 9.6764};
	
	{
		uint i = 0;
		for(plist::iterator it = dc.fwa[le].pairs.begin(); it != dc.fwa[le].pairs.end(); it++){
			assert(fabs(it->first - sln_vals[i]) < 10e-4);
			assert(it->second == sln_ids[i]);
			i++;
		}
		assert(i==4);
	}
}

int main(){
	WordinfoC::set_maxlen(4);
	conversion();
	readline();
}
