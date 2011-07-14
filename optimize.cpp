#include <vector>
#include <iostream>

#include <Vocab.h>
#include "decoder.h"
#include "aStar.h"
//#include "readtable.cpp"
#include "sentenceinfo.h"
#include "global.h"
#include "mert2.h"

using namespace std;


int main (int argc, char* argv[]) {
	
	if (argc < 9) {
		std::cout << "Aufruf mit: <französische Daten> <englische Daten> <Phrasentabelle> <prune_threshold> <prune_count> <bigram_daten> [Modellnr. Skalierung ...]\n";
		return 0;
	}
	vector<Cost::Model> models_selected;
	
	for(int i=7; i<argc;i++){
		std::string in = argv[i];
		Cost::Model selected_model;
		//Herausfinden, welches Modell ausgesucht wurde
		if     (in == "1") selected_model = Cost::source_to_target_phrase;
		else if(in == "2") selected_model = Cost::target_to_source_phrase;
		else if(in == "3") selected_model = Cost::source_to_target_unigram;
		else if(in == "4") selected_model = Cost::target_to_source_unigram;
		else if(in == "5") selected_model = Cost::phrase_penalty;
		else if(in == "6") selected_model = Cost::word_penalty;
		else if(in == "7") selected_model = Cost::single_count_bit;
		else if(in == "8") selected_model = Cost::source_to_target_ratio;
		else if(in == "9") selected_model = Cost::unigram_language_model;
		else if(!in.compare("10"))
			selected_model = Cost::bigram_language_model;
		else{
			cout << "invalid model seleciton!\n";
			exit (1);
		}
		
		//Fließkommazahl für die Skalierung einlesen
		std::stringstream sstream(argv[++i]); double scale;
		sstream >> scale;
		
		if(sstream.bad() || !sstream.eof()){
			cout << "could not parse floating point number for scale!\n";
			exit (1);
		}
		
		Cost::add_model(selected_model,scale); //Modell hinzufügen
	}
	
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
	//cout<<prune_threshold<<endl;
	//cout<<prune_count<<endl;

	Decoder decoder(argv[3], prune_threshold, prune_count,argv[6]);	//Decoder anlegen

	std::vector<Sentence>* f = Decoder::parseFile(decoder.flex,argv[1]);	//französiche
	std::vector<Sentence>* e = Decoder::parseFile(decoder.elex,argv[2]);	//und englische Daten einlesen
	
	Mert mert(decoder, f, e);
	cout << "Die optimierten Parameter sind" <<std::endl;
	vector<double> res = mert.optimize(decoder, f, e);
	
	for (unsigned int i=0; i< res.size(); i++){
		cout << res[i] << " ";
	}
	cout << endl;

	//Speicher befreien
	delete f;
	delete e;

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
