#include <algorithm>
#include "sentencepool.h"
#include <omp.h>

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

	#pragma omp parallel
	{
		#pragma omp for
		for(int sent = 0; sent < sp.guess.size(); sent++){
			std::vector<uint>& hyp = *sp.guess[sent], ref = *sp.reference[sent];

			char* gramsize = new char[ hyp.size()];
			std::fill(gramsize, gramsize + hyp.size(), 0);

			for(int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
				int rseq = 0;
				for(int l = 0; l < hyp.size() - rseq; l++){ //l index des wortes im Ü-Vorschlag
					int hseq = 0;
					if(gramsize[l] >= N) goto nextHyp; //wenn wir für dieses wort schon ein n-gram maximaler länge gefunden haben, wissen wir, dass wir nicht weitersuchen müssen
					if(rseq + k >= ref.size()) goto nextRef;

					/*assert(seq+k < ref.size());
					assert(seq+l < hyp.size());*/
					while(ref[hseq+k] == hyp[hseq+l]){
						if(hseq >= gramsize[l]){
							#pragma omp critical
							matchcount[gramsize[l]]++;
							#pragma omp end critical
							gramsize[l]++;
							rseq = hseq + 1;
							if(rseq + l >= hyp.size() || rseq+k >= ref.size())
								goto nextRef;
						}
						hseq++;
						if(hseq + l >= hyp.size() || hseq+k >= ref.size() || hseq >= N)
							goto nextHyp;
					}
					nextHyp: ;
				}
				nextRef: ;
			}
			nextSent:

			delete[] gramsize;

			#pragma omp critical
			refsize += ref.size();
			hypsize += hyp.size();

			for(int i = 0; i < N && hyp.size()-i > 0; i++)
				gramcount[i] += hyp.size() - i;
			#pragma omp end critical
		}
	}
	//matchcount und gramcount jetzt korrekt

	double result = 0;

	for( int i = 0; i<N; i++)
		result += log(((double) matchcount[i])/gramcount[i]);

	result /= N;

	delete[] gramcount;
	delete[] matchcount;

	double d = exp(result) * bleu_bp(refsize,hypsize);
	return d;
}
