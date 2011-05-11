
#include "HypothesisNode.h"

class PartialTranslation;

//Constructor

HypothesisNode::HypothesisNode(){
                bestcost= 0;
}

//getter, setter
double HypothesisNode::getBestcost(){
       return bestcost;
}
void HypothesisNode::setBestcost( double cost){
     bestcost=cost;
}
vector<PartialTranslation*>& HypothesisNode::getVektor(){
                             return inbound;
}



void HypothesisNode::add_Translation(PartialTranslation* Uebersetzung){
   inbound.push_back(Uebersetzung);  //hinten angefügt
}

void HypothesisNode::remove_Translation(unsigned int pos){
     inbound.erase(inbound.begin()+pos);  //beliebige Position wird gelöscht
}




