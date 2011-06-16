#include "cost.h"
#include <assert.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

bool test1(){
	//wir wollen nur source_to_target_phrase betrachten
	Cost::add_model(Cost::target_to_source_phrase);
		
	Cost kosten=Cost();
	assert(kosten.cost() == (1./0.));
	double relfreq_f = 0.5;
	double relfreq_e=0.7;
	vector<unsigned int> phrase;
	
	kosten.calc(relfreq_f, 1, phrase, phrase, 2,3,0.2,0.3,1.1);
	assert(fabs(kosten.cost() - 0.7) < 0.0000000001);
	return true;
}

bool test2(){
	vector<Cost::Model> vec_cost;
	//wir wollen nur source_to_target_phrase betrachten
	Cost::Model model1=Cost::source_to_target_phrase;
	Cost::Model model2=Cost::source_to_target_ratio;
	Cost::Model model3=Cost::target_to_source_phrase;
	
	/*vec_cost.push_back(model2);
	vec_cost.push_back(model1);
	vec_cost.push_back(model3);
	Cost::select(vec_cost);*/
	Cost::add_model(Cost::source_to_target_phrase);
	Cost::add_model(Cost::source_to_target_ratio);
	Cost::add_model(Cost::target_to_source_phrase);
	
	Cost kosten=Cost();
	assert(kosten.cost() == (1./0.));
	double relfreq_f = 0.5;
	double relfreq_e=0.7;
	vector<unsigned int> phrase(5);
	//source_to_target_phrase+source_to_target_ratio+target_to_source_phrase = 0.5 + 1 + 0.7
	kosten.calc(relfreq_f, relfreq_e, phrase, phrase, 2,3,0.2,0.3,1.1);
	for (int i =0; i<kosten.modelCosts.size(); i++)

	assert(fabs(kosten.cost() - 2.2) < 0.0000000001);
	Cost kosten2=kosten;
	assert(fabs(kosten.cost() - 2.2) < 0.000000001);
	return true;
}



int main(){
	//assert(test1());
	assert(test2());
}