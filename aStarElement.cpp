#include "aStarElement.h"

//Constructor
aStarElement::aStarElement(vector<unsigned int>&  t, Cost c, HypothesisNode* pos): cost(c), trl(t), pos(pos){}
aStarElement::aStarElement(const aStarElement& origin): cost(origin.cost),trl(origin.trl),pos(origin.pos){}

void aStarElement::addWord(uint w){
	trl.push_back(w);      //vorsicht, auch hier rückwärts denken!
}

void aStarElement::addWords2(std::vector<uint> w){
     for (int i=0; i<w.size(); i++)
	  addWord(w[w.size()-1-i]); //die Phrase muss umgekehrt eingefügt werden!
}

bool aStarElement::operator>(const aStarElement& a) const{
	return cost.cost()>a.cost.cost();
}
bool aStarElement::operator<(const aStarElement& a) const{
	return cost.cost()<a.cost.cost();
}

