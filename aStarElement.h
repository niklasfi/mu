#include <string>

class aStarElement {
	double cost;	//Teilkosten
	std:string trl; //Teilübersetzung
	int pos;	//aktueller HypothesisNode

	aStarElement(std:string t, double c);
	double getCost();
	sdt:string getTrl();
	int getPos();
	setCost(double c);
	setTrl(sdt:string t);
	setPost (int p);
}
