#include "dictionaryc.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "ui2.h"
#include "aStar.h"

int main(int argc, char*argv[])
{
	if(argc != 5){
		std::cerr << "usage: <pruneAfter> <sentenceCount> <locationSingleWordExtract> <locationOrigin>\n";
		exit(1);
	}
	
	uint pruneAfter = ui2::readarg<uint>(argv[1],"error reading argument pruneAfter");
	uint sentenceCount = ui2::readarg<uint>(argv[2],"error reading argument sentenceCount");

	WordinfoC::set_maxlen(pruneAfter);
	aStar::set_max_SentenceTranslation(sentenceCount);
	DictionaryC dc;

	std::ifstream swe_in(argv[3]); //swe=singlewordextract
	std::string line;

	while ( getline(swe_in,line) ) {
		dc.read_line_singlewordExtract(line);
	}
	aStar::Suchalgorithmus(dc,argv[4]);
}
