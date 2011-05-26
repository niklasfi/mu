#include "aStar.h"

uint aStar::max_SentenceTranslation=0; //Anzahl der besten(Satz)Übersetzungen

//Constructor
aStar::aStar(DictionaryC* dc, vector<HypothesisNode> vect):dc(dc),vect(vect){}


void aStar::set_max_SentenceTranslation(uint size){
	//if(aStar::max_SentenceTranslation != 0) throw max_SentenceTranslationAlreadySetException();
	aStar::max_SentenceTranslation = size;
}

//Vergleichsfunktion zum Sortieren
bool compare1 (aStarElement e1, aStarElement e2) {
	return e1.cost < e2.cost;
}

uint aStar::getStarElementPosition(const aStarElement& a){
	return vect.size()-1 - a.trl.size();  //Länge des Satzes - Länge der Teiübersetung
}

void aStar::print(){ //gibt die oberste Kombination in stack aus
	vector<uint> trl = stack.front().trl;
	for(vector<uint>::reverse_iterator it = trl.rbegin(); it!=trl.rend(); it++){
		std::cout << dc->elex.getString(*it) << " ";    //ID->Wort
	}
	std::cout << "\n";
}

//Eigentliche A*-Suche
void aStar::search() {

	int length = vect.size();

	vector<uint> vtemp;

	aStarElement elementI(vtemp, vect.back().getBestcost()); //erstesElement initialisiert
	stack.push_front(elementI);


	uint n =0;
	while(n<max_SentenceTranslation || max_SentenceTranslation==0){	//Anzahl der (Satz)Übersetzungen, die ausgegeben werden
		if(stack.empty()) break;

		if(getStarElementPosition(stack.front()) == 0) {	//Wenn Satzanfang erreicht, dann gib Übersetzung aus und lösche entsprechendes Element
			print();
			stack.pop_front();
			n++;
		} else {	//Führe einen A*-Schritt für das erste Element im Stack durch


			uint posfirst = getStarElementPosition(stack.front());
			vector<PartialTranslation*> v(vect[posfirst-1].getVektor()); //alle möglichen Übersetzungen
			uint bisherigerWeg = stack.front().cost - vect[posfirst].getBestcost();
			for(uint i = 0; i<v.size(); i++){//bestem Element werden nun alle Möglichkeiten zugefügt
				aStarElement anew(stack.front());
				anew.addWord(v[i]->getTranslation());
				anew.cost = v[i]->getCost() + vect[posfirst-1].getBestcost() + bisherigerWeg;//Kosten aktualisiert
				stack.push_back(anew);
			}
			stack.pop_front();//altes Element (ohne Aktualisierung) wird entfernt

			aStarElement* first_ptr=&stack.front();
			list<aStarElement>::iterator first_it=stack.begin();
			for (list<aStarElement>::iterator it=stack.begin(); it !=stack.end(); it++){
				if (*it < *first_ptr){
						first_ptr=&*it;
						first_it=it;

				}
			}
			stack.push_front(*first_ptr);
			stack.erase(first_it);

		}
	}
}


void aStar::Suchalgorithmus(DictionaryC dic,char* eingabe){
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


