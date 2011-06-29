#include <algorithm>
#include "sentencepool.h"
#include "sentenceinfo.h"
#include <omp.h>
#include <cmath>

typedef unsigned int uint;

double bleu_bp(uint refs, uint hyps){ //Brevity Penalty
	if(refs <= hyps) return 1.0;
	return exp(1.0-(1.0*refs)/hyps);
}
double bleu(SentencePool& sp, int N = 4){
	//berechnet bleu

	uint* matchcount = new uint[N]; //zählt die Matches wobei der Index für die Länge des n+1-grams steht (0 -> unigram)
	std::fill(matchcount, matchcount + N, 0);//alles auf 0 setzen


	uint* gramcount = new uint[N]; //zählt die Anzahl der möglichen n-grams
	std::fill(gramcount, gramcount + N, 0);


	uint refsize = 0;
	uint hypsize = 0;

	for(unsigned int sent = 0; sent < sp.guess.size(); sent++){
		std::vector<uint>& hyp = *sp.guess[sent], ref = *sp.reference[sent];

		int* gramsize = new int[ hyp.size()];
		std::fill(gramsize, gramsize + hyp.size(), 0);
		/* gramsize speichert für ein wort in der hypothese, wie lang das Länste
		 * n-gram bis jetzt war, in dem es verwendet wurde. Dies dient dazu mehr-
		 * fachverwertungen zu verhindern */

		for(unsigned int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
			int rseq = 0; //speichert für das aktuelle referenzwort, wie groß das beste n-gram ist.
			for(unsigned int l = 0; l < hyp.size() - rseq; l++){ //l index des wortes im Ü-Vorschlag
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
						matchcount[gramsize[l]]++;
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
		//nextSent:

		delete[] gramsize;

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

	delete[] gramcount;
	delete[] matchcount;

	double d = exp(result) * bleu_bp(refsize,hypsize);
	return d;
}

void bleu_sent( std::vector<uint> ref, SentenceInfo& hyp, int N = 4){
	if(hyp.bleu) return;
	
	hyp.bleu = new std::vector <std::pair <uint, uint>>(N,std::pair<uint,uint>(0,0));
	//Speicher für matchcount und gramcount alokieren
	
	int* gramsize = new int[ hyp.sentence.size()];
	std::fill(gramsize, gramsize + hyp.sentence.size(), 0);
	/* gramsize speichert für ein wort in der hypothese, wie lang das Länste
	 * n-gram bis jetzt war, in dem es verwendet wurde. Dies dient dazu mehr-
	 * fachverwertungen zu verhindern */

	for(unsigned int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
		int rseq = 0; //speichert für das aktuelle referenzwort, wie groß das beste n-gram ist.
		for(unsigned int l = 0; l < hyp.sentence.size() - rseq; l++){ //l index des wortes im Ü-Vorschlag
			int hseq = 0; //speichert für das aktuelle hypothesenwort, wie groß das beste n-gram ist.
			if(gramsize[l] >= N) goto nextHyp; //wenn wir für dieses wort schon ein n-gram maximaler länge gefunden haben, wissen wir, dass wir nicht weitersuchen müssen
			if(rseq + k >= ref.size()) goto nextRef;

			/*assert(seq+k < ref.size());
			assert(seq+l < hyp.sentence.size());*/
			while(ref[hseq+k] == hyp.sentence[hseq+l]){
			/* hier ist die eigentliche Magick am werk. Wir veringern die Komplex-
			 * ität, indem wir nicht für jede n-gram länge nochmal über jedes wort
			 * gehen, sondern nur jede referenz- und hypothesenwort kombination
			 * nur genau einmal bearbeiten müssen und dann gucken, ob wir noch ein
			 * längeren n-gram finden */
				if(hseq >= gramsize[l]){
					//matchcount[gramsize[l]]++;
					hyp.bleu->at(gramsize[k]).first++;
					gramsize[l]++;
					rseq = hseq + 1;
					if(rseq + l >= hyp.sentence.size() || rseq+k >= ref.size())
						goto nextRef;
				}
				hseq++;
				if(hseq + l >= hyp.sentence.size() || hseq+k >= ref.size() || hseq >= N)
					goto nextHyp;
			}
			nextHyp: ;
		}
		nextRef: ;
	}
	//nextSent:

	delete[] gramsize;

	for(int i = 0; i < N && hyp.sentence.size()-i > 0; i++)
		//gramcount[i] += hyp.sentence.size() - i;
		hyp.bleu->at(i).second += hyp.sentence.size() - i;
}

double membleu( std::vector< std::pair < std::vector<uint>, std::vector <SentenceInfo> > > nBestList, std::vector<uint> picks, uint N = 4){
	std::vector<uint> matchcount(N,0), gramcount(N,0);
	uint refsize = 0, hypsize = 0;
	
	for(uint i = 0; i < nBestList.size(); i++){
		std::vector<uint>& ref = nBestList[i].first;
		SentenceInfo hyp = nBestList[i].second[picks[i]];
		bleu_sent(ref,hyp,N);
		
		for(uint k = 0; k < N; k++){
			matchcount[k] += hyp.bleu->at(k).first;
			gramcount[k]  += hyp.bleu->at(k).second;
		}
		refsize += ref.size();
		hypsize += hyp.sentence.size();
	}
	
	double result = 0;
	
	for(unsigned int i = 0; i<N; i++)
		result += log(((double) matchcount[i])/gramcount[i]);

	result /= N;

	double d = exp(result) * bleu_bp(refsize,hypsize);
	return d;
}
