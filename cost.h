#pragma once

#include <utility>
#include <vector>

class Cost{
	public:
	
	enum Model{ 
		source_to_target_phrase,
		target_to_source_phrase,
		source_to_target_unigram,
		target_to_source_unigram,
		phrase_penalty,
		word_penalty,
		single_count_bit,
		source_to_target_ratio,
		unigram_language_model
	}
	static const unsigned int modelCount = 9; //gibt an, wie viele Modelle verfügt werden können
	
	static void select(std::vector<Model>); //select the models you want to use

	static std::vector<double> scale; //speichert die skalierungsfaktoren für die einzelnen Modelle
	//setter
	static void setScale(Model, double);
	static void setScale(std::vector< std::pair<Model,double> >);

	std::vector<double> modelCosts; //speichert die werte der einzelnen Modelle
	double cost(); //same as totalize
	private: double& cost(Model); //get (or set, as this is an lvalue) cost for model - geht kaputt, wenn das Model nicht gesetzt ist
	public: std::vector<double> cost(std::vector<Model>); //get vector of costs for multiple models
	Cost& cost(Model, double); //alternative way of setting the costs for a model
	Cost& cost(std::vector<std::pair<Model,double>>); //set multiple costs

	double totalize(); //calculates total costs given the current scaling
	
	double calc(
		double rel_freq_f,
		double rel_freq_e,
		const std::vector<unsigned int>& f_phrase,
		const std::vector<unsigned int>& e_phrase,
		unsigned int f_singlecount,
		unsigned int e_singlecount,
		double source_to_target,
		double target_to_source
		double unigram
	) //sets the penalties for all Models
	
	Cost& calc_source_to_target_phrase(double rel_freq_f){
		return cost(source_to_target_phrase,rel_freq_f);
	}
	Cost& calc_target_to_source_phrase(double rel_freq_e){
		return cost(target_to_source_phrase,rel_freq_e);
	}
	Cost& calc_source_to_target_unigram(double source_to_target){
		return cost(source_to_target_word,source_to_target);
	}
	Cost& calc_target_to_source_unigram(double target_to_source){
		return cost(target_to_source_word,target_to_source);
	}
	calc_phrase_penalty(){
		return cost(phrase_penalty,1);
	}
	calc_word_penalty(const std::vector<unsigned int>& e_phrase){
		return cost(word_penalty,e_phrase.size());
	}
	calc_single_count_bit(unsigned int f_singlecount, unsigned int e_singlecount){
		return cost(single_count_bit, f_singlecount > 1 && e_singlecount > 1 ? 1 : 0)
	}
	calc_source_to_target_ratio{const std::vector<unsigned int>& f_phrase, const std::vector<unsigned int>& e_phrase){
		return cost(source_to_target_ratio,(1.0*f_phrase.size())/e_phrase.size())
	}
	calc_unigram_language_model(double unigram){
		return cost(unigram_language_model, unigram);
	}

	private:
	
	static vector<int> lookup; // falls das modell gesetzt ist, dann wird hier die stelle im vektor des Modells angegeben sonst -1 
}
