#include <vector>
#include <iostream>
#include "global.h"

using namespace std;

class aStarElement {
	public:
	double cost;	//Teilkosten
	vector<unsigned int> trl; //Teilübersetzung in IDs rückwärts aufgeschrieben

	aStarElement(const aStarElement& origin);
	aStarElement(vector<unsigned int>&  t, double c);
	void addWord(uint w);
	void print();

	bool operator<(const aStarElement& a) const;
};
