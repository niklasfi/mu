#include "PartialTranslation.h"

PartialTranslation::PartialTranslation(){
                                         cost=0;
                                         translation=0;
}

PartialTranslation::PartialTranslation(double kosten){
                                              cost= kosten;
}


PartialTranslation::PartialTranslation(double kosten, int uebersetzung, HypothesisNode* ursprung){
     this->cost=kosten;
     this->origin=ursprung;
     this->translation=uebersetzung;
}


PartialTranslation::PartialTranslation(const PartialTranslation& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;
                                                
}           
             
double PartialTranslation::getCost(){
       return cost;
}
void PartialTranslation::setCost(double kosten){
     cost=kosten;
}
int PartialTranslation::getTranslation(){
    return translation;
}
void PartialTranslation::setTranslation (int uebersetzung){
     translation=uebersetzung;
}
HypothesisNode* PartialTranslation::getNode(){
                return origin;
}
void PartialTranslation::setOrigin(HypothesisNode* Ursprung){
     origin=Ursprung;
}

PartialTranslation PartialTranslation::operator=(PartialTranslation& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;
}
