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
		t1(3,std::vector<uint>{w[1]},&h[1],&h[0]),
		t2(4,std::vector<uint>{w[2]},&h[2],&h[1]),
		t3(2,std::vector<uint>{w[3]},&h[2],&h[0]),
		t4(1,std::vector<uint>{w[4],w[5]},&h[3],&h[2]);

	h[0].add_PartialTranslation_to_Inbound(&t1);
	h[0].setBestcost(0);
	h[0].add_PartialTranslation_to_Inbound(&t3);



	h[1].add_PartialTranslation_to_Inbound(&t2);
	h[1].add_PartialTranslation_to_Outbound(&t1);
	h[1].setBestcost(3);

	h[2].add_PartialTranslation_to_Outbound(&t2);
	h[2].add_PartialTranslation_to_Outbound(&t3);
	h[2].add_PartialTranslation_to_Inbound(&t3);
	h[2].setBestcost(2);

	h[3].setBestcost(3);
	h[3].add_PartialTranslation_to_Outbound(&t4);


	aStar a(h);

	a.search();

	return true;
}

int main(){
	assert(test1());

	//assert(false);
}
