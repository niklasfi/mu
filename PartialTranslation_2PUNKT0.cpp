#include "PartialTranslation_2PUNKT0.h"

//Constructor

PartialTranslation2::PartialTranslation2(){
                                         cost=(1./0.);
}

PartialTranslation2::PartialTranslation2(double kosten){
                                              cost= kosten;
}


PartialTranslation2::PartialTranslation2(double kosten, vector<unsigned int> uebersetzung, HypothesisNode2* ursprung){
     this->cost=kosten;
     this->origin=ursprung;
     this->translation=uebersetzung;
}

//Copy-Constructor
PartialTranslation2::PartialTranslation2(const PartialTranslation2& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;
                                                
}           

//getter und setter            
double PartialTranslation2::getCost(){
       return cost;
}
void PartialTranslation2::setCost(double kosten){
     cost=kosten;
}
 vector<unsigned int>  PartialTranslation2::getTranslation(){
    return translation;
}
void PartialTranslation2::setTranslation ( vector<unsigned int> uebersetzung){
     translation=uebersetzung;
}
HypothesisNode2* PartialTranslation2::getNode(){
                return origin;
}
void PartialTranslation2::setOrigin(HypothesisNode2* Ursprung){
     origin=Ursprung;
}



PartialTranslation2 PartialTranslation2::operator=(PartialTranslation2& original){
     this->cost=original.cost;
     this->origin=original.origin;
     this->translation=original.translation;
}
