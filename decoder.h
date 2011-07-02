#pragma once
#include <vector>
#include <string>

#include "lexicon.h"
#include "language.h"
#include "ptree.h"
#include "cost.h"
#include "sentenceinfo.h"



class Decoder{
	public:
		Lexicon* flex;
		Lexicon* elex;
		PTree<PTree<Cost>>* schwarz;
		/*filename soll der Pfad zur Phrasentabelle sein,
		 threshold gibt an, wie groß der relative Unterschied von der besten Übersetzung sein darf (geeignete Zahlen zwischen 2 und 5)
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
			hypRefPair(Sentence*, nBestList*);
			~hypRefPair();
		};
		//übersetzt einen Satz von ID's
		nBestList* translate(const Sentence&);
		
		//übersetzt einen String und gibt die Übersetzung in ID's aus
		nBestList* translate(const std::string& line);
		
		/*Eingabe ist der Pfad vom zu übersetzenden Text
		gibt die n besten Übersetzungen aus, n wird mit maxSentenceTranslation aus aStar gesetzt*/
		std::vector<nBestList>* translate(const char french[]);
		//gibt die nBestListe für einen ganzen Text zurück und enthält auch für jeden Satz eine Referenzübersetzung
		std::vector<hypRefPair>* translate(const char french[],const char ref[]);
		
		static std::vector<Sentence>* parseFile(Lexicon* lex, const char file[]);
	private:
		//liest die Phrasentabelle ein
		void readTable(const char filename[], double prune_threshold, 
			unsigned int prune_count);
		//parseLine macht aus einem String einen Vektor von ID's
		static Sentence* parseLine(Lexicon* lex, const std::string&);
};
