#include "gzstream/gzstream.h"
#include "PartialTranslation.h"
#include "HypothesisNode.h"
#include "dictionaryc.h"
#include <vector>
#include <sstream>



void Suchalgorithmus(DictionaryC dic){
	igzstream in("test");
	ogzstream out("test_uebersetzung");
	Lexicon flex=dic.flex;
	
	
	string token,line;

	while(getline(in,line)){ 
		istringstream ist(line);
		
		vector<HypothesisNode> Knoten;
		Knoten.push_back(HypothesisNode());//initialisiert den ersten Knoten
		
		while ( ist >> token){
			Word word_id_french=flex.getWord(token); // das word zum Wort (mit 2 Bits Sprache)
			unsigned int id_french= word_id_french.wordId(); //die id ohne sprachbits
			
			plist* translations=&dic.fwa[id_french].pairs; //holt sich - hoffentlich - die Übersetzungen;
			
			HypothesisNode node_next= HypothesisNode();//initialisiert den nächsten Knoten mit den bisherigen Kosten
			
			plist::iterator it;
			
			for (it=translations->begin(); it != translations->end(); it++){
				double relfreq=it->first;
				int id_english=it->second;
				
				double cost_old=Knoten.back().getBestcost();
				if(it==translations->begin()){
					node_next.setBestcost(cost_old+relfreq); //setzt die besten Kosten auf das erste auf der liste
					out << dic.elex.getString(id_english);
				}
				PartialTranslation Kante= PartialTranslation(relfreq,id_english,&node_next);
				Knoten.back().add_Translation(Kante);
			}
			
			Knoten.push_back(node_next);
		}
		
		out << endl;
		}
}

int main(){
}
