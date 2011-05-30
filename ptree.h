#pragma once
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <assert.h>

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
			next = new PTree(T(),last,query[i]);
			last->outbound[query[i]] = next;
		}
		return next;
	}

	PTree* parent;
	uint inbound;

	public:
	T c; //der eigentliche Inhalt des Knotens

	PTree(const T& t = T(),PTree* parent = 0, uint inbound = 0):
		parent(parent),inbound(inbound),c(t){}
	PTree(PTree* parent, uint indbound):
		parent(parent),inbound(inbound),c(T()){}
	~PTree(){
		/* Destruktor: da wir eine map<uint, Ptree*> haben müssen wir alle
		 * Objekte, die von outbound referenziert werden löschen */
		for(typename std::map<uint,PTree*>::iterator it = outbound.begin(); it != outbound.end(); it ++)
			delete it->second;
		if(parent)
			parent->outbound.erase(inbound);
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

	std::vector<uint> phrase(PTree* targetParent = 0){
		if(this == targetParent) return std::vector<uint>();

		std::vector<uint> v;
		PTree* next = this;
		while(next->parent != targetParent){
			v.push_back(next->inbound);
			next = next -> parent;
		}
		//TODO
		/*for(uint i = 0; i<v.size()/2;i++){
			uint tmp;
			tmp = v[i];
			v[i] = v[v.size()-i];
			v[v.size()-i] = tmp;
		}*/
		return v;
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
		next = new PTree(value,this,key);
		outbound[key] = next;
		return next;
	}

	class iterator{
		public:
		PTree* cur_nod;
		typename std::map<uint,PTree*>::iterator cur_it;
		std::queue<PTree*> toXplore;
		bool end;
		bool lastStep;

		iterator():cur_nod(0),end(false),lastStep(false){}

		iterator(const iterator& orig):
			cur_nod(orig.cur_nod),
			cur_it(orig.cur_it),
			toXplore(orig.toXplore){}

		iterator& operator=(const iterator& orig){
			cur_nod = orig.cur_nod;
			cur_it = orig.cur_it;
			toXplore = orig.toXplore;
			return *this;
		}

		PTree& operator*(){
			return *cur_it->second;
		}

		iterator& operator++(int i){
			std::cout << "size: " << toXplore.size() << "\n";

			if (lastStep){
				end=true;
				return *this;
			}

			cur_it ++;

			if(cur_it == cur_nod->outbound.end()){
				//cur_nod = toXplore.pop();
				//cur_it = cur_nod(it->outbund.begin());

				//neuen Knoten zum explorieren suchen
				PTree* nNode = 0;
				std::cout << "empty: " << toXplore.empty() << "\n";
				while((!nNode || nNode->outbound.empty()) && !toXplore.empty()){

					nNode = toXplore.front();
					toXplore.pop();
					std::cout << "nNode: " << nNode->c << "\n";
					std::cout << "empty: " << nNode->outbound.empty() << "\n";

					if(toXplore.empty() && (!nNode || nNode->outbound.empty())){
						lastStep=true;
						cur_nod = nNode;
						return *this;
					}
				}
				cur_nod = nNode;
				std::cout << "cur_nod: " << cur_nod << "\n";
				cur_it = cur_nod->outbound.begin();
			}

			for(typename std::map<uint,PTree*>::iterator it = cur_it->second->outbound.begin(); it != cur_it->second->outbound.end(); it++){
				toXplore.push(it->second); //alle Kindknoten zur Explorationsliste hinzufügen
			}
			return *this;
		}
	};

	iterator begin(){
		iterator it;
		it.cur_nod = this;
		it.cur_it = outbound.begin();

		for(typename std::map<uint,PTree*>::iterator it2 =this->outbound.begin(); it2!=this->outbound.end(); it2++)
			it.toXplore.push(it2->second);

		return it;
	}

	iterator end(){
		iterator it;
		it.end = true;
	}
};



