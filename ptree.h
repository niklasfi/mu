#pragma once
#include <map>
#include <vector>
#include <iostream>

typedef unsigned int uint;

template <class T>
class PTree{
	std::map<uint,PTree*> outbound;
	/* Eine Map für die ausgehenden Kanten. Diese ist besonders sinnvoll,
	 * da lookups (outbound[ ]) und insertion besonders schnell gehen.
	 * Spart einem von Hand auf einem Array einen AVL-Baum zu implemen-
	 * tieren */

	PTree* insertion_traversal(std::vector<uint> query, uint start = 0){
		/* Diese Funktion legt alle in Query hinter start liegenden Knoten
		 * und Kanten mit Inhalt T() an. Sie wird nur dann aufgerufen, wenn
		 * query[0] nicht im aktuellen Knoten gefunden wurde */
		PTree* next = this;
		PTree* last;
		for(uint i = start; i < query.size(); i++){
			last = next;
			next = new PTree();
			last->outbound[query[i]] = next;
		}
		return next;
	}

	public:
	T c; //der eigentliche Inhalt des Knotens

	PTree(const T& t = T()):c(t){}
	~PTree(){
		/* Destruktor: da wir eine map<uint, Ptree*> haben müssen wir alle
		 * Objekte, die von outbound referenziert werden löschen */
		for(typename std::map<uint,PTree*>::iterator it = outbound.begin(); it != outbound.end(); it ++)
			delete it->second;
	}

	PTree* traverse(std::vector<uint> query, bool insert = false){
		/* Diese Funktion traversiert den Baum endlang des durch query vor-
		 * gegebenen Pfades, ausgehend von *this. Rückgabe wert ist der
		 * letzte Knoten auf dem Pfad.
		 * insert == false: existiert der Pfad im Baum nicht, return 0
		 * insert == true: für den Pfad benötigte Knoten werden mit T() an-
		 * gelegt, und ein Zeiger auf den letzen angelegten Knoten wird
		 * zurückgegeben */
		PTree* next = this;
		PTree* last;
		for(uint i = 0; i < query.size(); i++){
			last = next;
			next = next->outbound[query[i]];
			if(!next){
				if (!insert) return 0;
				else return last->insertion_traversal(query,i);
			}
		}

		return next;
	}

	PTree* traverse(uint key, bool insert = false,const T& value = T()){
		/* wie traverse(std::vector<uint>,bool), aber der Pfad hat nur länge
		 * 1 und kann somit mit einem einzigen uint charakterisiert werden.
		 * ist insert = true, kann mit value ein in content des neu erstell-
		 * ten Knotens zu kopierenden Wert übergeben werden. */
		PTree* next = outbound[key];
		if(next) return next;
		if(!insert) return 0;

		//einfügen
		next = new PTree(value);
		outbound[key] = next;
		return next;
	}
};



