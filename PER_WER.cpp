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


double PER_global(SentencePool& sen){
	double ergebnis=0;

	#pragma omp parallel
	{
		#pragma omp for
		for(int i=0; i<sen.reference.size();i++){
			double tmp=PER(sen.reference[i], sen.guess[i]);
			#pragma omp critical
			ergebnis+=tmp;
			#pragma omp end critical
		}
	}
	return ergebnis;

}


double WER(vector<unsigned int>* r, vector <unsigned int>* g){
	Levenshtein lev(r,g);
	double leven = lev.distance();
	return (leven/r->size());

}
double WER_global(SentencePool& sen){
	double ergebnis=0;

	#pragma omp parallel
	{
		#pragma omp for
		for(int i=0; i<sen.reference.size();i++){
			double tmp=WER(sen.reference[i], sen.guess[i]);
			#pragma omp critical
			ergebnis+=tmp;
			#pragma omp end critical
		}
	}
	return ergebnis;

}
