#include "aStar.h"


void dotGraph(vector<HypothesisNode> &Knoten){
		std::cout << "digraph g{\n";
		for(int i=0; i<Knoten.size(); i++){
			std::cout << "\t";
			
			for (int j=0; j<Knoten[i].getOutbound().size(); j++){
				   
			      if(i == 0) std::cout << "root";
			      else{
					cout << Knoten[i].getBestcost();
			      }
			      
			      std::cout << " -> ";
			      unsigned int position=Knoten[i].getOutbound()[j]->getDestination_pos();
			      cout << Knoten[position].getBestcost();
			      
			      std::cout << " [label=\"";
				for (int k=0; k< Knoten[i].getOutbound()[j]->getTranslation().size(); k++)
					cout << Knoten[i].getOutbound()[j]->getTranslation()[k] << " ";
				cout << Knoten[i].getOutbound()[j]->getCost() << " ";
					
				cout << "\"]\n";
			}
		}
		std::cout << "}\n";

}
void printstack(priority_queue< aStarElement, vector<aStarElement>, greater<aStarElement>> stack){
	cout << "stack :" << endl;	
	while (!stack.empty() ){
		aStarElement first=stack.top();
		for (int i=0; i<first.trl.size(); i++)	cout << first.trl[i] << ", ";
		cout << first.cost<<endl;
		stack.pop();
	}
}

uint aStar::max_SentenceTranslation=0; //Anzahl der besten(Satz)Übersetzungen
Lexicon* aStar::elex=0;
Lexicon* aStar::flex=0;
PTree< PTree <double> >* aStar::schwarz=0;
double aStar::prune=2;

//Constructor
aStar::aStar(vector<HypothesisNode>& vect):vect(vect){}


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

void aStar::print(std::vector< std::vector <unsigned int> > v){
	for(vector< vector < uint > >::iterator i = v.begin(); i != v.end(); i++){
		std::cout << lineNumber << " # ";
		for(vector<uint>::reverse_iterator it = i->rbegin(); it!=i->rend(); it++){
			std::cout << aStar::elex->getString(*it) << " ";    //ID->Wort
		}
		std::cout << "\n";
	}
}

//Eigentliche A*-Suche
std::vector<std::vector<uint> > aStar::search() {
	std::vector< std::vector <uint> > output;

	int length = vect.size();

	vector<uint> vtemp;

	aStarElement elementI(vtemp, vect.back().getBestcost(),&vect[length-1]); //erstesElement initialisiert
	stack.push(elementI);


	uint n =0;
	while(n<max_SentenceTranslation || max_SentenceTranslation==0){	//Anzahl der (Satz)Übersetzungen, die ausgegeben werden
		if(stack.empty()) break;
		if(stack.top().pos->getOutbound().size() ==0) {	//Wenn Satzanfang erreicht, dann gib Übersetzung aus und lösche entsprechendes Element
			output.push_back(stack.top().trl);
			stack.pop();
			n++;
		} else {	//Führe einen A*-Schritt für das erste Element im Stack durch

			aStarElement toExplore=stack.top(); //zu erweiternder Teilsatz wird gespeichert
			stack.pop();//altes Element wird entfernt

			vector<PartialTranslation*> v=toExplore.pos->getOutbound(); //alle möglichen Übersetzungen
			double bisherigerWeg = toExplore.cost - toExplore.pos->getBestcost();

				for(uint i = 0; i<v.size(); i++){ //bestem Element werden nun alle Möglichkeiten zugefügt
				aStarElement* anew = new aStarElement(toExplore);

				if (v[i]->getTranslation().size() == 0)	cout << "??" << endl;
				anew->addWords2(v[i]->getTranslation());

				double remainingTranslationCost = vect[v[i]->getDestination_pos()].getBestcost();
				anew->cost = v[i]->getCost() + remainingTranslationCost + bisherigerWeg; //Kosten aktualisieren

				anew->pos=&vect[v[i]->getDestination_pos()];

				stack.push(*anew);
				delete anew;
			}
			priority_queue< aStarElement, vector<aStarElement>, greater<aStarElement> > stack2=stack;
		}
	}
	
	return output;
}



