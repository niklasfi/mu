#include <assert.h>
#include <vector>
#include "levenshtein.h"

int main () {
	
	int i;
	std::vector<unsigned int> ref (5);	//Referenzsatz
	std::vector<unsigned int> gss (5);	//Hypothese
	std::vector<unsigned int> *r;
	std::vector<unsigned int> *g;
	r = &ref;
	g = &gss;

	//Testsatz 1 - komplette Übereinstimmung
	for(i=0;i<5;i++) {
		ref[i] = 1;
		gss[i] = 1;
	}
	Levenshtein l1(r,g);
	assert(l1.distance() == 0);

	//Testsatz 2 - keinerlei Übereinstimmung
	for(i=0;i<5;i++) {
		ref[i] = i+5;
		gss[i] = i;
	}
	Levenshtein l2(r,g);
	assert(l2.distance() == 5);
	
	//Testsatz 3 - Satzlänge 0
	ref.resize(0);
	gss.resize(0);
	Levenshtein l3(r,g);
	assert(l3.distance() == 0);

	//Testsatz 4 - Referenz kleiner als Hypothese
	ref.resize(1);
	gss.resize(3);
	for(i=0;i<3;i++)
		gss[i] = i;
	ref[0] = 0;
	Levenshtein l4(r,g);
	assert(l4.distance() == 2);

	//Testsatz 5 - Hypothese kleiner als Referenz
	ref.resize(3);
	gss.resize(1);
	for(i=0;i<3;i++)
		ref[i] = i;
	gss[0] = 0;
	Levenshtein l5(r,g);
	assert(l5.distance() == 2);

	Lexicon elex;
	elex.add("A");
	elex.add("N");
	elex.add("S");
	elex.add("B");
	elex.add("E");
	Lexicon* elex_ptr=&elex;
	
	//Testsatz 6 - ANANAS/BANANE
	ref.resize(6);
	gss.resize(6);
	ref[0]=0; ref[1]=1; ref[2]=0; ref[3]=1; ref[4]=0; ref[5]=2;
	gss[0]=3; gss[1]=0; gss[2]=1; gss[3]=0; gss[4]=1; gss[5]=4;
	Levenshtein l6(r,g);
	assert(l6.distance() == 3);
	
	l6.print(elex_ptr);


	return 0;
}
