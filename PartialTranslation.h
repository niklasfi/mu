#include <vector>

#ifndef PartialTranslation2_H
#define PartialTranslation2_H
class HypothesisNode2; //Vorabdeklaration

#include <iostream>
using namespace std;

class PartialTranslation2{

      private:
             
             double cost; //relFreq
             vector<unsigned int> translation; //ID des Wortes(siehe lex)
             HypothesisNode2* origin;
             
      public://Constructor
             PartialTranslation2();
             PartialTranslation2(double kosten);
             PartialTranslation2(double kosten, vector<unsigned int> uebersetzung, HypothesisNode2* ursprung);
             PartialTranslation2(const PartialTranslation2& original);
			 //getter und setter
             double getCost();
             void setCost( double kosten);
             vector<unsigned int> getTranslation();
             void setTranslation (vector<unsigned int> uebersetzung);
             HypothesisNode2* getNode();
             void setOrigin(HypothesisNode2* Ursprung);

             PartialTranslation2 operator=(PartialTranslation2& original);
};
#endif
