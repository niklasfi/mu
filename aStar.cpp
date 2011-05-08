#include "aStar.h"

//Vergleichsfunktion zum Sortieren
bool compare (aStarElement e1, aStarElement e2) {
	return e1.cost < e2.cost;
}

//Eigentliche A*-Suche
void aStar::search(vector<HypothesisNode> vect) {

	int length = vect.size();
	list<aStarElement> stack;
	aStarElement elementI("",vect[length-1].getBestcost(),length-1)
	stack.push_first(elementI);

	while(true) {	//---Später noch Zähler einfügen, um nur n beste Übersetzungen auszugeben---
		if(stack.empty()) break;
		
		if(!stack.first().getPos()) {	//Wenn Satzanfang erreicht, dann gib Übersetzung aus und lösche entsprechendes Element
			cout << stack.first().getTrl();
			stack.pop_front();
		} else {	//Führe einen A*-Schritt für das erste Element im Stack durch			
			//Füge alle Möglichkeiten bis auf eine in den Sack ein
			for (int i=0; i< vector[stack.first().getPos()-1].getVektor().length()-1;i++) {
				aStarElement element(stack.first().getTrl() + vector[stack.first().getPos()-1].getVektor()[i].getTranslation() + " ",
					vector[stack.first().getPos()-1].getBestcost() + vector[stack.first().getPos()-1].getVektor()[i].getCost(),
					stack.first.getPos()-1);
				stack.push_back(element);
			}

			//Füge nun die letzte Möglichkeit ein
			stack.first().setTrl(stack.first().getTrl() + vector[stack.first().getPos()-1].getVektor()[i].getTranslation() + " ");
                	stack.first().setCost(vector[stack.first().getPos()-1].getBestcost() + vector[stack.first().getPos()-1].getVektor()[i].getCost());
                	stack.first().setPos(stack.first.getPos()-1);

			//Sortiere aufsteigend nach Kosten
			stack.sort(compare);
		}
	}
}
