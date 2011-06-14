#include <cost.h>
#include <assert.h>

std::vector<double> Cost::scale;
std::vector<int> Cost::lookup;

typedef unsigned int uint;

void Cost::select(std::vector<Model> ms){
	if(!lookup.empty()) assert(false); //man darf sich nur einmal entscheiden, welche Modelle man will!
	
	lookup.resize(modelCount,-1);
	
	uint nextPos = 0;
	
	for(uint i = 0; i < ms.size(); i++){
		if(lookup[i] == -1)
			lookup[i] = nextPos++;
	}
	
	scale = std::vector<double>(modelCount,1);
}

void Cost::setScale(Model m, double v){
	scale[m] = v;
}

void Cost::setScale(std::vector <std::pair<Model, double>> pairVec){
	for(uint i = 0; i < pairVec.size(); i++)
		setScale(pairVec[i].first, pairVec[i].second);
}

double Cost::cost() const {
	return totalize();
}

double Cost::cost(Model m){
	if (lookup[m] == -1)	return 0;
	return modelCosts[lookup[m]] * scale[m];
}

std::vector<double> Cost::cost(std::vector<Model> ms){
	std::vector<double> res;
	for(uint i = 0; i < ms.size(); i++)
		res.push_back(cost(ms[i]));
}

Cost& Cost::cost(Model m, double v){
	if(lookup[m]!=-1)
		modelCosts[lookup[m]]=v;
	return *this;
}

Cost& Cost::cost(std::vector<std::pair<Model,double>> mp){
	for(uint i = 0; i < mp.size(); i++)
		cost(mp[i].first, mp[i].second);
	return *this;
}

double Cost::totalize() const {
	double total = 0;
	for(uint i = 0; i < lookup.size(); i++)
		if (lookup[i] != -1)
			total += modelCosts[lookup[i]]; //falls model nicht existiert rechnet er + 0
	return total;
}

double Cost::calc(
	double rel_freq_f,
	double rel_freq_e,
	const std::vector<unsigned int>& f_phrase,
	const std::vector<unsigned int>& e_phrase,
	unsigned int f_singlecount,
	unsigned int e_singlecount,
	double source_to_target,
	double target_to_source,
	double unigram
){
	return 
	calc_source_to_target_phrase(rel_freq_f).
	calc_target_to_source_phrase(rel_freq_e).
	calc_source_to_target_unigram(source_to_target).
	calc_target_to_source_unigram(target_to_source).
	calc_phrase_penalty().
	calc_word_penalty(e_phrase).
	calc_single_count_bit(f_singlecount, e_singlecount).
	calc_source_to_target_ratio(f_phrase, e_phrase).
	calc_unigram_language_model(unigram).totalize();
}

Cost& Cost::operator=(const Cost& c){
	modelCosts=c.modelCosts;
	return *this;
}

Cost& Cost::operator+=(const Cost& c){
	for (uint i=0; i< lookup.size(); i++)
		if (lookup[i] != -1)
			modelCosts[lookup[i]]+=c.modelCosts[lookup[i]];
	return *this;
}

Cost Cost::operator+(const Cost& c){
	Cost z=*this;
	z+=c;
	return z;
}

Cost& Cost::operator-=(const Cost& c){
	for (uint i=0; i<lookup.size(); i++)
		if (lookup[i] != -1)
			modelCosts[lookup[i]]-=c.modelCosts[lookup[i]];
	return *this;
}

Cost Cost::operator-(const Cost& c){
	Cost z=*this;
	z-=c;
	return z;
}
