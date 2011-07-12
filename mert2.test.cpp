#include "mert2.h"

#include <iostream>

#include <assert.h>

std::deque<StraightLine*> mkdeq(){
	StraightLine* l1 = new StraightLine( 3,1,0,0);
	StraightLine* l2 = new StraightLine(-2,0,0,0);
	StraightLine* l3 = new StraightLine( 0,0,0,0);

	return std::deque<StraightLine*>{l1,l2,l3};
}

class MertTest: private Mert{
	MertTest(){}
public:
	static bool sort_test();
};

bool MertTest::sort_test(){
	MertTest mert;

	mert.lines = mkdeq();
	mert.sort();

	std::vector<double> answer{3,0,-2};

	for(unsigned int i = 0 ; i < mert.lines.size(); i++){
		//std::cout << mert.lines[i]->gradient << "\n";
		assert(mert.lines[i]->gradient == answer[i]);
	}

	return true;
}

int main(){
	assert(MertTest::sort_test());
	return 0;
}
