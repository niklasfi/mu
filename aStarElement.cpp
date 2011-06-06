#include "aStarElement.h"

//Constructor
aStarElement::aStarElement(vector<unsigned int>&  t, double c): cost(c), trl(t){}
aStarElement::aStarElement(const aStarElement& origin): cost(origin.cost),trl(origin.trl){}

void aStarElement::addWord(uint w){
	trl.push_back(w);      //vorsicht, auch hier rückwärts denken!
}

void aStarElement::addWords2(std::vector<uint> w){
     for (int i=0; i<w.size(); i++)
	  addWord(w[i]);
}

bool aStarElement::operator<(const aStarElement& a) const{
	return cost<a.cost; 
}

bool aStarElement::operator>(const aStarElement& a) const{
	return cost>a.cost;
}
