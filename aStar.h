#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <cctype>

#include "HypothesisNode.h"
#include "PartialTranslation.h"
#include "aStarElement.h"
#include "dictionaryc.h"
#include "global.h"


class aStar {
	DictionaryC* dc;
	vector<HypothesisNode> vect;
  list<aStarElement> stack;


	bool compare1(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack

	uint getStarElementPosition(const aStarElement& a);

	public:
	aStar(DictionaryC* dc, vector<HypothesisNode> vect);
	void search();	//A*-Suche

	void print();
};

