#include <vector>
#include <iostream>


#include "lexicon.h"
#include "aStar.h"
#include "readtable.cpp"
#include "sentenceinfo.h"
#include "global.h"
//#include "mert.cpp"
#include "simplex.cpp"

using namespace std;


int main (int argc, char* argv[]) {

	int mNumber = 1;
	vector <double> skalierung(9,0);
	std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > nBestLists;

	Lexicon elex;
	Lexicon flex;
	PTree<PTree<Cost>> schwarz;

	readTable(schwarz,flex,elex,"training/test_singlewordextract",2,10);
	
	aStar::set_max_SentenceTranslation(10);
    aStar::Suchalgorithmus("training/franzesatz",&schwarz,&elex,&flex,nBestLists);

	ifstream guess("training/englischsatz");
	string line;
	
	for(int j=0;j<nBestLists.size();j++){
		getline(guess,line);
			string token;
			istringstream s_str(line);

			while(s_str >> token) {
				nBestLists[j].first.push_back(elex.getWord_or_add(token));
			}
	}


	skalierung=DownhillSimplex(nBestLists);
	for(int i=0; i<9;i++) cout<<skalierung[i]<<endl;
	//mert(nBestLists,mNumber);
	return 0;
}
