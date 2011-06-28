#include <vector>
#include <iostream>

#include "opt_translate.cpp"
#include "sentenceinfo.h"
#include "global.h"
#include "mert.cpp"


int main (int argc, char* argv[]) {

	int mNumber = 1;
	std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > nBestLists;
	nBestLists = translate(argc,argv);
	mert(nBestLists,mNumber);
	return 0;
}
