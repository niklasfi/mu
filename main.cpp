#include "gszstream.h"
#include "PartialTranslation"
#include "HypothesisNode"
#include "dictionary.h"
#include <vector>
#include <sstream>



void Suchalgorithmus(DictionaryC dic){
	igzstream in("test");
	ogzstrem out("test_uebersetzung");
	Lexicon flex=dic.flex;
	
	
	string token,line;

	while(getline(source,line)){ 
		istringstream ist(line);
		
		vector<HypothesisNode> Knoten;
		Knoten.push_back(HypothesisNode());//initialisiert den ersten Knoten
		
		while ( ist >> token){
			Word word_id_french=flex.getWord(token); // das word zum Wort (mit 2 Bits Sprache)
			unsigned int id_french= word_id_french.wordId(); //die id ohne sprachbits
			
			list< pair<double,double> > translations=&dic.fwa[id_french].pairs; //holt sich - hoffentlich - die Übersetzungen;
			
			HypothesisNode node_next= HypothesisNode();//initialisiert den nächsten Knoten mit den bisherigen Kosten
			
			list< pair<double,double> >::iterator it;
			
			for (it=translations.begin(); i<translations.end(); i++){
				double relfreq=*it->first;
				unsigned int id_english=*it->second;
				
				double cost_old=Knoten.back().getBestcost();
				if(*it==translations.begin()){
					node_next.setBestcost(cost_old+relfreq); //setzt die besten Kosten auf das erste auf der liste
					out << ??????????????????.getWord();
				
				Partialtranslation Kante= Partialtranslation(relfreq,id_english,node_next);
				Knoten.back().addTranslation(Kante);
			}
			
			Knoten.push_back(node_next);
		}
		
		out << endl;
		}
}

int main(){
}