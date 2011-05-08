#include <vector>
#include <list>
#include <string>
#include <iostream>

#include "HypothesisNode.h"
#include "PartialTranslation.h"
#include "aStarElement.h"


class aStar {
	bool compare(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack
	
	public:
	void search(vector<HypothesisNode> vect);	//A*-Suche
}

