#include "dictionaryc.h"
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char*argv[])
{
	DictionaryC dc;
	
	std::ifstream swe_in(argv[1]); //swe=singlewordextract
	std::string line;

	while ( getline(swe_in,line) ) {
		dc.read_line_singlewordExtract(line);
	}
	dc.printall();
}
