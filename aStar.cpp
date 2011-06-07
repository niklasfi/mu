#include "aStar.h"

uint aStar::max_SentenceTranslation=0; //Anzahl der besten(Satz)Übersetzungen
Lexicon* aStar::elex=0;
Lexicon* aStar::flex=0;
PTree< PTree <double> >* aStar::schwarz=0;
int aStar::prune=2;

//Constructor
aStar::aStar(vector<HypothesisNode> vect):vect(vect){}


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
	vector<uint> trl = stack.top().trl;
	for(vector<uint>::reverse_iterator it = trl.rbegin(); it!=trl.rend(); it++){
		std::cout << aStar::elex->getString(*it) << " ";    //ID->Wort
	}
	std::cout << "\n";
}

//Eigentliche A*-Suche
void aStar::search() {

	int length = vect.size();

	vector<uint> vtemp;

	aStarElement elementI(vtemp, vect.back().getBestcost(),&vect[length-1]); //erstesElement initialisiert
	stack.push(elementI);


	uint n =0;
	while(n<max_SentenceTranslation || max_SentenceTranslation==0){	//Anzahl der (Satz)Übersetzungen, die ausgegeben werden
		if(stack.empty()) break;
		cout << "stack.top().pos " << stack.top().pos << " cost: " << stack.top().pos->getBestcost() << endl;
		if(stack.top().pos->getOutbound().size() ==0) {	//Wenn Satzanfang erreicht, dann gib Übersetzung aus und lösche entsprechendes Element
			print();
			stack.pop();
			n++;
		} else {	//Führe einen A*-Schritt für das erste Element im Stack durch

			aStarElement toExplore=stack.top(); //zu erweiternder Teilsatz wird gespeichert
			stack.pop();//altes Element wird entfernt

			vector<PartialTranslation*> v=toExplore.pos->getOutbound(); //alle möglichen Übersetzungen
			double bisherigerWeg = stack.top().cost - toExplore.pos->getBestcost();

			cout << "v.size " << v.size();

			for(uint i = 0; i<v.size(); i++){ //bestem Element werden nun alle Möglichkeiten zugefügt
				aStarElement* anew = new aStarElement(toExplore);
				if (v[i]->getTranslation().size() == 0)	cout << "??" << endl;
				anew->addWords2(v[i]->getTranslation());
				double remainingTranslationCost = v[i]->getDestination()->getBestcost();
				anew->cost = v[i]->getCost() + remainingTranslationCost + bisherigerWeg; //Kosten aktualisieren
				if (anew->cost<0.000001)	cout << "anewcost " << anew->cost << " = " << remainingTranslationCost << " + " << bisherigerWeg << endl;
				anew->pos=v[i]->getDestination();
				stack.push(*anew);
				cout<< "anew ist da? " << anew->cost;
			}
			/*cout << "Der stack nach der for-schleife" << endl;
			priority_queue<aStarElement, vector<aStarElement>, greater<aStarElement> > stack2=stack;
			while (!stack2.empty()){
				cout << stack2.top().cost << ", ";
				stack2.pop();
			}
			cout << endl;*/
		}
	}
}


void aStar::Suchalgorithmus(char* eingabe, PTree<PTree < double> >* blacktree, Lexicon* eLex, Lexicon* fLex){
	aStar::schwarz=blacktree;
	aStar::elex=eLex;
	aStar::flex=fLex;

	igzstream in(eingabe);


	string token,line;

	while(getline(in,line)){
		istringstream ist(line);   //Einlesen des Satzes
		vector<uint> sentence_id;	//Hier wird der Satz in einem Vektor gespeichert (als ID's)

		vector<HypothesisNode> Knoten;
		Knoten.push_back(HypothesisNode());
		Knoten[0].setBestcost(0);
		unsigned int aktPos=0; //gibt an, wieviele Wörter schon eingelesen wurden

		while ( ist >> token){
		    aktPos++;
		    Word word_id_french=flex->getWord(token); // das word zum Wort (mit 2 Bits Sprache)
		    unsigned int id_french= word_id_french.wordId(); //die id ohne sprachbits

		    sentence_id.push_back(id_french);

		    HypothesisNode knoten_next=HypothesisNode();

		    for (int laengePhrase=1; laengePhrase<=prune; laengePhrase++){ //iteriert über die länge der Phrase
			 vector<uint> fphrase;


			 int posPhraseStart=aktPos-laengePhrase;
			 if (posPhraseStart<0)	continue; //wenn wir an pos i sind machen Phrasen die länger als i sind keinen Sinn

			 for (int j=posPhraseStart; j<aktPos; j++)	fphrase.push_back(sentence_id[j]); //fphrase wird initialisiert


			 PTree<PTree <double> >* schwarzRest=schwarz->traverse(fphrase);
			 if (!schwarzRest)	continue; //wenn es die französische Phrase nicht gibt, nächste überprüfen
			 PTree <double>* blauBaum=&schwarzRest->c;

			 int counter = 0;

			 for (PTree<double>::iterator it=blauBaum->begin(); it!=blauBaum->end(); it++){//inbound wird initialisiert
					double relfreq=it->c;
			      if (relfreq==(1./0.) )	continue; //wir befinden uns mitten in einer Phrase oder haben unendlich viele kosten
			      if (counter++ > 10) continue;

			      vector<uint> ephrase=it->phrase();
			      if (knoten_next.getBestcost()+prune < Knoten[posPhraseStart].getBestcost()+relfreq)	continue; //Pruning, schlechte Translations werden ignoriert

			      if (knoten_next.getBestcost()> (Knoten[posPhraseStart].getBestcost()+relfreq))
				   knoten_next.setBestcost((Knoten[posPhraseStart].getBestcost()+relfreq)); //Kosten des Hy.Nodes aktualisiert

						if (ephrase.size()==0){
							cout << "wir haben eine leere Phrase!!!"<< endl;
							exit(1);
						}
			      PartialTranslation* Kante= new PartialTranslation(relfreq,ephrase,&knoten_next,&Knoten[posPhraseStart]);
			      Knoten[posPhraseStart].add_PartialTranslation_to_Inbound(Kante);
			      knoten_next.add_PartialTranslation_to_Outbound(Kante);
				}
			if (knoten_next.getOutbound().empty())	cout << "hier ist was leer" << endl;

			Knoten.push_back(knoten_next);
			}
		}

		aStar astar(Knoten);
		astar.search();
	}


}


