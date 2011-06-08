#include <vector>
#include <iostream>
#include "global.h"
#include "HypothesisNode.h"

using namespace std;

class aStarElement {
	public:
	double cost;	//Teilkosten
	vector<unsigned int> trl; //Teilübersetzung in IDs rückwärts aufgeschrieben
	HypothesisNode* pos; //gibt an bis zu welchem Knoten die Übersetzung gekommen ist

	//Constructor
	aStarElement(const aStarElement& origin);
	aStarElement(vector<unsigned int>&  t, double c, HypothesisNode* pos);


	void addWord(uint w); //erweitert Teilübersetzung um Wort w
	void addWords2(vector<uint> w); //erweitert Teilübersetzung um die Wörter aus dem Vektor w

	bool operator<(const aStarElement& a) const;
	bool operator>(const aStarElement& a) const;
};
