struct hypRefPair;

#include "hyprefpair.h"

hypRefPair::hypRefPair(Sentence* ref, nBestList* nBest):
	reference(ref),nBest(nBest){}

hypRefPair::~hypRefPair(){
	//delete nBest; nBest = 0;
}
