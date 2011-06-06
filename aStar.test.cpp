#include <iostream>
#include <assert.h>
#include <vector>

#include "aStar.h"
#include "HypothesisNode.h"
#include "PartialTranslation.h"

bool test1(){
	std::vector<HypothesisNode> h(4);
		
	PartialTranslation 
		t1(3,std::vector<uint>{1},&h[1]),
		t2(4,std::vector<uint>{2},&h[2]),
		t3(2,std::vector<uint>{3},&h[2]),
		t4(1,std::vector<uint>{4,5},&h[3]);
		
	h[0].add_Translation(&t1);
	h[0].add_Translation(&t3);
	
	h[1].add_Translation(&t2);
	
	h[2].add_Translation(&t3);
		
	aStar a(h);
		
	a.search();
		
	return true;
}

int main(){
	assert(test1);
	
}