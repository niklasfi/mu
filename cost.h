#pragma once

#include <utility>
#include <vector>
#include <iostream>


class Cost{
	private:
	static std::vector<int> lookup; // falls das modell gesetzt ist, dann wird hier die stelle im vektor des Modells angegeben sonst -1 
	static unsigned int usedModelCount;
	
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
		unigram_language_model,
		bigram_language_model
	};
	static const unsigned int modelCount = 10; //gibt an, wie viele Modelle verfügt werden können
	
	static std::vector<double> scale; //speichert die skalierungsfaktoren für die einzelnen Modelle
	
	//============Attribute=========================
	std::vector<double> modelCosts; //speichert die werte der einzelnen Modelle
	
	//=============Konstruktor======================
	Cost(double val = 1.0/0.0){
		modelCosts = std::vector< double > (usedModelCount,val);
	}
	



	
	//============getter & setter=====================
	
	static void add_model(Model m,double scale = 1);
	static void select(std::vector<Model>); //select the models you want to use
	static void setScale(Model, double);
	static void setScale(std::vector< std::pair<Model,double> >);
	static double getScale(Model);
	static bool set(Model); //gibt zurück, ob das Model gesetzt wurde 
	
	
	
	std::vector<double> cost(std::vector<Model>); //get vector of costs for multiple models

	Cost& cost(Model, double); //alternative way of setting the costs for a model
	Cost& cost(std::vector<std::pair<Model,double>>); //set multiple costs

	
	//=============die Funktionen=========================

	double cost() const; //same as totalize
	double cost(Model); //get (or set, as this is an lvalue) cost for model - geht kaputt, wenn das Model nicht gesetzt ist



	double totalize() const; //calculates total costs given the current scaling
	
	double calc(//sets the penalties for all Models
		double rel_freq_f,
		double rel_freq_e,
		const std::vector<unsigned int>& f_phrase,
		const std::vector<unsigned int>& e_phrase,
		unsigned int f_singlecount,
		unsigned int e_singlecount,
		double source_to_target,
		double target_to_source,
		double unigram
	);
	
	//================berechnet die einzelnen Modelle========================
	Cost& calc_source_to_target_phrase(double rel_freq_f){
		return cost(source_to_target_phrase,rel_freq_f);
	}
	Cost& calc_target_to_source_phrase(double rel_freq_e){
		return cost(target_to_source_phrase,rel_freq_e);
	}
	Cost& calc_source_to_target_unigram(double source_to_target){
		return cost(source_to_target_unigram,source_to_target);
	}
	Cost& calc_target_to_source_unigram(double target_to_source){
		return cost(target_to_source_unigram,target_to_source);
	}
	Cost& calc_phrase_penalty(){
		return cost(phrase_penalty,1);
	}
	Cost& calc_word_penalty(const std::vector<unsigned int>& e_phrase){
		return cost(word_penalty,e_phrase.size());
	}
	Cost& calc_single_count_bit(unsigned int f_singlecount, unsigned int e_singlecount){
		return cost(single_count_bit, f_singlecount > 1 && e_singlecount > 1 ? 1 : 0);
	}
	Cost& calc_source_to_target_ratio(const std::vector<unsigned int>& f_phrase, const std::vector<unsigned int>& e_phrase){
		double quotient=((1.0*f_phrase.size())/e_phrase.size());
		return cost(source_to_target_ratio,(quotient));
	}
	Cost& calc_unigram_language_model(double unigram){
		return cost(unigram_language_model, unigram);
	}
	Cost& calc_bigram_language_model(double bigram){
		return cost(bigram_language_model, bigram);
	}
	
	//================Operatorenüberladung==================
	Cost& operator=(const Cost& c);
	
	Cost& operator+=(const Cost& c);
	Cost& operator+=(const std::pair<Model, double> p); //addiert die double auf die Modelkosten drauf
	Cost operator+ (const Cost& c) const;
	
	Cost& operator-=(const Cost& c);
	Cost operator- (const Cost& c) const;
	
	bool operator==(const Cost& c) const;
	bool operator!=(const Cost& c) const;
	
	bool operator<(const Cost& c) const;

};
