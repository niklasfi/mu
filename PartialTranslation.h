

#ifndef PartialTranslation_H
#define PartialTranslation_H
class HypothesisNode;

#include <iostream>
using namespace std;

class PartialTranslation{

      private:
             
             double cost;
             unsigned int translation;
             HypothesisNode* origin;
             
      public:
             PartialTranslation();
             PartialTranslation(double kosten);
             PartialTranslation(double kosten, int uebersetzung, HypothesisNode* ursprung);
             PartialTranslation(const PartialTranslation& original);
             double getCost();
             void setCost( double kosten);
             int getTranslation();
             void setTranslation (int uebersetzung);
             HypothesisNode* getNode();
             void setOrigin(HypothesisNode* Ursprung);
             PartialTranslation operator=(PartialTranslation& original);
};
#endif
