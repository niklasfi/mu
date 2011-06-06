#include <algorithm>
#include "sentencepool.h"
#include <omp.h>
#include <cmath>

typedef unsigned int uint;

double bleu_bp(uint refs, uint hyps){ //Brevity Penalty
	if(refs <= hyps) return 1.0;
	return exp(1.0-(1.0*refs)/hyps);
}
double bleu(SentencePool& sp, uint N = 4){
	//berechnet bleu

	uint* matchcount = new uint[N]; //zählt die Matches wobei der Index für die Länge des n+1-grams steht (0 -> unigram)
	std::fill(matchcount, matchcount + N, 0);//alles auf 0 setzen


	uint* gramcount = new uint[N]; //zählt die Anzahl der möglichen n-grams
	std::fill(gramcount, gramcount + N, 0);


	uint refsize = 0;
	uint hypsize = 0;

	//parallel macht mehr spaß:
	#pragma omp parallel
	{
		#pragma omp for
		for(int sent = 0; sent < sp.guess.size(); sent++){
			std::vector<uint>& hyp = *sp.guess[sent], ref = *sp.reference[sent];

			char* gramsize = new char[ hyp.size()];
			std::fill(gramsize, gramsize + hyp.size(), 0);
			/* gramsize speichert für ein wort in der hypothese, wie lang das Länste
			 * n-gram bis jetzt war, in dem es verwendet wurde. Dies dient dazu mehr-
			 * fachverwertungen zu verhindern */

			for(int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
				int rseq = 0; //speichert für das aktuelle referenzwort, wie groß das beste n-gram ist.
				for(int l = 0; l < hyp.size() - rseq; l++){ //l index des wortes im Ü-Vorschlag
					int hseq = 0; //speichert für das aktuelle hypothesenwort, wie groß das beste n-gram ist.
					if(gramsize[l] >= N) goto nextHyp; //wenn wir für dieses wort schon ein n-gram maximaler länge gefunden haben, wissen wir, dass wir nicht weitersuchen müssen
					if(rseq + k >= ref.size()) goto nextRef;

					/*assert(seq+k < ref.size());
					assert(seq+l < hyp.size());*/
					while(ref[hseq+k] == hyp[hseq+l]){
					/* hier ist die eigentliche Magick am werk. Wir veringern die Komplex-
					 * ität, indem wir nicht für jede n-gram länge nochmal über jedes wort
					 * gehen, sondern nur jede referenz- und hypothesenwort kombination
					 * nur genau einmal bearbeiten müssen und dann gucken, ob wir noch ein
					 * längeren n-gram finden */
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
