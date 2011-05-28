#include "ptree.h"
#include <iostream>
#include <assert.h>
#include <vector>
int main(){
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

}
