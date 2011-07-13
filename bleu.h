#pragma once
#include "decoder.h"
#include "sentenceinfo.h"
#include "sentencepool.h"
#include "sentence.h"
#include "hyprefpair.h"


class BleuInfo{
public:	
	static unsigned int N; //default: 4
	std::vector<unsigned int> gramCount;
	std::vector<unsigned int> matchCount;
	unsigned int ref_len;
	unsigned int hyp_len;

	BleuInfo();
	BleuInfo(const Sentence& ref, const Sentence& hyp);

	BleuInfo& operator+=(const BleuInfo&);
	BleuInfo& add(const BleuInfo&);

	BleuInfo& operator-=(const BleuInfo&);
	BleuInfo& subtract(const BleuInfo&);

	static BleuInfo membleu(
		std::vector<hypRefPair>&,
		const std::vector<unsigned int>& picks
	);

	double operator()() const; //ruft totalize auf //gibt den BLEU-Wert zurück
	double totalize() const;

	static double bleu(SentencePool& sp);
private:
	double bleu_bp() const;
};
