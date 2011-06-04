#include <vector>

#ifndef PartialTranslation_H
#define PartialTranslation_H
class HypothesisNode; //Vorabdeklaration

#include <iostream>
using namespace std;

class PartialTranslation{

      private:
             
             double cost; //relFreq
             vector<unsigned int> translation; //ID des Wortes(siehe lex)
             HypothesisNode* origin;
             
      public://Constructor
             PartialTranslation();
             PartialTranslation(double kosten);
             PartialTranslation(double kosten, vector<unsigned int> uebersetzung, HypothesisNode* ursprung);
             PartialTranslation(const PartialTranslation& original);
			 //getter und setter
             double getCost();
             void setCost( double kosten);
             vector<unsigned int> getTranslation();
             void setTranslation (vector<unsigned int> uebersetzung);
             HypothesisNode* getNode();
             void setOrigin(HypothesisNode* Ursprung);

             PartialTranslation operator=(PartialTranslation& original);
};
#endif
