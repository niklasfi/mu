#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "lexicon.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "ptree.h"
#include "aStar.h"
#include "cost.h"

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
