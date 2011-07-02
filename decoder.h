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
		
		nBestList* translate(const Sentence&);
		nBestList* translate(const std::string& line);
		std::vector<nBestList>* translate(const char french[]);
		std::vector<hypRefPair>* translate(const char french[],const char ref[]);
		
		static std::vector<Sentence>* parseFile(Lexicon* lex, const char file[]);
	private:
		void readTable(const char filename[], double prune_threshold, 
			unsigned int prune_count);
		static Sentence* parseLine(Lexicon* lex, const std::string&);
};