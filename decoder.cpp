class Decoder;
#include "decoder.h"

Decoder::Decoder(const char filename[], double prune_threshold, unsigned int prune_count, const char * ngramfilename):
schwarz(new PTree<PTree<Cost>>){
	flex=new Vocab();
	elex=new Vocab();
	
	readTable(filename, prune_threshold, prune_count);

	
	if (Cost::set(Cost::bigram_language_model) && ngramfilename != "nix"){//Model wurde gesetzt
		ngram= new Ngram(*elex, 2);
		cout << "vor der file" << endl;
		File file(fopen(ngramfilename, "r"));
		cout << "nach der file" <<endl;
		ngram->read(file);
	}
}
Decoder::~Decoder(){
	delete flex;    flex    = 0;
	delete elex;    elex    = 0;
	delete schwarz; schwarz = 0;
	if (ngram)	delete ngram; ngram=0;
}

typedef VocabIndex* VocabBuffer;
	
void Decoder::add_bigram(nBestList* nbestlist){
	for (unsigned int i=0; i< nbestlist->size(); i++){
		
		//zuerst in einen Buffer umschreiben
		VocabBuffer buf (new VocabIndex[(*nbestlist)[i].sentence.size()+3]); //vocabbuffer der richtigen länge initialisieren (+3 für satzanfang und satzende)
		buf[0]=elex->seIndex(); //satzanfangsmarkierer
		unsigned int buf_index=1;
		for (int j=(*nbestlist)[i].sentence.size()-1; j >= 0; j--)
			buf[buf_index++]=(*nbestlist)[i].sentence[j];
		buf[buf_index++]=elex->ssIndex(); //satzendemarkierer
		buf[buf_index++]=Vocab_None;
		
		//jetzt Kosten ausrechnen
		for (unsigned int pos=0;  pos<= (*nbestlist)[i].sentence.size(); pos++){
			pair<Cost::Model, double> tmp;
			tmp.first=Cost::bigram_language_model;
			tmp.second=ngram->wordProb(buf[pos], &buf[pos+1]);
			(*nbestlist)[i].cost+=(tmp);
		}
	}
}

nBestList* Decoder::translate(Sentence& sent){
	double bigram_scale=(double)Cost::set(Cost::bigram_language_model);
	
	if (bigram_scale){
		bigram_scale = Cost::getScale(Cost::bigram_language_model);
		Cost::setScale(Cost::bigram_language_model,0); //wir setzen im Scale Vektor auf 0 damit es in der aStar-Search unbeachtet bleibt
	}
	nBestList* result = aStar::Suchalgorithmus(sent,this);
	if (bigram_scale)	add_bigram(result);
	return result;
}

hypRefPair* Decoder::translate(Sentence& french, Sentence& ref)
{
	return new hypRefPair(&ref,translate(french));
}

std::vector<nBestList>* Decoder::translate(std::vector<Sentence>& french){
	std::vector<nBestList>* result = new std::vector<nBestList>();
	for(unsigned int i = 0; i < french.size(); i++){
		nBestList* translation = translate(french[i]);
		result->push_back(*translation);
		delete translation;
	}
	return result;
}

std::vector<hypRefPair>* Decoder::translate(std::vector<Sentence>& french,
 std::vector<Sentence>& ref)
{
	std::vector<hypRefPair>* result = new std::vector<hypRefPair>();
	for(unsigned int i = 0; i < french.size(); i++){
		hypRefPair* translation = translate(french[i], ref[i]);
		result->push_back(*translation);
		delete translation;
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
			token_char=token.c_str();
			fphrase.push_back(flex->addWord(token_char));
		}
		while(ist>>token && token != "#"){
			token_char=token.c_str();
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

Sentence* Decoder::parseLine(Vocab* lex, const std::string& line){
	istringstream ist(line);
	std::string token;
	Sentence* sent=new Sentence;
	
	while ( ist >> token){
		sent->push_back(lex->addWord(token.c_str()));
	}
	return sent;
}

std::vector<Sentence>* Decoder::parseFile(Vocab* lex, const char file[]){
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
