#include <algorithm>
#include "sentencepool.h"


typedef unsigned int uint;

double bleu_bp(uint refs, uint hyps){
	if(refs <= hyps) return 1.0;
	return exp(1.0-(1.0*refs)/hyps);
}
double bleu(SentencePool& sp, uint N = 4){
	uint* matchcount = new uint[N];
	std::fill(matchcount, matchcount + N, 0);//alles auf 0 setzen
	
	uint* gramcount = new uint[N];
	std::fill(gramcount, gramcount + N, 0);
	
	uint refsize = 0;
	uint hypsize = 0;
	
	for(int sent = 0; sent < sp.guess.size(); sent++){
		std::vector<uint>& hyp = *sp.guess[sent], ref = *sp.reference[sent];
		
		char* gramsize = new char[ hyp.size()];
		std::fill(gramsize, gramsize + hyp.size(), 0);
		
		for(int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
			int seq = 0;
			for(int l = 0; l < hyp.size(); l++){ //l index des wortes im Ü-Vorschlag
				if(gramsize[l] < N){ //wenn wir für dieses wort schon ein n-gram maximaler länge gefunden haben, wissen wir, dass wir nicht weitersuchen müssen
					while(seq+k < ref.size() && seq + l < hyp.size() && ref[seq+k] == hyp[seq+l] && seq < N){
						/*     (1)                     (2)                        (3)                (4)
						 * (1) überprüfen, ob das n-gram noch in den Referenzsatz passt
						 * (2) überprüfen, ob das n-gram noch in den Übersetzungsvorschlag passt
						 * (3) überprüfen, ob das Wort im Rerfenz- mit dem Vorschlagssatz übereinstimmt
						 * (4) abbrechen, wenn seq zu groß wird
						 */
						if(seq >= gramsize[l]){
							matchcount[gramsize[l]]++;
							gramsize[l]++;
						}
						seq++;
					}
				}
			}
		}
		refsize += ref.size();
		hypsize += hyp.size();
		
		for(int i = 0; i < N && hyp.size()-i > 0; i++)
			gramcount[i] += hyp.size() - i;
	}
	
	//matchcount und gramcount jetzt korrekt
	
	double result = 0;

	for( int i = 0; i<N; i++)
		result += log(((double) matchcount[i])/gramcount[i]);

	result /= N;

	double d = exp(result) * bleu_bp(refsize,hypsize);
	return d;
}
