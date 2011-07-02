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
#include "decoder.h"

typedef std::vector<SentenceInfo> nBestList;

class aStar {

	nBestList* nTranslations;

	vector<HypothesisNode> vect;
 	priority_queue<aStarElement, vector<aStarElement>, greater<aStarElement> > stack;
 	

	static double prune;


	static uint max_SentenceTranslation;


	bool compare1(aStarElement e1, aStarElement e2);	//Vergleichsfunktion zum Sortieren des Stack

	uint getStarElementPosition(const aStarElement& a);  //gibt an bei welchem Knoten die Teilübersetzung ist

	public:
	
	static PTree<PTree < Cost> >* schwarz;
	static Lexicon* elex;
	static Lexicon* flex;
	//Constructor
	aStar(vector<HypothesisNode>& vect);

	static void set_max_SentenceTranslation(uint size);

	void search();	//A*-Suche

	void addSentence(const aStarElement& a);   //Ausgabefunktion
	/* die Funktion nach außen(bzw. für den decoder, sie gibt die n übersetzugen für einen satz (vektor aus ID's an) */
	static nBestList* Suchalgorithmus(
	std::vector<unsigned int>sentende_id, PTree<PTree <Cost> >* blacktree, Lexicon* eLex, Lexicon* fLex);
};
