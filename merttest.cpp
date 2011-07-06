#include "mert.cpp"
#include <vector>
#include "global.h"
#include "sentenceinfo.h"
#include "cost.h"

int main () {

	std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > nBestList;

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

	return 0;
}
