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
	
		Decoder(const char filename[], double prune_threshold, 
			unsigned int prune_count);
		~Decoder();
	
		typedef std::vector<SentenceInfo> nBestList;
		typedef std::vector<unsigned int> Sentence;
	
		struct hypRefPair{
			Sentence* reference;
			nBestList* nBest;  
			hypRefPair(Sentence*, nBestList*);
			~hypRefPair();
		};
		
		nBestList* translate(const Sentence&) const;
		nBestList* translate(const std::string& line) const;
		std::vector<nBestList>* translate(const char french[]) const;
		std::vector<hypRefPair>* translate(const char french[],const char ref[]) const;
		std::vector<nBestList>* translate(const std::vector<Sentence>& french) const;
		std::vector<hypRefPair>* translate(const std::vector<Sentence>& french, const std::vector<Sentence>& ref);
		/* die Funktionen können alle const sein, da an den flex, elex, schwarz
		 * Pointern nichts geändert wird */
		
		static std::vector<Sentence>* parseFile(Lexicon* lex, const char file[]);
	private:
		void readTable(const char filename[], double prune_threshold, 
			unsigned int prune_count) const;
		static Sentence* parseLine(Lexicon* lex, const std::string&);
};