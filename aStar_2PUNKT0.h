#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>

#include "HypothesisNode_2PUNKT0.h"
#include "PartialTranslation_2PUNKT0.h"
#include "aStarElement.h"
#include "lexicon.h"
#include "ptree.h"
#include "gzstream/gzstream.h"
#include "global.h"


class aStar2 {
	
	vector<HypothesisNode2> vect;
 	list<aStarElement> stack;
	
	static int prune;
	
	
	static uint max_SentenceTranslation;


	bool compare1(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack

	uint getStarElementPosition(const aStarElement& a);  //git an bei welchem Knoten die Teilübersetzung ist

	public:
	     
	
	static PTree<PTree < double> >* schwarz;
	static Lexicon* elex;
	static Lexicon* flex;
	//Constructor
	aStar2(vector<HypothesisNode2> vect);

	static void set_max_SentenceTranslation(uint size);

	void search();	//A*-Suche

	void print();   //Ausgabefunktion
	
	static void Suchalgorithmus(char* eingabe, PTree<PTree < double> >* blacktree, Lexicon* eLex, Lexicon* fLex);
};

