#include <vector>
#include "cost.h"

#ifndef PartialTranslation_H
#define PartialTranslation_H
class HypothesisNode; //Vorabdeklaration

#include <iostream>
using namespace std;

class PartialTranslation{

      private:

             Cost cost; //kosten für die Modelle
             vector<unsigned int> translation; //ID des Wortes(siehe lex)
             HypothesisNode* origin;
             unsigned int destination_pos; //gibt die postiition des zielknoten im vector an

      public://Constructor
             PartialTranslation();
             PartialTranslation(Cost kosten);
             PartialTranslation(Cost kosten, vector<unsigned int> uebersetzung, HypothesisNode* ursprung, unsigned int destination);
             PartialTranslation(const PartialTranslation& original);
			 //getter und setter
             Cost getCost();
             void setCost(Cost kosten);
             vector<unsigned int> getTranslation();
             void setTranslation (vector<unsigned int> uebersetzung);
             HypothesisNode* getNode();
             void setOrigin(HypothesisNode* Ursprung);
             unsigned int getDestination_pos();

             PartialTranslation& operator=(PartialTranslation& original);
};
#endif
