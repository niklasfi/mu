#include "aStar_2PUNKT0.h"

uint aStar2::max_SentenceTranslation=0; //Anzahl der besten(Satz)Übersetzungen

//Constructor
aStar2::aStar2(vector<HypothesisNode2> vect):vect(vect){}


void aStar2::set_max_SentenceTranslation(uint size){
	//if(aStar2::max_SentenceTranslation != 0) throw max_SentenceTranslationAlreadySetException();
	aStar2::max_SentenceTranslation = size;
}

//Vergleichsfunktion zum Sortieren
bool compare1 (aStarElement e1, aStarElement e2) {
	return e1.cost < e2.cost;
}

uint aStar2::getStarElementPosition(const aStarElement& a){
	return vect.size()-1 - a.trl.size();  //Länge des Satzes - Länge der Teiübersetung
}

void aStar2::print(){ //gibt die oberste Kombination in stack aus
	vector<uint> trl = stack.front().trl;
	for(vector<uint>::reverse_iterator it = trl.rbegin(); it!=trl.rend(); it++){
		std::cout << elex->getString(*it) << " ";    //ID->Wort
	}
	std::cout << "\n";
}

//Eigentliche A*-Suche
void aStar2::search() {

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
			vector<PartialTranslation2*> v=(vect[posfirst-1].getVektor()); //alle möglichen Übersetzungen
			uint bisherigerWeg = stack.front().cost - vect[posfirst].getBestcost();
			
			for(uint i = 0; i<v.size(); i++){//bestem Element werden nun alle Möglichkeiten zugefügt
				aStarElement anew(stack.front());
				anew.addWords2(v[i]->getTranslation());
				anew.cost = v[i]->getCost() + v[i]->getNode()->getBestcost() + bisherigerWeg;//Kosten aktualisiert
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


void aStar2::Suchalgorithmus(char* eingabe, PTree<PTree < double> >* blacktree, Lexicon* eLex, Lexicon* fLex){
	aStar2::schwarz=blacktree;
	aStar2::elex=eLex;
	aStar2::flex=fLex;
	
	igzstream in(eingabe);


	string token,line;

	while(getline(in,line)){
		istringstream ist(line);   //Einlesen des Satzes
		vector<uint> sentence_id;	//Hier wird der Satz in einem Vektor gespeichert (als ID's)
		
		vector<HypothesisNode2> Knoten;
		Knoten.push_back(HypothesisNode2());
		Knoten[0].setBestcost(0);
		unsigned int aktPos=0; //gibt an, wieviele Wörter schon eingelesen wurden
		
		while ( ist >> token){
		    aktPos++;
		    Word word_id_french=flex.getWord(token); // das word zum Wort (mit 2 Bits Sprache)
		    unsigned int id_french= word_id_french.wordId(); //die id ohne sprachbits
		    
		    sentence_id.push_back(id_french);
		    
		    HypothesisNode2 knoten_next=HypothesisNode2();
		    
		    for (int laengePhrase=1; laengePhrase<=prune; laengePhrase++){ //iteriert über die länge der Phrase
			 vector<uint> fphrase;
			 
			 int posPhraseStart=aktPos-laengePhrase;
			 
			 for (int j=posPhraseStart; j<aktPos; j++)	fphrase.push_back(sentence_id[j]); //fphrase wird initialisiert
			      
			 PTree<double>* blauBaum=schwarz.traverse(fphrase)->c;
			      
			 for (PTree<double>::iterator it=blauBaum->begin(); it!=blauBaum->end(); it++){//inbound wird initialisiert
			      double relfreq=it->c;
			      if (relfreq ==0)	break; //wir befinden uns mitten in einer Phrase
				   
			      vector<uint> ephrase=it->phrase();
				
			      if (knoten_next.getBestcost()+prune < Knoten[posPhraseStart].getBestcost()+relfreq)	break; //Pruning, schlechte Translations werden ignoriert
			     
			      if (knoten_next.getBestcost()> (Knoten[posPhraseStart].getBestcost()+relfreq))
				   knoten_next.setBestcost((Knoten[posPhraseStart].getBestcost()+relfreq)); //Kosten des Hy.Nodes aktualisiert
			      
			      PartialTranslation2* Kante= new PartialTranslation2(relfreq,ephrase,&Knoten[posPhraseStart]);
			      knoten_next.add_Translation(Kante);
			}
			Knoten.push_back(knoten_next);
		    }
		}

		//std::cout << "\n";
		aStar2 astar(Knoten);
		astar.search();
	}


}


