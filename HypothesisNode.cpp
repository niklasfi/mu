
#include "HypothesisNode.h"

class PartialTranslation;

//Constructor

HypothesisNode::HypothesisNode(){
                bestcost= (1./0.);
}

//getter, setter
double HypothesisNode::getBestcost(){
       return bestcost;
}
void HypothesisNode::setBestcost( double cost){
     bestcost=cost;
}
vector<PartialTranslation*>& HypothesisNode::getInbound(){
                             return inbound;
}

vector<PartialTranslation*>& HypothesisNode::getOutbound(){
	return outbound;
}



void HypothesisNode::add_PartialTranslation_to_Inbound(PartialTranslation* Uebersetzung){
   inbound.push_back(Uebersetzung);  //hinten angefügt
}

void HypothesisNode::add_PartialTranslation_to_Outbound(PartialTranslation* Uebersetzung){
   outbound.push_back(Uebersetzung);  //hinten angefügt
}

void HypothesisNode::remove_Translation(unsigned int pos){
     inbound.erase(inbound.begin()+pos);  //beliebige Position wird gelöscht
}




