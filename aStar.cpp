#include "aStar.h"

//Constructor
aStar::aStar(DictionaryC* dc, vector<HypothesisNode> vect):dc(dc),vect(vect){}

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

	int n=0;
	while(n<10) {	//Anzahl der (Satz)Übersetzungen, die ausgegeben werden
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
			stack.sort(); 

		}
	}
}
