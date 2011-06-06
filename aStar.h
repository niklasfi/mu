#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <cctype>

#include "HypothesisNode.h"
#include "PartialTranslation.h"
#include "aStarElement.h"
#include "dictionaryc.h"
#include "gzstream/gzstream.h"
#include "global.h"


class aStar {
	DictionaryC* dc;
	vector<HypothesisNode> vect;
 	priority_queue<aStarElement, vector<aStarElement>, greater<aStarElement> > stack;

	static uint max_SentenceTranslation;


	bool compare1(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack

	uint getStarElementPosition(const aStarElement& a);  //git an bei welchem Knoten die Teilübersetzung ist

	public:
	//Constructor
	aStar(DictionaryC* dc, vector<HypothesisNode> vect);

	static void set_max_SentenceTranslation(uint size);

	void search();	//A*-Suche

	void print();   //Ausgabefunktion
	
	static void Suchalgorithmus(DictionaryC dic,char* eingabe);
};

