#include "decoder.h"

class BleuInfo{
public:	
	static unsigned int N; //default: 4
	std::vector<unsigned int> gramCount;
	std::vector<unsigned int> matchCount;
	unsigned int ref_len;
	unsigned int hyp_len;

	BleuInfo(
		const Decoder::Sentence& ref, const Decoder::Sentence& hyp);

	BleuInfo& operator+=(const BleuInfo&);
	BleuInfo& add(const BleuInfo&);

	BleuInfo& operator-=(const BleuInfo&);
	BleuInfo& subtract(const BleuInfo&);

	static BleuInfo membleu(
		std::vector<Decoder::hypRefPair>&,
		const std::vector<unsigned int>& picks
	);

	double operator()() const; //ruft totalize auf
	double totalize() const;
};
