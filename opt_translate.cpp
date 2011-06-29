#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "lexicon.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "ptree.h"
#include "opt_aStar.h"
#include "cost.h"
#include "sentenceinfo.h"

using namespace std;

void readTable(
	PTree<PTree<Cost>>& schwarz,
	Lexicon& flex,
	Lexicon& elex,
	const char filename[],
	double prune_threshold,
	int prune_count
){
//==================Einlesen der Phrasentabelle============================
	PTree< pair <unsigned int, double> > pruningTree; //speichert für jede Übersetzung die Anzahl der eingelesenen Übersetzungen und die beste Übersetzung
	pair <unsigned int, double> pruningStart; //die Startkombi für den PruningTree
	pruningStart.first=0;
	pruningStart.second=(1./0.);

	igzstream in(filename);
	string line,token;


	while(getline(in,line)){
		stringstream ist(line);


		double relfreq_f, relfreq_e, source_to_target, target_to_source, unigram_sprachmodell;
		unsigned int singlecf, singlece; 
		vector<uint> ephrase, fphrase;

		//Ausgabe: relfreq_f relfreq_e # quellphrase # zielphrase # singlecf singlece # source_to_target target_to_source #  unigram-sprachmodell
		ist >> relfreq_f  >> relfreq_e >>token; // token für "#"

		while(ist>>token && token != "#"){
			fphrase.push_back(flex.getWord_or_add(token));
		}
		while(ist>>token && token != "#"){
			ephrase.push_back(elex.getWord_or_add(token));
		}
		ist >> singlecf >> singlece >> token >> source_to_target >> target_to_source >> token >> unigram_sprachmodell;

		Cost kosten=Cost();
		kosten.calc(relfreq_f, relfreq_e, fphrase, ephrase, singlecf, singlece, source_to_target, target_to_source, unigram_sprachmodell);
		double kosten_insgesamt=kosten.cost();

		pair< unsigned int, double>* pruning_infos=&pruningTree.traverse(fphrase,true,pruningStart)->c;

		if (kosten_insgesamt > pruning_infos->second+prune_threshold || pruning_infos->first >prune_count)	continue; //pruning ergibt, wir wollen es nicht in den Ptree mitaufnehmen

		//if (kosten_insgesamt< pruning_infos->second)	pruning_infos->second=kosten_insgesamt;  _jetzt irrelevant, da ich von einer geordneten eingabe ausgehe
		pruning_infos->first++;



		schwarz.traverse(fphrase,true)->c.traverse(ephrase,true,Cost(1./0.))->c = kosten;

	}
	//cerr << " schwarz erstellt" << endl;

}

std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > translate (int argc, char* argv[]){
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
		
		

	if(argc<6){
		cout<<"usage: template.exe <prune-threshold> <prune-count> <sentenceCount> <phrasentabelle> <quelltext> [Modellnr. Skalierung ...]\n";
		cout << " 1 source_to_target_phrase,\n 2 target_to_source_phrase, \n 3 source_to_target_unigram,\n 4 target_to_source_unigram, \n 5 phrase_penalty,\n 6 word_penalty,\n 7 single_count_bit, \n 8 source_to_target_ratio, \n 9 unigram_language_model" << endl;
		exit(1);
	}
	if (argc<7){
		cout << "Geben Sie min 1 Model an! z.B. 1 1.0 8 1.3 9 .3\n"
		<< "1 source_to_target_phrase,\n 2 target_to_source_phrase, \n 3 source_to_target_unigram,\n 4 target_to_source_unigram, \n 5 phrase_penalty,\n 6 word_penalty,\n 7 single_count_bit, \n 8 source_to_target_ratio, \n 9 unigram_language_model" <<endl;
		exit(1);
	}
	
	//cerr << "ist die Phrasentabelle nach relfreq sortiert? Sonst schmeißen wir gute übersetzungen weg!!"<<endl;
	
	vector<Cost::Model> models_selected;
	
	for(int i=6; i<argc;i++){
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
	
	Lexicon elex;
	Lexicon flex;
	PTree<PTree<Cost>> schwarz;

	readTable(schwarz,flex,elex,argv[4],prune_threshold, prune_count);
	
     aStar::set_max_SentenceTranslation(sentenceCount);
     return aStar::Suchalgorithmus(argv[5],&schwarz,&elex,&flex);
}