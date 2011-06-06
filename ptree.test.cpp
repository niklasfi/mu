#include "ptree.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>

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
		std::vector<uint> ref = {1,2,3};
		//da die Phrase rückwärts ausgegeben wird, muss hier 3,2,1 stehen
		std::vector<uint> hyp = p.traverse(std::vector<uint>{1,2,3})->phrase();

		assert(ref.size() == hyp.size());
		for(uint i = 0; i < hyp.size(); i++){
			//std::cout << i << ": " << ref[i] << ", " << hyp[i] << "\n";
			assert(ref[i] == hyp[i]);
		}
	}

	{ //überprüfen der Ausgabe der Phrase
		std::vector<uint> ref = {1,2};
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
	
	/* DOT graph des zu durchlaufenden Graphen:
		digraph test{
			r -> 1
			1 -> 0
			1 -> 2 -> 3;
					 2 -> 4 -> 8;
			1 -> 5 -> 7;
			1 -> 9
		}
	*/
	
	PTree<double>* one = p.traverse(1,true,1);
	one->traverse(0,true,0);
	one->traverse(5,true,5)->traverse(7,true,7);
	one->traverse(2,true,2)->traverse(4,true,4)->traverse(8,true,8);
	one->traverse(2)->traverse(3,true,3);
	one->traverse(9,true,9);

	std::vector<uint> ref = {1,0,2,5,9,3,4,7,8};
	
	uint i = 0;
	for(PTree<double>::iterator it = p.begin(); it!=p.end(); it++){
		assert(it->c == ref[i]);
		
		/*std::vector<uint> ph = it->phrase();
		for(std::vector<uint>::iterator i2 = ph.begin(); i2 != ph.end(); i2++){
			std::cout << *i2;
		}
		std::cout << "!\n";*/
		i++;
	}
	
	return true;
}

bool test3(){
     /* Ich versuche mal einen ganz kleinen Baum, so wie wir in brauchen ..., also er hat 2 Knoten im schwarzen Baum 8->1
      * und im blauen sollen auch zwei Knoten sein 5->10, eingehängt soll der blaue Baum im 2. Knoten sein. Im blauen Knoten mit der 10
      * soll die double 0.25 gespeichert sein*/
     PTree<PTree<double>> schwarz;
     PTree<PTree<double>>* schwarz_ptr=schwarz.traverse(8,true,8);
     schwarz_ptr->traverse(1,true,1);
     PTree<double>* blau_ptr= new PTree<double>;
     blau_ptr->traverse(5,true,5);
     PTree<double>* blau_ptr2= blau_ptr->traverse(10,true,10);
		 blau_ptr2->c=0.25;
     schwarz_ptr->c=*blau_ptr;
     
     std::vector<uint> v1; //Weg zum Blauen Baum
     v1.push_back(8); v1.push_back(1);
     
     std::vector<uint> v2;
     v2.push_back(5); //Weg im blauen Baum zum Knoten mit relfreq
     
     /*im debugging sagt er hier: p schwarz.traverse(v1,false)->c.traverse(v2,false) 
      * $5 = (PTree<double> *) 0x0 
      * allerdings stimmen auch die Adressen von schwarz.traverse(v1,false)->c und von blau_ptr nicht überein
      */
    
     
     assert( fabs( schwarz.traverse(v1)->c.traverse(v2)->c - 0.25) < 0.0000001); 

     //phrase müsste also 10,5 sein
     assert(blau_ptr2->phrase()[0] == 10);
     assert(blau_ptr2->phrase()[1] == 5);
     return true;
}


bool test4(){
	//wie test 3, nur in funktional...
	
	PTree<PTree<double>> schwarz;
	
	PTree<double>* blau = &schwarz.traverse(std::vector<uint>{8,1},true)->c;
	
	blau->traverse(std::vector<uint>{5,10},true)->c = .25;
	
	assert( fabs( schwarz.traverse(std::vector<uint>{8,1})->c.traverse(std::vector<uint>{5,10})->c - 0.25) < 0.0000001); 
	
	return true;
}

bool test5(){
	PTree<PTree<double>> schwarz;
	PTree<PTree<double>>* schwarz_ptr=schwarz.traverse(8,true,8);
	schwarz_ptr->traverse(1,true,1);
	PTree<double>* blau_ptr= new PTree<double>;
	blau_ptr->traverse(5,true,5)->c=0.25;
	PTree<double>* blau_ptr2= blau_ptr->traverse(10,true,10);;
	schwarz_ptr->c=*blau_ptr;

	std::vector<uint> v1; //Weg zum Blauen Baum
	v1.push_back(8); v1.push_back(1);

	std::vector<uint> v2;
	v2.push_back(5); //Weg im blauen Baum zum Knoten mit relfreq

	/*im debugging sagt er hier: p schwarz.traverse(v1,false)->c.traverse(v2,false) 
	* $5 = (PTree<double> *) 0x0 
	* allerdings stimmen auch die Adressen von schwarz.traverse(v1,false)->c und von blau_ptr nicht überein
	*/

	PTree<double>* blau = &schwarz.traverse(8)->c;
	PTree<double>* five = blau->traverse(5,false);
	
	

	assert( fabs( schwarz.traverse(8)->c.traverse(v2)->c - 0.25) < 0.0000001); 

	//phrase müsste also 10 sein
	assert(blau_ptr2->phrase()[0] == 10);
	
	delete blau_ptr;
	
	return true;
}

int main(){
	assert(test1());
	assert(test2());
	assert(test4());
	assert(test5());
	//assert(test3());
}
