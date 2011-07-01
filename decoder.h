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
	
		Decoder(const char filename[], double prune_threshold, unsigned int prune_count);
		~Decoder();
	
		struct hypRefPair{
			std::vector<unsigned int> reference;
			std::vector<SentenceInfo> nBest;
		};
		
		hypRefPair* translate(std::string line);
		std::vector<hypRefPair>* translate(const char filename[]);
	private:
		void readTable(const char filename[], double prune_threshold, unsigned int prune_count);
		
};