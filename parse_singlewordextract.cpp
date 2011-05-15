#include "dictionaryc.h"
#include <fstream>
#include <string>
#include <sstream>
#include "aStar.h"

int main(int argc, char*argv[])
{
	WordinfoC::set_maxlen(2);
	aStar::set_max_SentenceTranslation(10);
	DictionaryC dc;

	std::ifstream swe_in(argv[1]); //swe=singlewordextract
	std::string line;

	while ( getline(swe_in,line) ) {
		dc.read_line_singlewordExtract(line);
	}
	aStar::Suchalgorithmus(dc,argv[2]);
}
