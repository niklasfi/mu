class BleuInfo;

#include "sentence.h"

#include "bleu.h"

#include <algorithm>
#include <cmath>

typedef unsigned int uint;

unsigned int BleuInfo::N(4);

BleuInfo::BleuInfo():gramCount(4,0),matchCount(4,0),ref_len(0),hyp_len(0){}

BleuInfo::BleuInfo(const Sentence& ref,
	const Sentence& hyp):
gramCount(N,0), matchCount(N,0), ref_len(ref.size()),hyp_len(hyp.size())
{
	unsigned int* gramsize = new unsigned int[ hyp.size()];
	std::fill(gramsize, gramsize + hyp.size(), 0);
	/* gramsize speichert für ein wort in der hypothese, wie lang das Länste
	 * n-gram bis jetzt war, in dem es verwendet wurde. Dies dient dazu mehr-
	 * fachverwertungen zu verhindern */

	for(unsigned int k = 0; k < ref.size(); k++){ //k index des vergleichwortes in der ref
		int rseq = 0; //speichert für das aktuelle referenzwort, wie groß das beste n-gram ist.
		for(unsigned int l = 0; l < hyp.size() - rseq; l++){ //l index des wortes im Ü-Vorschlag
			unsigned int hseq = 0; //speichert für das aktuelle hypothesenwort, wie groß das beste n-gram ist.
			if(gramsize[l] >= N) goto nextHyp; //wenn wir für dieses wort schon ein n-gram maximaler länge gefunden haben, wissen wir, dass wir nicht weitersuchen müssen
			if(rseq + k >= ref.size()) goto nextRef;

			/*assert(seq+k < ref.size());
			assert(seq+l < hyp.sentence.size());*/
			while(ref[hseq+k] == hyp[hseq+l]){
			/* hier ist die eigentliche Magick am werk. Wir veringern die Komplex-
			 * ität, indem wir nicht für jede n-gram länge nochmal über jedes wort
			 * gehen, sondern nur jede referenz- und hypothesenwort kombination
			 * nur genau einmal bearbeiten müssen und dann gucken, ob wir noch ein
			 * längeren n-gram finden */
				if(hseq >= gramsize[l]){
					matchCount[gramsize[l]]++;
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

	for(unsigned int i = 0; i < N && hyp.size()-i > 0; i++)
		gramCount[i] += hyp.size() - i;
}

BleuInfo& BleuInfo::operator+=(const BleuInfo& b){
	return add(b);
}

BleuInfo& BleuInfo::add(const BleuInfo& b){
	for(unsigned int i = 0; i < N; i++){
		gramCount[i]  += b.gramCount[i];
		matchCount[i] += b.matchCount[i];
	}

	ref_len += b.ref_len;
	hyp_len += b.hyp_len;

	return *this;
}

BleuInfo& BleuInfo::operator-=(const BleuInfo& b){
	return subtract(b);
}

BleuInfo& BleuInfo::subtract(const BleuInfo& b){
	for(unsigned int i = 0; i < N; i++){
		gramCount[i]  -= b.gramCount[i];
		matchCount[i] -= b.matchCount[i];
	}

	ref_len -= b.ref_len;
	hyp_len -= b.hyp_len;
		
	return *this;
}

BleuInfo membleu(std::vector<hypRefPair> hrp,
	const std::vector<unsigned int>& picks)
{
	BleuInfo bi;
	
	for(unsigned int i = 0; i < hrp.size(); i++){
		SentenceInfo& si = (*hrp[i].nBest)[picks[i]];
		if(!si.bleu) si.bleu = new BleuInfo(*hrp[i].reference, si.sentence);

		bi += *si.bleu;
	}

	return bi;
}

double BleuInfo::operator()() const{
	return totalize();
}

double BleuInfo::totalize() const{
	double result = 0;

	for(unsigned int i = 0; i < N; i++)
		result += log(((double) matchCount[i])/gramCount[i]);

	result /= N;

	double d = exp(result) * bleu_bp();
	return d;
}

double BleuInfo::bleu_bp() const{ //Brevity Penalty
	if(ref_len <= hyp_len) return 1.0;
	return exp(1.0-(1.0*ref_len)/hyp_len);
}

double bleu(SentencePool& sp){
	BleuInfo bi;

	for(unsigned int i = 0; i < sp.guess.size(); i ++)
		bi += BleuInfo(*sp.reference[i], *sp.guess[i]);

	return bi();
}

