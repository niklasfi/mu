#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include <Vocab.h>
#include "gzstream/gzstream.h"
#include "aStar.h"
#include "cost.h"


using namespace std;

int main(int argc, char* argv[]){
	 /*Diese Modelle gibt es:
	  * source_to_target_phrase,
		target_to_source_phrase,
		source_to_target_unigram,
		target_to_source_unigram,
		phrase_penalty,
		word_penalty,
		single_count_bit,
		source_to_target_ratio,
		unigram_language_model
	 bool sttPhrase = false,
		ttsPhrase = false,
		sttUnigram = false,
		ttsUnigram = false,
		phrasePenalty = false, 
		wordPenalty = false, 
		singleCountBit = false, 
		stRatio = false,//stRatio =Source-Target-Ratio 
		unigram_language_model = false; */
		
		

	if(argc<7){
		cout<<"usage: template.exe <prune-threshold> <prune-count> <sentenceCount> <phrasentabelle> <quelltext> <bigram_daten> [Modellnr. Skalierung ...]\n";
		cout << " 1 source_to_target_phrase,\n 2 target_to_source_phrase, \n 3 source_to_target_unigram,\n 4 target_to_source_unigram, \n 5 phrase_penalty,\n 6 word_penalty,\n 7 single_count_bit, \n 8 source_to_target_ratio, \n 9 unigram_language_model \n 10 bigram_language_model" << endl;
		exit(1);
	}
	if (argc<8){
		cout << "Geben Sie min 1 Model an! z.B. 1 1.0 8 1.3 9 .3\n"
		<< "1 source_to_target_phrase,\n 2 target_to_source_phrase, \n 3 source_to_target_unigram,\n 4 target_to_source_unigram, \n 5 phrase_penalty,\n 6 word_penalty,\n 7 single_count_bit, \n 8 source_to_target_ratio, \n 9 unigram_language_model \n 10 bigram_language_model" <<endl;
		exit(1);
	}
	
	//cerr << "ist die Phrasentabelle nach relfreq sortiert? Sonst schmeißen wir gute übersetzungen weg!!"<<endl;
	
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
		std::istringstream ist(argv[1]);
		ist >> prune_threshold;
		if(ist.bad()  || !ist.eof()){
			std::cout << "konnte prune-threshold nicht verarbeiten. (keine nichtnegative Ganzzahl)\n";
			exit(1);
		}
	}
	unsigned int prune_count;
	{
		std::istringstream ist(argv[2]);
		ist >> prune_count;
		if(ist.bad()  || !ist.eof()){
			std::cout << "konnte prune-count nicht verarbeiten. (keine nichtnegative Ganzzahl)\n";
			exit(1);
		}
	}
	unsigned int sentenceCount;{
		std::istringstream ist(argv[3]);
		ist >> sentenceCount;
		if(ist.bad()  || !ist.eof()){
			std::cout << "konnte SentenceCount nicht verarbeiten. (keine nichtnegative Ganzzahl)\n";
			exit(1);
		}
	}
	
	aStar::set_max_SentenceTranslation(sentenceCount);
	
	Decoder decoder(argv[4], prune_threshold, prune_count,argv[6]);

	std::vector<Sentence>* f = Decoder::parseFile(decoder.flex,argv[5]);
	
	std::vector<nBestList> translation = decoder.translate(*f);
	
	
	for (unsigned int i=0; i< translation.size(); i++){
		for (unsigned int k=0; k< translation[i].size(); k++){
			for (unsigned int j=0; j<translation[i][k].sentence.size(); j++)
				cout << decoder.elex->getWord(translation[i][k].sentence[j]) << " ";
			
			cout << endl;
		}
	}
	
	delete f;
}
