#pragma once
#include <vector>
#include <string>

#include <Vocab.h>
#include "language.h"
#include "ptree.h"
#include "cost.h"
#include "sentenceinfo.h"
#include "aStar.h"
#include "gzstream/gzstream.h"

/* usage: übersetzen einer Textdatei:
	Decoder decoder(phraseextract, prune_threshold, prune_count);

	std::vector<Decoder::Sentence>* f = Decoder::parseFile(decoder.flex,frenchfile);
	std::vector<Decoder::Sentence>* e = Decoder::parseFile(decoder.elex,engfile);
	//kann weggelassen werden, für referenz
	
	std::vector<Decoder::hypRefPair>* translation = decoder.translate(*f,*e);
	//für nur übersetzen:
	std::vector<Decoder::nBestliste>* translation = decoder.translate(*f);
	
	//...
	
	delete f;
	delete e;
	delete translation;
	
	//.. nächste übersetzung
*/

class Decoder{
	public:
		Vocab* flex;
		Vocab* elex;
		PTree<PTree<Cost>>* schwarz;
		/*filename soll der Pfad zur Phrasentabelle sein,
		 threshold gibt an, wie groß der relative Unterschied von der besten 
		 Übersetzung sein darf (geeignete Zahlen zwischen 2 und 5)
		 prune_count gibt an, wie viele Übersetzungen absolut zugelassen werden 
		*/
		Decoder(const char filename[], double prune_threshold, unsigned int prune_count);
		~Decoder();
	
		typedef std::vector<SentenceInfo> nBestList;
		typedef std::vector<unsigned int> Sentence;
	
		//struct um eine "nBestListe"+Referenzübersetzung für einen ganzen Text zu speichern
		struct hypRefPair{
			Sentence* reference;
			nBestList* nBest;  
			hypRefPair(Sentence* ref, nBestList* hyp);
			~hypRefPair();
		};
		//übersetzt einen Satz von ID's
		nBestList* translate(Sentence&);
		hypRefPair* translate(Sentence& french, Sentence& ref);
		
		std::vector<nBestList>* translate(std::vector<Sentence>&);
		std::vector<hypRefPair>* translate(std::vector<Sentence>& french,
			std::vector<Sentence>& ref);
		
		/* parseFile liest die textdatei `file` ein und gibt einen 
		 * vector<vector<uint>> mit den jeweiligen ids zurück */
		static std::vector<Sentence>* parseFile(Vocab* lex, const char file[]);
		//parseLine macht aus einem String einen Vektor von ID's
		static Sentence* parseLine(Vocab* lex, const std::string&);
	private:
		//liest die Phrasentabelle ein
		void readTable(const char filename[], double prune_threshold, 
			unsigned int prune_count);
};


