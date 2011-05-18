#include <assert.h>

#include "sentencepool.h"

int main(){
	//dieser test soll eigentlich nur sicherstellen, dass die enum implementiert ist. auf die Zahlen kommt es mir nicht an.
	
	assert( SentencePool::ref == 0);
	assert( SentencePool::gue == 1);
	
	{
		SentencePool sp; //leerer konstruktor

		sp.addSentence(SentencePool::ref, "a b c d e f");
		sp.addSentence(SentencePool::gue, "d e");

		sp.addSentence(SentencePool::ref, "a a a");
		sp.addSentence(SentencePool::gue, "g h");

		sp.addSentence(SentencePool::ref,"");
		sp.addSentence(SentencePool::gue,""); //mal gucken was passiert, wenn wir den leeren Satz einfügen
		
		sp.addSentence(SentencePool::ref, "g h k");
		sp.addSentence(SentencePool::gue, "g k");

		uint a = sp.elex.getWord("a");
		uint b = sp.elex.getWord("b");
		uint c = sp.elex.getWord("c");
		uint d = sp.elex.getWord("d");
		uint e = sp.elex.getWord("e");
		uint f = sp.elex.getWord("f");
		uint g = sp.elex.getWord("g");
		uint h = sp.elex.getWord("h");
		uint k = sp.elex.getWord("k");
		
		{ //1. Satz
			assert((*sp.reference[0]).size() == 6);
			uint ref[] = {a,b,c,d,e,f};
			for (uint i = 0; i< 6; i++)
				assert(ref[i] == (*sp.reference[0])[i]);
			
			assert((*sp.guess[0]).size() == 2);
			uint gue[] = {d,e};
			for (uint i = 0; i< 2; i++)
				assert(gue[i] == (*sp.guess[0])[i]);
		}
		
		{ //2. Satz
			assert((*sp.reference[1]).size() == 3);
			uint ref[] = {a,a,a};
			for (uint i = 0; i< 3; i++)
				assert(ref[i] == (*sp.reference[1])[i]);
			
			assert((*sp.guess[1]).size() == 2);
			uint gue[] = {g,h};
			for (uint i = 0; i< 2; i++)
				assert(gue[i] == (*sp.guess[1])[i]);
		}

		{ //4. Satz
			assert((*sp.reference[2]).size() == 3);
			uint ref[] = {g,h,k};
			for (uint i = 0; i< 3; i++)
				assert(ref[i] == (*sp.reference[2])[i]);
			
			assert((*sp.guess[2]).size() == 2);
			uint gue[] = {g,k};
			for (uint i = 0; i< 2; i++)
				assert(gue[i] == (*sp.guess[2])[i]);
		}	
	}	
	
}
