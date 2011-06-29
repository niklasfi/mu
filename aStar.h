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
#include "sentenceinfo.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "cmath"
#include <string>
#include "cost.h"


class aStar {

	vector<SentenceInfo>* nTranslations;

	vector<HypothesisNode> vect;
 	priority_queue<aStarElement, vector<aStarElement>, greater<aStarElement> > stack;
 	

	static double prune;


	static uint max_SentenceTranslation;


	bool compare1(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack

	uint getStarElementPosition(const aStarElement& a);  //gibt an bei welchem Knoten die Teilübersetzung ist

	public:
	
	unsigned int lineNumber;

	static PTree<PTree < Cost> >* schwarz;
	static Lexicon* elex;
	static Lexicon* flex;
	//Constructor
	aStar(vector<HypothesisNode>& vect);

	static void set_max_SentenceTranslation(uint size);

	std::vector<std::vector<unsigned int> > search();	//A*-Suche

	void addSentence(const aStarElement& a);   //Ausgabefunktion

	static void Suchalgorithmus(char* eingabe, PTree<PTree <Cost> >* blacktree, Lexicon* eLex, Lexicon* fLex, vector < pair < vector <unsigned int>, vector < SentenceInfo> > >& nBestList);
	static void Suchalgorithmus2(char* eingabe, PTree<PTree < Cost> >* blacktree, Lexicon* eLex, Lexicon* fLex);
     
};
