
#include "HypothesisNode_2PUNKT0.h"

class PartialTranslation2;

//Constructor

HypothesisNode2::HypothesisNode2(){
                bestcost= (1./0.);
}

//getter, setter
double HypothesisNode2::getBestcost(){
       return bestcost;
}
void HypothesisNode2::setBestcost( double cost){
     bestcost=cost;
}
vector<PartialTranslation2*>& HypothesisNode2::getVektor(){
                             return inbound;
}



void HypothesisNode2::add_Translation(PartialTranslation2* Uebersetzung){
   inbound.push_back(Uebersetzung);  //hinten angefügt
}

void HypothesisNode2::remove_Translation(unsigned int pos){
     inbound.erase(inbound.begin()+pos);  //beliebige Position wird gelöscht
}




