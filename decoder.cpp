#include "decoder.h"

Decoder::Decoder(const char filename[], double prune_threshold, unsigned int prune_count):
schwarz(new PTree<PTree<Cost>>){
	readTable(filename, prune_threshold, prune_count);
	flex=new Vocab(0,-1);
//  	elex=new Vocab();
}
Decoder::~Decoder(){
	delete flex;    flex    = 0;
	delete elex;    elex    = 0;
	delete schwarz; schwarz = 0;
}

Decoder::hypRefPair::hypRefPair(Sentence* ref,
	nBestList* nBest):
	reference(ref),nBest(nBest){}

Decoder::hypRefPair::~hypRefPair(){
	//delete nBest; nBest = 0;
}
	
Decoder::nBestList* Decoder::translate(Decoder::Sentence& sent){
	return aStar::Suchalgorithmus(sent, this);
}
Decoder::hypRefPair* Decoder::translate(Decoder::Sentence& french,
	Decoder::Sentence& ref)
{
	return new hypRefPair(&ref,translate(french));
}

std::vector<Decoder::nBestList>* Decoder::translate(std::vector<Decoder::Sentence>& french){
	std::vector<nBestList>* result = new std::vector<nBestList>();
	for(unsigned int i = 0; i < french.size(); i++){
		nBestList* translation = translate(french[i]);
		result->push_back(*translation);
		delete translation;
	}
	return result;
}

std::vector<Decoder::hypRefPair>* Decoder::translate(std::vector<Decoder::Sentence>& french,
 std::vector<Decoder::Sentence>& ref)
{
	std::vector<hypRefPair>* result = new std::vector<hypRefPair>();
	for(unsigned int i = 0; i < french.size(); i++){
		hypRefPair* translation = new hypRefPair(&ref[i],translate(french[i])); //translate(french[i], ref[i]);
		result->push_back(*translation);
		//delete translation;
	}
	return result;
}
void Decoder::readTable(const char filename[], double prune_threshold,	unsigned int prune_count){
//==================Einlesen der Phrasentabelle============================
	PTree< pair <unsigned int, double> > pruningTree; //speichert für jede Übersetzung die Anzahl der eingelesenen Übersetzungen und die beste Übersetzung
	pair <unsigned int, double> pruningStart; //die Startkombi für den PruningTree
	pruningStart.first=0;
	pruningStart.second=(1./0.);

	igzstream in(filename);
	std::string line, token;

	while(getline(in,line)){
		std::stringstream ist(line);

		double relfreq_f, relfreq_e, source_to_target, target_to_source, unigram_sprachmodell;
		unsigned int singlecf, singlece; 
		vector<uint> ephrase, fphrase;

		//Ausgabe: relfreq_f relfreq_e # quellphrase # zielphrase # singlecf singlece # source_to_target target_to_source #  unigram-sprachmodell
		ist >> relfreq_f  >> relfreq_e >>token; // token für "#"

		const char* token_char=token.c_str();
		
		while(ist>>token && token != "#"){
			fphrase.push_back(flex->addWord(token_char));
		}
		while(ist>>token && token != "#"){
			ephrase.push_back(elex->addWord(token_char));
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

Decoder::Sentence* Decoder::parseLine(Vocab* lex, const std::string& line){
	istringstream ist(line);
	std::string token;
	Sentence* sent=new Sentence;
	
	while ( ist >> token){
		sent->push_back(lex->addWord(token.c_str()));
	}
	return sent;
}

std::vector<Decoder::Sentence>* Decoder::parseFile(Vocab* lex, const char file[]){
	igzstream igz(file);
	std::string line;
	
	std::vector<Sentence>* result = new std::vector<Sentence>();
	
	while(getline(igz,line)){
		Sentence* parsed_line = parseLine(lex,line);
		result->push_back(*parsed_line);
		delete parsed_line;
	}
	
	return result;
}
