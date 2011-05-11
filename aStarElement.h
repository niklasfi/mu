#include <vector>
#include <iostream>
#include "global.h"

using namespace std;

class aStarElement {
	public:
	double cost;	//Teilkosten
	vector<unsigned int> trl; //Teilübersetzung in IDs rückwärts aufgeschrieben

	//Constructor
	aStarElement(const aStarElement& origin);
	aStarElement(vector<unsigned int>&  t, double c);


	void addWord(uint w); //erweitert Teilübersetzung um Wort w

	bool operator<(const aStarElement& a) const;
};
