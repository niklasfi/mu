
#include <iostream>
#ifndef HypothesisNode2_H
#define HypothesisNode2_H
class PartialTranslation2;  //Vordeklaration

#include <vector>
using namespace std;




class HypothesisNode2{

      private:

             double bestcost; //minimalen Kosten bis zu diesem Knoten
             vector<PartialTranslation2*> inbound; //alle möglichen uebersetzung um zu diesem Knoten zu kommen

      public:
             HypothesisNode2(); //Constructor
             double getBestcost();
             void setBestcost(double cost);
             vector<PartialTranslation2*>& getVektor();
             void add_Translation(PartialTranslation2* Uebersetzung); //ein Pointer wird hinzugefügt
             void remove_Translation(unsigned int pos); //es fängt von 0 an zu zählen, beliebiges Element kann gelöscht werden


};
#endif

