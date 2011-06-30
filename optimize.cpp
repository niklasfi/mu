#include <vector>
#include <iostream>

#include "lexicon.h"
#include "aStar.h"
#include "readtable.cpp"
#include "sentenceinfo.h"
#include "global.h"
#include "mert.cpp"
#include "simplex.cpp"

using namespace std;


int main (int argc, char* argv[]) {
	for(int i=0;i<9;i++)
		Cost::add_model((Cost::Model)(i));
	int mNumber = 9;
	vector <double> skalierung(9,0);
	std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > >* nBestLists = new std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > >;

	Lexicon elex;
	Lexicon flex;
	PTree<PTree<Cost>>* schwarz = new PTree<PTree<Cost>>;

	readTable(*schwarz,flex,elex,"training/phraseextract_small_sorted",4,10);
	
	aStar::set_max_SentenceTranslation(10);
    aStar::Suchalgorithmus("training/f-test4",schwarz,&elex,&flex,*nBestLists);

	ifstream guess("training/e-test4");
	string line;
	
	for(unsigned int j=0;j<nBestLists->size();j++){
		getline(guess,line);
			string token;
			istringstream s_str(line);

			while(s_str >> token) {
				(*nBestLists)[j].first.push_back(elex.getWord_or_add(token));
			}
	}


	//skalierung=DownhillSimplex(*nBestLists);
	//for(unsigned int i=0; i<9;i++) cout<<skalierung[i]<<endl;
	mert(*nBestLists,mNumber);
	return 0;
}
