#include "aStar.h"

//Vergleichsfunktion zum Sortieren
bool compare1 (aStarElement e1, aStarElement e2) {
	return e1.cost < e2.cost;
}

uint aStar::getStarElementPosition(const aStarElement& a){
	return vect.size()-1 - a.trl.size();
}

void aStar::print(){ //gibt die oberste Kombination aus
	vector<uint> trl = stack.front().trl;
	for(vector<uint>::reverse_iterator it = trl.rbegin(); it!=trl.rend(); it++){
		std::cout << dc->elex.getString(*it) << " ";
	}
	std::cout << "\n";
}

aStar::aStar(DictionaryC* dc, vector<HypothesisNode> vect):dc(dc),vect(vect){}

//Eigentliche A*-Suche
void aStar::search() {

	int length = vect.size();

	vector<uint> vtemp;

	aStarElement elementI(vtemp, vect.back().getBestcost()); //erstesElement
	stack.push_front(elementI);

	int n=0;
	while(n<10) {	//---Später noch Zähler einfügen, um nur n beste Übersetzungen auszugeben---
		if(stack.empty()) break;

		if(getStarElementPosition(stack.front()) == 0) {	//Wenn Satzanfang erreicht, dann gib Übersetzung aus und lösche entsprechendes Element
			print();
			stack.pop_front();
			n++;
		} else {	//Führe einen A*-Schritt für das erste Element im Stack durch
			//Füge alle Möglichkeiten bis auf eine in den Stack ein

			uint posfirst = getStarElementPosition(stack.front());
			vector<PartialTranslation*> v(vect[posfirst-1].getVektor());
			uint bisherigerWeg = stack.front().cost - vect[posfirst].getBestcost();
			for(uint i = 0; i<v.size(); i++){
				aStarElement anew(stack.front());
				anew.addWord(v[i]->getTranslation());
				anew.cost = v[i]->getCost() + vect[posfirst-1].getBestcost() + bisherigerWeg;
				stack.push_back(anew);
			}
			stack.pop_front();
			stack.sort();

			/*for (int i=0; i< vect[stack.begin()->getPos()-1].getVektor().size()-1;i++) {
				string uebersetzung=elex.getString(vect[stack.begin()->getPos()-1].getVektor()[i].getTranslation());
				aStarElement element(stack.begin()->getTrl() + uebersetzung + " ",
					vect[stack.begin()->getPos()-1].getBestcost() + vect[stack.begin()->getPos()-1].getVektor()[i].getCost(),
					stack.first.getPos()-1);
				stack.push_back(element);
			}

			//Füge nun die letzte Möglichkeit ein
			stack.begin()->setTrl(stack.begin()->getTrl() + vect[stack.begin()->getPos()-1].getVektor()[i].getTranslation() + " ");
                	stack.begin()->setCost(vect[stack.begin()->getPos()-1].getBestcost() + vect[stack.begin()->getPos()-1].getVektor()[i].getCost());
                	stack.begin()->setPos(stack.first.getPos()-1);

			//Sortiere aufsteigend nach Kosten
			stack.sort(compare);*/
		}
	}
}
