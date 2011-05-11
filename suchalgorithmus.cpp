#include "suchalgorithmus.h"

void Suchalgorithmus(DictionaryC dic,char* eingabe){
	igzstream in(eingabe);
	Lexicon flex=dic.flex;


	string token,line;

	while(getline(in,line)){
		istringstream ist(line);   //Einlesen des Satzes

		vector<HypothesisNode> Knoten;
		Knoten.push_back(HypothesisNode());//initialisiert den ersten Knoten

		while ( ist >> token){
			Word word_id_french=flex.getWord(token); // das word zum Wort (mit 2 Bits Sprache)
			unsigned int id_french= word_id_french.wordId(); //die id ohne sprachbits

			plist* translations=&dic.fwa[id_french].pairs; //holt sich die Übersetzungen

			HypothesisNode node_next= HypothesisNode();//initialisiert den nächsten Knoten mit den bisherigen Kosten

			plist::iterator it;

			for (it=translations->begin(); it != translations->end(); it++){//inbound wird initialisiert
				double relfreq=it->first;
				unsigned int id_english=it->second;

				double cost_old=Knoten.back().getBestcost();
				if(it==translations->begin()){
					node_next.setBestcost(cost_old+relfreq); //setzt die besten Kosten auf die beste Kante plus bisherige Kosten
					//std::cout << dic.elex.getString(id_english) << " ";
				}
				PartialTranslation* Kante= new PartialTranslation(relfreq,id_english,&node_next);
				Knoten.back().add_Translation(Kante);
			}

			Knoten.push_back(node_next); //letzter Knoten (node_next) hat keine eingehenden Kanten
		}

		//std::cout << "\n";
		aStar astar(&dic,Knoten);
		astar.search();
	}

}

