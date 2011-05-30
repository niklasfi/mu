#include "ptree.h"
#include <iostream>
#include <assert.h>
#include <vector>

bool test1(){
		PTree<double> p;

	// so hier jetzt ein Beispiel für die Benutzung von ptree:

	p.traverse(std::vector<uint>{1,2,3},true);

	// jetzt sind die Kanten 1 -> 2 -> 3 vom Root aus angelegt

	{ //setzen wir ein paar Werte auf den Knoten:
		PTree<double>* t = &p;
		t->c = 13;

		t=t->traverse(1);
		t->c = 17;

		t=t->traverse(2);
		t->c = 19;

		t=t->traverse(3);
		t->c = 21;

		/* der t pointer ist jetzt bei dem letzten Knoten auf dem Pfad
		 * 1 -> 2 -> 3 angekommen. Die Knoten sollten jetzt folgende Werte
		 * haben:
		 * root: 13, 1: 17, 2: 19, 3: 21 */
	}

	//überprüfen wir das mal:
	assert(p.c == 13);
	assert(p.traverse(1)->c == 17);
	assert(p.traverse(std::vector<uint>{1,2}) -> c == 19);
	assert(p.traverse(std::vector<uint>{1,2,3}) -> c == 21);

	{ //überprüfen der Ausgabe der Phrase
		std::vector<uint> ref = {3,2,1};
		//da die Phrase rückwärts ausgegeben wird, muss hier 3,2,1 stehen
		std::vector<uint> hyp = p.traverse(std::vector<uint>{1,2,3})->phrase();

		assert(ref.size() == hyp.size());
		for(uint i = 0; i < hyp.size(); i++){
			//std::cout << i << ": " << ref[i] << ", " << hyp[i] << "\n";
			assert(ref[i] == hyp[i]);
		}
	}

	{ //überprüfen der Ausgabe der Phrase
		std::vector<uint> ref = {2,1};
		//da die Phrase rückwärts ausgegeben wird, muss hier 3,2,1 stehen
		std::vector<uint> hyp = p.traverse(std::vector<uint>{1,2})->phrase();

		assert(ref.size() == hyp.size());
		for(uint i = 0; i < hyp.size(); i++){
			//std::cout << i << ": " << ref[i] << ", " << hyp[i] << "\n";
			assert(ref[i] == hyp[i]);
		}
	}
	return true;
}

bool test2(){
	PTree<double> p;

	p.traverse(std::vector<uint>{1,0},true);
	p.traverse(std::vector<uint>{1,5,7},true);
	p.traverse(std::vector<uint>{1,2,4,8},true);
	p.traverse(std::vector<uint>{1,9},true);
	
	PTree<double>* one = p.traverse(1,true,1);
	one->traverse(0,true,0);
	one->traverse(5,true,5)->traverse(7,true,7);
	one->traverse(2,true,2)->traverse(4,true,4)->traverse(8,true,8);
	one->traverse(2)->traverse(3,true,3);
	one->traverse(9,true,9);

	for(PTree<double>::iterator it = p.begin(); !it.ended(); it++){
		std::vector<uint> ph = (*it).phrase();
		for(std::vector<uint>::reverse_iterator i2 = ph.rbegin(); i2 != ph.rend(); i2++){
			std::cout << *i2;
		}
		std::cout << "!\n";
	}
	
	return true;
}

int main(){
	assert(test1());
	assert(test2());
}
