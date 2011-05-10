
#include <iostream>
#ifndef HypothesisNode_H
#define HypothesisNode_H
class PartialTranslation;  //Vordeklaration

#include <vector>
using namespace std;




class HypothesisNode{

      private:

             double bestcost;
             vector<PartialTranslation*> inbound;

      public:
             HypothesisNode();
             double getBestcost();
             void setBestcost(double cost);
             vector<PartialTranslation*>& getVektor();
             void add_Translation(PartialTranslation* Uebersetzung);
             void remove_Translation(unsigned int pos); //es fängt von 0 an zu zählen


};
#endif

