#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>

#include "cost.h"
#include "sentenceinfo.h"
#include "bleu.h"
#include "decoder.h"

/* === Strukturtyp StraightLine, um Geraden darzustellen === */
struct StraightLine {
	double gradient;	//Steigung
	double offset;		//Achsen-Abstand
	unsigned int sentence_pos;		//welcher Satz übersetzt wurde
	unsigned int sentence_best;		//welche Hypothese dazugehört
	double x; 
	
	StraightLine(double gradient, double offset, unsigned int sentence_pos, unsigned int sentence_best): gradient(gradient), offset(offset), sentence_pos(sentence_pos), sentence_best(sentence_best){}
};

class Mert{
	
private:
	std::deque<StraightLine*> lines; //alle Geraden und später alle Schnittpunkte
	
	unsigned int	aktParam; //Parameter, der gerade optimiert wird
	BleuInfo	bleu; //speichert Nenner etc. der Grams

	
	std::vector<hypRefPair>* translation;
	
	//Funktionen
	
	void init_lines_single(hypRefPair& einSatz, unsigned int sentence_nr);
	void init_lines();
	void sort();
	double bleu_optimize();
	void findSections ();
	
	Mert():translation(0){}
	friend class MertTest;
public:
	Mert(Decoder decoder, std::vector<Sentence>* f, std::vector<Sentence>* e){
		translation = decoder.translate(*f,*e);		//und beste Übersetzungen suchen;
	}
	~Mert(){
		delete translation;
		for (unsigned int i=0; i< lines.size(); i++)	delete lines[i];
	}
	std::vector<double> optimize();
	
};
