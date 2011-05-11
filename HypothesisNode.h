
#include <iostream>
#ifndef HypothesisNode_H
#define HypothesisNode_H
class PartialTranslation;  //Vordeklaration

#include <vector>
using namespace std;




class HypothesisNode{

      private:

             double bestcost; //minimalen Kosten bis zu diesem Knoten
             vector<PartialTranslation*> inbound; //alle m�glichen uebersetzung um zu diesem Knoten zu kommen

      public:
             HypothesisNode(); //Constructor
             double getBestcost();
             void setBestcost(double cost);
             vector<PartialTranslation*>& getVektor();
             void add_Translation(PartialTranslation* Uebersetzung); //ein Pointer wird hinzugef�gt
             void remove_Translation(unsigned int pos); //es f�ngt von 0 an zu z�hlen, beliebiges Element kann gel�scht werden


};
#endif

