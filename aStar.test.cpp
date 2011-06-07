#include <iostream>
#include <assert.h>
#include <vector>

#include "aStar.h"
#include "HypothesisNode.h"
#include "PartialTranslation.h"
#include "lexicon.h"
#include "language.h"

bool test1(){
	std::vector<uint>w(6); //words
	Lexicon elex(english);
	
	aStar::elex = &elex;
	
	w[0] = elex.add("null");
	w[1] = elex.add("eins");
	w[2] = elex.add("zwei");
	w[3] = elex.add("drei");
	w[4] = elex.add("vier");
	w[5] = elex.add("fünf");
	
	std::vector<HypothesisNode> h(4);
		
	PartialTranslation 
		t1(3,std::vector<uint>{w[1]},&h[1]),
		t2(4,std::vector<uint>{w[2]},&h[2]),
		t3(2,std::vector<uint>{w[3]},&h[2]),
		t4(1,std::vector<uint>{w[4],w[5]},&h[3]);
		
	h[0].add_Translation(&t1);
	h[0].add_Translation(&t3);
	
	h[1].add_Translation(&t2);
	
	h[2].add_Translation(&t3);
		
	aStar a(h);
	
	a.search();
		
	return true;
}

int main(){
	assert(test1());
	
	assert(false);
}