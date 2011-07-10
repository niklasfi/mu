#pragma once

#include <vector>
#include "sentence.h"
#include "nbestlist.h"

//struct um eine "nBestListe"+Referenzübersetzung für einen ganzen Text zu speichern
struct hypRefPair{
	Sentence* reference;
	nBestList* nBest;  
	hypRefPair(Sentence* ref, nBestList* hyp);
	~hypRefPair();
};
