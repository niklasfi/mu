
#include "HypothesisNode.h"

class PartialTranslation;

HypothesisNode::HypothesisNode(){
                bestcost= 0;
}


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
   inbound.push_back(Uebersetzung);
}

void HypothesisNode::remove_Translation(unsigned int pos){
     inbound.erase(inbound.begin()+pos);
}




