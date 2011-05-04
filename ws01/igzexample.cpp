#pragma comment(lib, "yourlib.lib");

#include "gzstream/gzstream.h"
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char*argv[])
{
	igzstream f_in("training/f");
	string line, token;

	while ( getline(f_in,line) ) {
		istringstream ist(line);
		//cout << "<line>" << endl;
		while ( ist >> token ) {
			cout << token << endl;
		}
		//cout << "</line>" << endl;
		return 0;
	}
}

