#pragma once
#include <vector>
#include <string>

#include "lexicon.h"
#include "language.h"
#include "ptree.h"
#include "cost.h"

class Decoder{
	public:
		Lexicon* flex;
		Lexicon* elex;
		PTree<PTree<Cost>>* schwarz;
	
		Decoder(const char filename[], double prune_threshold, unsigned int prune_count);
		~Decoder();
	
	private:
		void readTable(const char filename[], double prune_threshold, unsigned int prune_count);
		
};