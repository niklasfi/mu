#include "sentencepool.h"
#include "levenshtein.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>



using namespace std;

double PER(vector<unsigned int>* r, vector <unsigned int>* g){
	double match=0;
	bool *found = new bool[r->size()];
	fill(found, found + (r->size()), false);
	for(int i=0;i<(g->size());i++){
		for(int j=0;j<(r->size());j++){
			if((*r)[j]==(*g)[i] && found[j]!=true){
				match++;
				found[j]=true;
				j=(r->size());
			}
		}
	}
	int groesse=(g->size())-(r->size());
	double ergebnis=1-(match-max(0,groesse))/(r->size());
	delete[] found;
	return ergebnis;
}
double PER2(vector<unsigned int>* r, vector <unsigned int>* g){
	double match=0;
	bool *found = new bool[r->size()];
	fill(found, found + (r->size()), false);
	for(int i=0;i<(g->size());i++){
		for(int j=0;j<(r->size());j++){
			if((*r)[j]==(*g)[i] && found[j]!=true){
				match++;
				found[j]=true;
				j=(r->size());
			}
		}
	}
	int groesse=(g->size())-(r->size());
	double ergebnis=(match-max(0,groesse));
	delete[] found;
	return ergebnis;
}


double PER_global(SentencePool& sen){
	double ergebnis=0;
	double zaehler =0;
	double nenner =0;

//	#pragma omp parallel
//	{
//		#pragma omp for
		for(unsigned int i=0; i<sen.reference.size();i++){
			double tmp=PER2(sen.reference[i], sen.guess[i]);
//			#pragma omp critical
			zaehler+=tmp;
			nenner+=(sen.reference[i]->size());
//			#pragma omp end critical
		}
//	}
	ergebnis= 1 - zaehler/nenner;
	return ergebnis;

}


double WER(vector<unsigned int>* r, vector <unsigned int>* g){
	Levenshtein lev(r,g);
	double leven = lev.distance();
	return (leven/r->size());

}
double WER2(vector<unsigned int>* r, vector <unsigned int>* g){
	Levenshtein lev(r,g);
	double leven = lev.distance();
	return leven;

}
double WER_global(SentencePool& sen){
	double ergebnis=0;
	double zaehler =0;
	double nenner =0;

//	#pragma omp parallel
//	{
//		#pragma omp for
		for(unsigned int i=0; i<sen.reference.size();i++){
			double tmp=WER2(sen.reference[i], sen.guess[i]);
//			#pragma omp critical
			zaehler+=tmp;
			nenner+=(sen.reference[i]->size());
//			#pragma omp end critical
		}
//	}
	ergebnis=(zaehler/nenner);
	return ergebnis;

}
