#include "aStarElement.h"

//Constructor
aStarElement::aStarElement(vector<unsigned int>&  t, double c): cost(c), trl(t){}
aStarElement::aStarElement(const aStarElement& origin): cost(origin.cost),trl(origin.trl){}

void aStarElement::addWord(uint w){
	trl.push_back(w);      //vorsicht, auch hier rückwärts denken!
}

bool aStarElement::operator<(const aStarElement& a) const{
	return cost<a.cost;
}
