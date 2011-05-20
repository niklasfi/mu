#include <algorithm>
#include "sentencepool.h"


typedef unsigned int uint;

double bleu_bp(uint referenceSize, uint guessSize){
	uint& c = guessSize;
	uint& r = referenceSize;
	
	if(c >= r) return 1;
	return exp(1-r/c);
}

double bleu(SentencePool& sp){
	uint N = 4;

	uint* matchcount = new uint[N];
	std::fill(matchcount, matchcount + N, 0);//alles auf 0 setzen
	
	uint* gramcount = new uint[N];
	std::fill(gramcount, gramcount + N, 0);
	
	uint refsize = 0;
	uint guesssize = 0;
	
	for(int sent = 0; sent < sp.guess.size(); sent++){
		std::vector<uint>& gs = *sp.guess[sent], ref = *sp.reference[sent];
		
		char* guess_n = new char[gs.size()];
		std::fill(guess_n, guess_n + gs.size(), 0);
		
		for(int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
			for(int l = 0; l < gs.size(); l++){ //l index des wortes im Ü-Vorschlag
				if(guess_n[l] < N){ //wenn wir für dieses wort schon ein n-gram maximaler länge gefunden haben, wissen wir, dass wir nicht weitersuchen müssen
					int seq = 0;
					while(seq+k < ref.size() && seq + l < gs.size() && ref[seq+k] == gs[seq+l] && seq < N){
						/*     (1)                     (2)                        (3)                (4)
						 * (1) überprüfen, ob das n-gram noch in den Referenzsatz passt
						 * (2) überprüfen, ob das n-gram noch in den Übersetzungsvorschlag passt
						 * (3) überprüfen, ob das Wort im Rerfenz- mit dem Vorschlagssatz übereinstimmt
						 * (4) abbrechen, wenn seq zu groß wird
						 */
						if(seq > guess_n[l]){
							matchcount[guess_n[l]]++;
							guess_n[l]++;
						}
						seq++;
					}
				}
			}
		}
		refsize += ref.size();
		guesssize += gs.size();
		
		for(int i = 0; i < N; i++)
			gramcount[i] += ref.size() - i;
	}
	
	//matchcount und gramcount jetzt korrekt
	
	double result = 0;

	for( int i = 0; i<N; i++)
		result += log(((double) matchcount[i])/gramcount[i]);

	result /= N;

	return result * bleu_bp(refsize,guesssize);
}
