#include "PartialTranslation.h"

//Constructor

PartialTranslation::PartialTranslation(){
                                         cost=(1./0.);
}

PartialTranslation::PartialTranslation(double kosten){
                                              cost= kosten;
}


PartialTranslation::PartialTranslation(double kosten, vector<unsigned int> uebersetzung, HypothesisNode* ursprung, unsigned int destination_pos) {
     this->cost=kosten;
     this->origin=ursprung;
     this->translation=uebersetzung;
     this->destination_pos=destination_pos;
}

//Copy-Constructor
PartialTranslation::PartialTranslation(const PartialTranslation& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;

}

//getter und setter
double PartialTranslation::getCost(){
       return cost;
}
void PartialTranslation::setCost(double kosten){
     cost=kosten;
}
 vector<unsigned int>  PartialTranslation::getTranslation(){
    return translation;
}
void PartialTranslation::setTranslation ( vector<unsigned int> uebersetzung){
     translation=uebersetzung;
}
HypothesisNode* PartialTranslation::getNode(){
                return origin;
}
void PartialTranslation::setOrigin(HypothesisNode* Ursprung){
     origin=Ursprung;
}

unsigned int PartialTranslation::getDestination_pos(){
	return destination_pos;
}



PartialTranslation PartialTranslation::operator=(PartialTranslation& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;
}
