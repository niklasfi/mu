#include "mert.cpp"
#include <vector>
#include <list>
#include "global.h"
#include "sentenceinfo.h"
#include "cost.h"
#include <assert.h>

using namespace std;


bool test_mert(){
	vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > nBestList;

	std::pair<std::vector<uint>, std::vector<SentenceInfo> > pair_tmp;
	std::vector<SentenceInfo> hypothesis_tmp;
	SentenceInfo sinfo_tmp;
	Cost cost_tmp;
	
	//Referenz anlegen
	pair_tmp.first.push_back(1);
	pair_tmp.first.push_back(2);
	pair_tmp.first.push_back(3);
	pair_tmp.first.push_back(4);

	//1. Hypothese anlegen
	sinfo_tmp.sentence.push_back(0);
	sinfo_tmp.sentence.push_back(2);
	sinfo_tmp.sentence.push_back(3);
	sinfo_tmp.sentence.push_back(4);
	cost_tmp.modelCosts = std::vector<double> (2, 1);
	cost_tmp.scale = std::vector<double>(2, 0.1);
	sinfo_tmp.cost = cost_tmp;
	pair_tmp.second.push_back(sinfo_tmp);

	//2. Hypothese anlegen
	sinfo_tmp.sentence.push_back(0);
	sinfo_tmp.sentence.push_back(2);
	sinfo_tmp.sentence.push_back(3);
	sinfo_tmp.sentence.push_back(5);
	sinfo_tmp.sentence.push_back(6);
	cost_tmp.modelCosts[0] = 3;
	sinfo_tmp.cost = cost_tmp;
	pair_tmp.second.push_back(sinfo_tmp);

	//3. Hypothese anlegen
	sinfo_tmp.sentence.push_back(1);
	sinfo_tmp.sentence.push_back(2);
	sinfo_tmp.sentence.push_back(4);
	sinfo_tmp.sentence.push_back(4);
	cost_tmp.modelCosts[0] = 4;
	cost_tmp.modelCosts[1] = 3;
	sinfo_tmp.cost = cost_tmp;
	pair_tmp.second.push_back(sinfo_tmp);

	nBestList.push_back(pair_tmp);

	mert(nBestList,2); //Mert ausführen
	return true;
}

bool test_RandomPermutation(){
	std::list<int> Zahlen=randomPermutation(10);
	
	//überprüfe, ob jede Zahl da ist
	for (int i=0; i<10; i++){
		bool found_i=false;
		for (std::list<int>::iterator it=Zahlen.begin() ; it != Zahlen.end(); it++ )
			found_i= ((*it) == i) || found_i;
		assert(found_i);
	}
	assert(Zahlen.size() == 10);
	return true;
}

bool test_selectedModel(){
	vector<Cost::Model> models = selectModels(3);
	assert(models[0] == Cost::source_to_target_phrase);
	assert(models[2] == Cost::source_to_target_unigram);
	return true;
}

bool test_addNewHypothesis(){
	return true;
}

int main () {
	assert(test_mert());
	assert(test_RandomPermutation());
	assert(test_selectedModel());
	return 0;
}
