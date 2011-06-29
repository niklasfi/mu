#include "aStar.h"

using namespace std;

void dotGraph(vector<HypothesisNode> &Knoten, Lexicon* elex){
		//int knotenid=0;
		std::cout << "digraph g{\n";
		for(unsigned int i=0; i<Knoten.size(); i++){
			std::cout << "\t";
			
			for (unsigned int j=0; j<Knoten[i].getOutbound().size(); j++){
				   
			      if(i == 0) std::cout << "root";
			      else{
					cout << Knoten[i].getBestcost().cost();
			      }
			      
			      std::cout << " -> ";
			      unsigned int position=Knoten[i].getOutbound()[j]->getDestination_pos();
			      cout << Knoten[position].getBestcost().cost();
			      
			      std::cout << " [label=\"  ";
				for (unsigned int k=0; k< Knoten[i].getOutbound()[j]->getTranslation().size(); k++)
					cout << elex->getString(Knoten[i].getOutbound()[j]->getTranslation()[k]) << "	";
				cout << "Kosten: "<< Knoten[i].getOutbound()[j]->getCost().cost() << " ";
					
				cout << "\"]\n";
			}
		}
		std::cout << "}\n";

}
void printstack(priority_queue< aStarElement, vector<aStarElement>, greater<aStarElement>> stack){
	while (!stack.empty() ){
		aStarElement first=stack.top();
		for (unsigned int i=0; i<first.trl.size(); i++)	cout << first.trl[i] << ", ";
		cout << first.cost.cost()<<endl;
		stack.pop();
	}
}

uint aStar::max_SentenceTranslation=0; //Anzahl der besten(Satz)Übersetzungen
Lexicon* aStar::elex=0;
Lexicon* aStar::flex=0;
PTree< PTree <Cost> >* aStar::schwarz=0;
double aStar::prune=2;

//Constructor
aStar::aStar(vector<HypothesisNode>& vect):vect(vect){
	nTranslations= new vector<SentenceInfo>;	
}


void aStar::set_max_SentenceTranslation(uint size){
	//if(aStar::max_SentenceTranslation != 0) throw max_SentenceTranslationAlreadySetException();
	aStar::max_SentenceTranslation = size;
}


void aStar::addSentence(const aStarElement& a){
	vector< unsigned int> v_id;
	for(int i=a.trl.size()-1; i>=0; i--){
		v_id.push_back(a.trl[i]);    //ID->Wort
	}
	SentenceInfo tmp;
	tmp.sentence=v_id;
	tmp.cost=a.cost;
	nTranslations->push_back(tmp);
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
		if(stack.top().pos->getOutbound().size() ==0) {	//Wenn Satzanfang erreicht, dann gib Übersetzung aus und lösche entsprechendes Element
			addSentence(stack.top());
			stack.pop();
			n++;
		} else {	//Führe einen A*-Schritt für das erste Element im Stack durch

			aStarElement toExplore=stack.top(); //zu erweiternder Teilsatz wird gespeichert
			stack.pop();//altes Element wird entfernt

			vector<PartialTranslation*> v=toExplore.pos->getOutbound(); //alle möglichen Übersetzungen
			Cost bisherigerWeg = toExplore.cost - toExplore.pos->getBestcost();

				for(uint i = 0; i<v.size(); i++){ //bestem Element werden nun alle Möglichkeiten zugefügt
				aStarElement* anew = new aStarElement(toExplore);

				if (v[i]->getTranslation().size() == 0)	cout << "??" << endl;
				anew->addWords2(v[i]->getTranslation());

				Cost remainingTranslationCost = vect[v[i]->getDestination_pos()].getBestcost();
				anew->cost = v[i]->getCost() + remainingTranslationCost + bisherigerWeg; //Kosten aktualisieren

				anew->pos=&vect[v[i]->getDestination_pos()];

				stack.push(*anew);
				delete anew;
			}
			priority_queue< aStarElement, vector<aStarElement>, greater<aStarElement> > stack2=stack;
		}
	}
	
}



void aStar::Suchalgorithmus(const char* eingabe, PTree<PTree <Cost> >* blacktree, Lexicon* eLex, Lexicon* fLex, vector < pair < vector <unsigned int>, vector < SentenceInfo> > >& nBestList){
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
	  Cost startkosten(0);
	  Knoten[0].setBestcost(startkosten);
	  //cout << "start kosten " << Knoten[0].getBestcost().cost() << endl;
	  
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
			PTree<PTree <Cost> >* schwarzRest=schwarz->traverse(fphrase);
			if (!schwarzRest)	continue; //wenn es die französische Phrase nicht gibt, nächste überprüfen
			PTree <Cost>* blauBaum=&schwarzRest->c;
			

		    
		    if (blauBaum){
			 //int counter=0; //nur fürs Programmieren, damit alle Fehler ausgemerzt werden 
			 for (PTree<Cost>::iterator it=blauBaum->begin(); it!=blauBaum->end(); it++){
			      //if (counter++==10)	continue;
			      vector<unsigned int> ephrase=it->phrase();
			      
			      Cost relfreq = it->c;
				
			      if (relfreq.cost() == 1./0. )	continue;
			      
			      double cost_till_aktPos=Knoten[posPhraseStart].getBestcost().cost();
			      
			      if (cost_till_aktPos+prune > knoten_next.getBestcost().cost())	continue; //pruning ergibt, das ist eine schlecht Übersetzung
			      
			      if(cost_till_aktPos+relfreq.cost() < knoten_next.getBestcost().cost())	knoten_next.setBestcost(Knoten[posPhraseStart].getBestcost()+relfreq.cost());
			      
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
			PartialTranslation* Kante= new PartialTranslation(Cost(0),vector<unsigned int>{id_english},&Knoten[aktPos],aktPos-1);
			knoten_next.setBestcost(Knoten.back().getBestcost());
			knoten_next.add_PartialTranslation_to_Outbound(Kante);
			Knoten.back().add_PartialTranslation_to_Inbound(Kante);
		}
	       Knoten.push_back(knoten_next); //letzter Knoten (node_next) hat keine eingehenden Kanten
	       
	  }
	  
	  //dotGraph(Knoten, elex);
	  aStar astar(Knoten);
	  astar.lineNumber = lineNumber;
		astar.search();
		pair < vector <unsigned int>, vector < SentenceInfo> > tmp;
		nBestList.push_back(tmp);
		nBestList.back().second=*astar.nTranslations;	  
//		SentenceInfo bla=*astar.nTranslations;
	for(unsigned int i = 0; i < Knoten.size(); i++){
		 HypothesisNode& hnode = Knoten[i];
		 for(unsigned int j = 0; j < hnode.getOutbound().size(); j++)
		 	delete hnode.getOutbound()[j];
	  }
	//delete[] astar.nTranslations;
	  
	  lineNumber ++;
	  }
}

