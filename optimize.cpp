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
	
	if (argc != 7) {
		std::cout << "Aufruf mit: <französische Daten> <englische Daten> <Phrasentabelle> <prune_threshold> <prune_count> <mNumber>\n";
		return 0;
	}
	double modelNumber;
	{
		std::istringstream ist(argv[6]);
		ist >> modelNumber;
		if(ist.bad()  || !ist.eof()){
			std::cout << "konnte mNumber nicht verarbeiten. (kein double)\n";
			exit(1);
		}
	}
	
	for(int i=0;i<modelNumber;i++)
		Cost::add_model((Cost::Model)(i));
// 	int mNumber = 9;
	
	double prune_threshold;
	{
		std::istringstream ist(argv[4]);
		ist >> prune_threshold;
		if(ist.bad()  || !ist.eof()){
			std::cout << "konnte prune-threshold nicht verarbeiten. (keine nichtnegative Ganzzahl)\n";
			exit(1);
		}
	}
	unsigned int prune_count;
	{
		std::istringstream ist(argv[5]);
		ist >> prune_count;
		if(ist.bad()  || !ist.eof()){
			std::cout << "konnte prune-count nicht verarbeiten. (keine nichtnegative Ganzzahl)\n";
			exit(1);
		}
	}
	aStar::set_max_SentenceTranslation(10);

	Decoder decoder(argv[3], prune_threshold, prune_count);	//Decoder anlegen

	std::vector<Decoder::Sentence>* f = Decoder::parseFile(decoder.flex,argv[1]);	//französiche
	std::vector<Decoder::Sentence>* e = Decoder::parseFile(decoder.elex,argv[2]);	//und englische Daten einlesen
	
	std::vector<Decoder::hypRefPair>* translation = decoder.translate(*f,*e);		//und beste Übersetzungen suchen

	std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > nBestList;
	
	for (unsigned int index1 = 0; index1 < translation->size(); index1++) {		//Iteriere über alle Sätze des Quelltextes
		std::pair<std::vector<uint>, std::vector<SentenceInfo> > pair_tmp;
		pair_tmp.first = *((*translation)[index1].reference);		//Speichere Referenz (=Satz des Quelltextes)
		pair_tmp.second = *((*translation)[index1].nBest);		//Speichere Übersetzungshypothesen
		nBestList.push_back(pair_tmp);
	}

	//Speicher befreien
	delete f;
	delete e;
	delete translation;

	mert(nBestList, modelNumber);	//Führe mert mit oben generierter nBestList aus

	return 0;
}

// === Alter Code === */
/*	for(int i=0;i<9;i++)
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
	return 0;*/