void aStar::Suchalgorithmus(char* eingabe, PTree<PTree < double> >* blacktree, Lexicon* eLex, Lexicon* fLex){
     igzstream in(eingabe);
     aStar::flex=fLex;
     elex=eLex;
     schwarz=blacktree;


     string token,line;

	  unsigned int lineNumber = 0;
	
     while(getline(in,line)){
	  istringstream ist(line); //Einlesen des Satzes
	  
	  vector<unsigned int> sentence_id;

	  vector<HypothesisNode> Knoten;
	  Knoten.push_back(HypothesisNode());//initialisiert den ersten Knoten
	  Knoten[0].setBestcost(0);
	  
	  int aktPos=0; //merkt sich, wieviele Wörter schon eingelesen wurden
	  

	  while ( ist >> token){
	       Word word_id_french=flex->getWord_or_add(token); // das word zum Wort (mit 2 Bits Sprache)
	       unsigned int id_french= word_id_french.wordId(); //die id ohne sprachbits

	       sentence_id.push_back(id_french);
	       aktPos++;
	       
	       HypothesisNode knoten_next= HypothesisNode();//initialisiert den nächsten Knoten mit den bisherigen Kosten
	       
	       for (int laengePhrase=1; laengePhrase<5; laengePhrase++){
		    int posPhraseStart=aktPos-laengePhrase; //gibt die Pos. für den Knoten, auf dem die Phrase beginnt
		    if (posPhraseStart < 0)	break; //wir befinden uns am Satzanfang und es gibt keine Phrasen

		    vector<unsigned int> fphrase;
		    for (int i=posPhraseStart; i<aktPos; i++){
			 fphrase.push_back(sentence_id[i]);
		    }
			PTree<PTree <double> >* schwarzRest=schwarz->traverse(fphrase);
			if (!schwarzRest)	continue; //wenn es die französische Phrase nicht gibt, nächste überprüfen
			PTree <double>* blauBaum=&schwarzRest->c;
			

		    
		    if (blauBaum){
			 int counter=0; //nur fürs Programmieren, damit alle Fehler ausgemerzt werden 
			 for (PTree<double>::iterator it=blauBaum->begin(); it!=blauBaum->end(); it++){
			      //if (counter++==10)	continue;
			      vector<unsigned int> ephrase=it->phrase();
			      
			      double relfreq = it->c;
				
			      if (relfreq == 1./0. )	continue;
			      
			      double cost_till_aktPos=Knoten[posPhraseStart].getBestcost();
			      
			      if (cost_till_aktPos+prune > knoten_next.getBestcost())	continue; //pruning ergibt, das ist eine schlecht Übersetzung
			      
			      if(cost_till_aktPos+relfreq < knoten_next.getBestcost())	knoten_next.setBestcost(cost_till_aktPos+relfreq);
			      
			      PartialTranslation* Kante= new PartialTranslation(relfreq,ephrase,&knoten_next,posPhraseStart);
			      Knoten[posPhraseStart].add_PartialTranslation_to_Inbound(Kante);
			      knoten_next.add_PartialTranslation_to_Outbound(Kante);   
			 }
		    }
		
	       }
		if (knoten_next.getOutbound().size() == 0){
			//zuerst in das englische Lexicon einfügen
			string word_string = flex->getString(sentence_id[aktPos-1]);
			unsigned int id_english=elex->getWord_or_add(word_string);

			//dann die Kante anlegen, dabei sollen die Kosten niedrig sein, da sie sowieso genutzt werden muss, kann sie auch direkt exploriert werden
			PartialTranslation* Kante= new PartialTranslation(0,vector<unsigned int>{id_english},&Knoten[aktPos],aktPos-1);
			knoten_next.setBestcost(Knoten.back().getBestcost());
			knoten_next.add_PartialTranslation_to_Outbound(Kante);
			Knoten.back().add_PartialTranslation_to_Inbound(Kante);
		}
	       Knoten.push_back(knoten_next); //letzter Knoten (node_next) hat keine eingehenden Kanten
	         
	  }
	  
	  
	  aStar astar(Knoten);
	  astar.lineNumber = lineNumber;
	  astar.print(astar.search());
	  
	  for(unsigned int i = 0; i < Knoten.size(); i++){
		 HypothesisNode& hnode = Knoten[i];
		 for(unsigned int j = 0; j < hnode.getOutbound().size(); j++)
		 	delete hnode.getOutbound()[j];
	  }
	  
	  lineNumber ++;
	  }
	
}

