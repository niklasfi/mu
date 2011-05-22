#include "sentencepool.h"
#include "levenshtein.h"
#include <vector>
#include <algorithm>
using namespace std;

double PER(vector<unsigned int>* r, vector <unsigned int>* g){
	double match=0;
	for(int i=0;i<(r->size());i++){
		for(int j=0;j<(g->size());j++){
			if(r[i]==g[i]){
				match+=1;
				break;
			}
		}
	}
	int groesse=(g->size())-(r->size());
	double ergebnis=1-(match-max(0,groesse))/(r->size());
	return ergebnis;
}
double PER_global(SentencePool sen){
	double ergebnis=0;
	for(int i=0; i<sen.reference.size();i++)
		ergebnis+=PER(sen.reference[i], sen.guess[i]);
	return ergebnis;		

}


double WER(vector<unsigned int>* r, vector <unsigned int>* g){
	Levenshtein lev(r,g);
	double leven = lev.distance();
	return (leven/r->size());

}
double WER_global(SentencePool sen){
	double ergebnis=0;
	for(int i=0; i<sen.reference.size();i++)
		ergebnis+=WER(sen.reference[i], sen.guess[i]);
	return ergebnis;		

}
