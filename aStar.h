#pragma once

#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>

#include "HypothesisNode.h"
#include "PartialTranslation.h"
#include "aStarElement.h"
#include "lexicon.h"
#include "ptree.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "cmath"
#include <string>


class aStar {

	vector<HypothesisNode> vect;
 	priority_queue<aStarElement, vector<aStarElement>, greater<aStarElement> > stack;

	static int prune;


	static uint max_SentenceTranslation;


	bool compare1(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack

	uint getStarElementPosition(const aStarElement& a);  //gibt an bei welchem Knoten die Teilübersetzung ist

	public:


	static PTree<PTree < double> >* schwarz;
	static Lexicon* elex;
	static Lexicon* flex;
	//Constructor
	aStar(vector<HypothesisNode>& vect);

	static void set_max_SentenceTranslation(uint size);

	void search();	//A*-Suche

	void print();   //Ausgabefunktion

	static void Suchalgorithmus(char* eingabe, PTree<PTree < double> >* blacktree, Lexicon* eLex, Lexicon* fLex);
};
