
#include <iostream>
#ifndef HypothesisNode2_H
#define HypothesisNode2_H
class PartialTranslation2;  //Vordeklaration

#include <vector>
using namespace std;




class HypothesisNode2{

      private:

             double bestcost; //minimalen Kosten bis zu diesem Knoten
             vector<PartialTranslation2*> inbound; //alle m�glichen uebersetzung um zu diesem Knoten zu kommen

      public:
             HypothesisNode2(); //Constructor
             double getBestcost();
             void setBestcost(double cost);
             vector<PartialTranslation2*>& getVektor();
             void add_Translation(PartialTranslation2* Uebersetzung); //ein Pointer wird hinzugef�gt
             void remove_Translation(unsigned int pos); //es f�ngt von 0 an zu z�hlen, beliebiges Element kann gel�scht werden


};
#endif

