#include "decoder.h"
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include "lexicon.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "ptree.h"
#include "aStar.h"
#include "cost.h"

Decoder::Decoder(const char filename[], double prune_threshold, unsigned int prune_count):
	flex(new Lexicon(french)), elex(new Lexicon(english)), schwarz(new PTree<PTree<Cost>>){
	readTable(filename, prune_threshold, prune_count);
}
Decoder::~Decoder(){
	delete flex;    flex    = 0;
	delete elex;    elex    = 0;
	delete schwarz; schwarz = 0;
}

Decoder::hypRefPair::hypRefPair(std::vector<unsigned int>* ref, std::vector<SentenceInfo>* nBest):
	reference(ref),nBest(nBest){}

Decoder::hypRefPair::~hypRefPair(){
	delete reference; reference = 0;
	delete nBest;     nBest     = 0;
}

Decoder::nBestList* Decoder::translate(const std::string& line){
	return translate(*parseLine(flex, line));
}

std::vector<Decoder::nBestList>* Decoder::translate(const char french[]){
	igzstream figz(french);
	std::string fline;
	
	std::vector<Decoder::nBestList>* translation = new std::vector<Decoder::nBestList>();
	
	while(getline(figz,fline))
		translation->push_back(*translate(fline));

	return translation;
}

std::vector<Decoder::hypRefPair>* Decoder::translate(const char french[],const char ref[]){
	igzstream figz(french), rigz(ref);
	std::string fline, rline;
	
	std::vector<Decoder::hypRefPair>* translation = new std::vector<Decoder::hypRefPair>();
	
	while(getline(figz, fline) && getline(rigz, rline))
		translation->push_back(
			Decoder::hypRefPair(
				parseLine(elex, rline),
				new std::vector<SentenceInfo>()
				//translate(parseLine(flex,fline))
			)	
		);

	return translation;
}

void Decoder::readTable(const char filename[], double prune_threshold,	unsigned int prune_count){
//==================Einlesen der Phrasentabelle============================
	PTree< pair <unsigned int, double> > pruningTree; //speichert für jede Übersetzung die Anzahl der eingelesenen Übersetzungen und die beste Übersetzung
	pair <unsigned int, double> pruningStart; //die Startkombi für den PruningTree
	pruningStart.first=0;
	pruningStart.second=(1./0.);

	igzstream in(filename);
	std::string line,token;


	while(getline(in,line)){
		std::stringstream ist(line);

		double relfreq_f, relfreq_e, source_to_target, target_to_source, unigram_sprachmodell;
		unsigned int singlecf, singlece; 
		vector<uint> ephrase, fphrase;

		//Ausgabe: relfreq_f relfreq_e # quellphrase # zielphrase # singlecf singlece # source_to_target target_to_source #  unigram-sprachmodell
		ist >> relfreq_f  >> relfreq_e >>token; // token für "#"

		while(ist>>token && token != "#"){
			fphrase.push_back(flex->getWord_or_add(token));
		}
		while(ist>>token && token != "#"){
			ephrase.push_back(elex->getWord_or_add(token));
		}
		ist >> singlecf >> singlece >> token >> source_to_target >> target_to_source >> token >> unigram_sprachmodell;

		Cost kosten=Cost();
		kosten.calc(relfreq_f, relfreq_e, fphrase, ephrase, singlecf, singlece, source_to_target, target_to_source, unigram_sprachmodell);
		double kosten_insgesamt=kosten.cost();

		pair< unsigned int, double>* pruning_infos=&pruningTree.traverse(fphrase,true,pruningStart)->c;

		if (kosten_insgesamt > pruning_infos->second+prune_threshold || pruning_infos->first >prune_count)	continue; //pruning ergibt, wir wollen es nicht in den Ptree mitaufnehmen

		//if (kosten_insgesamt< pruning_infos->second)	pruning_infos->second=kosten_insgesamt;  _jetzt irrelevant, da ich von einer geordneten eingabe ausgehe
		pruning_infos->first++;

		schwarz->traverse(fphrase,true)->c.traverse(ephrase,true,Cost(1./0.))->c = kosten;

	}
	//cerr << " schwarz erstellt" << endl;

}

Decoder::Sentence* Decoder::parseLine(Lexicon* lex, const std::string& line){
	istringstream ist(line);
	std::string token;
	Sentence* sent=new Sentence;
	
	while ( ist >> token){
		sent->push_back(lex->getWord_or_add(token).wordId());
	}
	return sent;
}