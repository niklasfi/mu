#pragma once
#include <map>
#include <vector>
#include <deque>
#include <iostream>
#include <assert.h>

typedef unsigned int uint;

template <class T>
class PTree{
	public:std::map<uint,PTree*> outbound;
	/* Eine Map für die ausgehenden Kanten. Diese ist besonders sinnvoll,
	 * da lookups (outbound[ ]) und insertion besonders schnell gehen.
	 * Spart einem von Hand auf einem Array einen AVL-Baum zu implemen-
	 * tieren */

	PTree* insertion_traversal(std::vector<uint> query, uint start = 0, const T& value = T()){
		/* Diese Funktion legt alle in Query hinter start liegenden Knoten
		 * und Kanten mit Inhalt T() an. Sie wird nur dann aufgerufen, wenn
		 * query[0] nicht im aktuellen Knoten gefunden wurde */
		PTree* next = this;
		PTree* last;
		for(uint i = start; i < query.size(); i++){
			last = next;
			next = new PTree(value,last,query[i]);
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
	PTree(const PTree& t):
		parent(t.parent),inbound(t.inbound),c(t.c){}
	PTree(PTree* parent, uint inbound):
		parent(parent),inbound(inbound),c(T()){}
	~PTree(){
		/* Destruktor: da wir eine map<uint, Ptree*> haben müssen wir alle
		 * Objekte, die von outbound referenziert werden löschen */
		for(typename std::map<uint,PTree*>::iterator it = outbound.begin(); it != outbound.end(); it ++){
			if(it->second){
				it->second->parent = 0;
				delete it->second;
			}
		}
		if(parent)
			parent->outbound.erase(inbound);
	}
	
	PTree& operator=(const PTree& t){
		for(typename std::map<uint,PTree*>::iterator it = outbound.begin(); it != outbound.end(); it ++){
			it->second->parent = 0;
			delete it->second;
		}
		parent = t.parent;
		
		for(typename std::map<uint,PTree*>::const_iterator it = t.outbound.begin(); it != t.outbound.end(); it ++){
			outbound[it->first]= new PTree(*it->second);
		}
		
		c = t.c;
	}
	
	PTree* traverse(std::vector<uint> query, bool insert = false, const T& value = T()){
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
				else return last->insertion_traversal(query,i,value);
			}
		}

		return next;
	}

	std::vector<uint> phrase(PTree* targetParent = 0){
		/* Gibt den Pfad ausgehend von targetParent zum aktuellen Knoten aus.
		 * Hierbei ist zu beachten, dass nur direkte Pfade nach oben genommen
		 * werden. Wird für targetParent der defaultwert 0 gewählt, so ent-
		 * spricht der Rückgabewert dem Pfad vom aktuellen Knoten zum Wurzel-
		 * Knoten des Präfixbaumes */

		if(this == targetParent) return std::vector<uint>();

		std::vector<uint> v;
		PTree* next = this;
		while(next->parent != targetParent){
			v.push_back(next->inbound);
			next = next -> parent;
		}
		
		for(uint i = 0; i<v.size()/2;i++){
			uint tmp;
			tmp = v[i];
			v[i] = v[v.size()-1-i];
			v[v.size()-i-1] = tmp;
		}
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
		/* Diese Klasse implementiert einen Iterator ähnlich denen, in der
		 * STL. Das ziel des Iterators ist es jeden Knoten außer der Wurzel
		 * jeweils genau einmal zu erreichen. Ein iterator wird zunächst mit
		 * der begin Funktion von PTree erzeugt. Dann zeigt der Iterator auf
		 * das linkeste Kind des PTrees. Mit operator++ kann jetzt der Baum
		 * in Form einer Breitensuche traversiert werden. Wie üblich sollte
		 * die Abfrage am Ende einer For-Schleife mit it != ptree.end()
		 * geschehen */
		PTree* cur_nod;
		typename std::map<uint,PTree*>::iterator cur_it;
		std::deque<PTree*> toXplore;
		
		bool ended() const{
			/* Diese eigenschaft ist wahr, wenn die iteration Komplett ist */
			return !cur_nod || cur_nod->outbound.end() == cur_it;
		}
		
		public:
		iterator():cur_nod(0){}

		iterator& operator=(const iterator& orig){
			cur_nod = orig.cur_nod;
			cur_it = orig.cur_it;
			toXplore = orig.toXplore;
			return *this;
		}
		
		bool operator==(const iterator i2) const{
			/* ein Billiger vergleichsoperator, der nur Überprüft, ob beide
			 * iteratoren das Ende erreicht haben */
			return i2.ended()==ended();
		}
		
		bool operator!=(const iterator i2) const{
			return i2.ended()!=ended();
		}

		PTree& operator*(){
			/* Gibt den PTree zurück, auf den der Iterator zur Zeit zeigt. */
			return *cur_it->second;
		}
		
		PTree* operator->(){
			/* Gibt einen Zeiger auf den Ptree zurück, auf den der Iterator
			 * zur Zeit zeigt. */
			return cur_it->second;
		}
		
		iterator& operator++(int i){
			if(ended())
				return *this; //wir sind fertig
			
			cur_it ++;

			if(cur_it == cur_nod->outbound.end()){
				//neuen Knoten zum explorieren suchen
				PTree* nNode = 0;
				while((!nNode || nNode->outbound.empty()) && !toXplore.empty()){

					nNode = toXplore.front();
					toXplore.pop_front();
				}
				cur_nod = nNode;
				cur_it = cur_nod->outbound.begin();
				
				for(typename std::map<uint,PTree*>::iterator it = cur_nod->outbound.begin(); it != cur_nod->outbound.end(); it++){
					toXplore.push_back(it->second); //alle Kindknoten zur Explorationsliste hinzufügen
				}
			}
			
			return *this;
		}
		
		friend iterator PTree::begin();
		friend iterator PTree::end();
	};

	iterator begin(){
		iterator it;
		it.cur_nod = this;
		it.cur_it = this->outbound.begin();

		for(typename std::map<uint,PTree*>::iterator it2 =this->outbound.begin(); it2!=this->outbound.end(); it2++)
			it.toXplore.push_back(it2->second);
		return it;
	}
	
	iterator end(){
		iterator it;
		it.cur_nod = 0;
		return it;
	}
	
	void dot(){
		std::cout << "digraph g{\n";
		for(iterator it = this->begin(); it != end(); it++){
			std::cout << "\t";
			if(it->parent->parent == 0) std::cout << "root";
			else{
				std::vector<uint> ph = it->parent->phrase();
				for(uint i = 0; i < ph.size(); i++)
					std::cout << ph[i];
			}
			
			std::cout << " -> ";
			
			std::vector<uint> ph = it->phrase();
			for(uint i = 0; i < ph.size(); i++)
				std::cout << ph[i];
			
			std::cout << " [label=\"" << it->inbound << "\"]\n";
		}
		std::cout << "}\n";
	}
};



