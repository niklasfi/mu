#include "singlewordextract.h"
#include <string>
#include <sstream>

int main(int argc, char*argv[])
{
	Dictionary d;
	
	igzstream f_in(argv[1]), e_in(argv[2]), a_in(argv[3]);
	std::string f_line, e_line, a_line;

	while ( getline(f_in,f_line) && getline(e_in,e_line)) {
		std::string token;
		std::istringstream f_ist(f_line), e_ist(e_line);
		std::vector<std::string> f_vec, e_vec;
		
		while ( f_ist >> token )
			f_vec.push_back(token);
		while ( e_ist >> token)
			e_vec.push_back(token);
		getline(a_in, a_line); //truncate SENT: ... line;
		/*std::cout << a_line << "\n";
		std::cout << "f: " << f_line << "\n";
		std::cout << "e: " << e_line << "\n";*/
		do{
			getline(a_in, a_line);
			
			if(a_line == "") break;
			else{
				std::istringstream a_ist(a_line);
				uint f_ind, e_ind;
				std::string s;
				a_ist >> s >> f_ind >> e_ind;
				//std::cout << "f: " << f_vec[f_ind] << ", e: " << e_vec[e_ind] << "\n";
				d.add(f_vec[f_ind],e_vec[e_ind]);
			}
			
		}while(true);
		
		//std::cout << "\n--- next sentence ---\n";
	}
	d.printall();
}
