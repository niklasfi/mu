#include "dictionary.h"

int main(){
	Dictionary d;
	d.add("hallo","hi");
	d.add("hallo","howdy");
	d.add("hallo","g'day");
	d.add("hallo","good");
	d.add("hallo","evening");
	d.add("willkommen","hi");
	d.add("hallo","hi");
	d.add("gruzi","hi");

	Word hallo = d.flex.getWord("hallo");
	Word willkommen = d.flex.getWord("willkommen");
	Word gruzi = d.flex.getWord("gruzi");

	Word hi = d.elex.getWord("hi");
	Word howdy = d.elex.getWord("howdy");
	Word gday = d.elex.getWord("g'day");
	Word good = d.elex.getWord("good");
	Word evening = d.elex.getWord("evening");

	//f->e
	assert(d.fwa[hallo].singlecount == 6);

	assert(fabs(d.fwa[hallo].relFreq(hi) - 1.0986122886681096914) < 1e-15); //2/6
	assert(fabs(d.fwa[hallo].relFreq(howdy) - 1.7917594692280550008) < 1e-15); //1/6
	assert(fabs(d.fwa[hallo].relFreq(good) - 1.7917594692280550008) < 1e-15);
	assert(fabs(d.fwa[hallo].relFreq(evening) - 1.7917594692280550008) < 1e-15);
	assert(fabs(d.fwa[hallo].relFreq(gday) - 1.7917594692280550008) < 1e-15);

	assert(d.fwa[willkommen].singlecount == 1);
	assert(d.fwa[willkommen].relFreq(hi) == 0);

	assert(d.fwa[gruzi].singlecount == 1);
	assert(d.fwa[gruzi].relFreq(hi) == 0);

	//e->f
	assert(d.ewa[hi].singlecount == 4);
	assert(fabs(d.ewa[hi].relFreq(hallo) - 0.69314718055994530942) < 1e-15); //2/4
	assert(fabs(d.ewa[hi].relFreq(gruzi) - 1.3862943611198906188) < 1e-15); //1/4
	assert(fabs(d.ewa[hi].relFreq(willkommen) - 1.3862943611198906188) < 1e-15);

	assert(d.ewa[howdy].singlecount == 1);
	assert(d.ewa[howdy].relFreq(hallo) == 0);

	assert(d.ewa[good].singlecount == 1);
	assert(d.ewa[good].relFreq(hallo) == 0);

	assert(d.ewa[evening].singlecount == 1);
	assert(d.ewa[evening].relFreq(hallo) == 0);

	assert(d.ewa[gday].singlecount == 1);
	assert(d.ewa[gday].relFreq(hallo) == 0);

	/*d.add("der","the");
	d.add("die","the");
	d.add("das","the");
	d.add("drop","fallen");
	d.add("drop","lassen");*/


}
