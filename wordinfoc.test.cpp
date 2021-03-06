#include "wordinfoc.h"

#include <iostream>

void maxlen(){
	WordinfoC::set_maxlen(3);

	try{
		WordinfoC::set_maxlen(5);
		assert(false);
	}
	catch(WordinfoC::maxlenAlreadySetException m) {}
}

void straight(){
	WordinfoC winfo1;

	for (uint i = 0; i<30; i++)
		winfo1.add_translation(i,i/30.0);

	{
		plist::iterator it;
		uint i = 0;
		for(it = winfo1.pairs.begin(); it != winfo1.pairs.end(); it++){
			assert( i/30.0 == it->first);
			assert( i == it->second);
			i++;
		}
	}
}

void mixed(){
	double data[7] = {.1, .2, .7, .4, .5, .6, .3};
	double sorted[3] = {.1, .2, .3};
	uint sortedI[3] = {0,1,6};

	WordinfoC winfo2;

	for (uint i = 0; i<7; i++)
		winfo2.add_translation(i,data[i]);

	{
		plist::iterator it;
		uint i = 0;
		for(it = winfo2.pairs.begin(); it != winfo2.pairs.end(); it++){
			assert( sorted[i] == it->first );
			assert( sortedI[i] == it->second );
			i++;
		}
	}
}

void straightRev(){
	WordinfoC winfo1;

	for (uint i = 0; i<31; i++)
		winfo1.add_translation(i,1-i/30.0);

	{
		plist::iterator it;
		uint i = 0;
		for(it = winfo1.pairs.begin(); it != winfo1.pairs.end(); it++){
			assert( fabs(i/30.0 - it->first) < 1e-10);
			assert( 30-i == it->second);
			i++;
		}
	}
}

void singlecount(){
	WordinfoC winfo1(3);

	try{
		winfo1.set_singlecount(4);
		assert (false);
	}
	catch (WordinfoC::singleCountAlreadySetException s){}

	WordinfoC winfo2;

	winfo2.set_singlecount(3);
	try{
		winfo2.set_singlecount(10);
		assert (false);
	}
	catch (WordinfoC::singleCountAlreadySetException s){}
}

void conversion(){
	Wordinfo wi;
	wi.pairs[132]=12;
	wi.pairs[78]=8;
	wi.pairs[4]=4;
	wi.pairs[8]=24;
	wi.pairs[13]=24;
	wi.singlecount = 72;

	WordinfoC wic(wi);

	double result[3] = {wi.relFreq(13),wi.relFreq(8),wi.relFreq(132)};
	uint resultW[3] = {13,8,132};

	{
		plist::iterator it;
		uint i = 0;
		for(it = wic.pairs.begin(); it != wic.pairs.end(); it++){
			assert( result[i] == it->first);
			assert( resultW[i] == it->second);
			i++;
		}
	}
}

int main(){
	maxlen(); //sets maxlen to 3
	straight();
	mixed();
	straightRev();
	singlecount();
	conversion();
	return 0;
}
