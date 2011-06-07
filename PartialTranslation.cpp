#include "PartialTranslation.h"

//Constructor

PartialTranslation::PartialTranslation(){
                                         cost=(1./0.);
}

PartialTranslation::PartialTranslation(double kosten){
                                              cost= kosten;
}


PartialTranslation::PartialTranslation(double kosten, vector<unsigned int> uebersetzung, HypothesisNode* ursprung, HypothesisNode* destination){
     this->cost=kosten;
     this->origin=ursprung;
     this->translation=uebersetzung;
     this->destination=destination;
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

HypothesisNode* PartialTranslation::getDestination(){
	return destination;
}



PartialTranslation PartialTranslation::operator=(PartialTranslation& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;
}
