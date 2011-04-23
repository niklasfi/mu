#include "main.h"
#include <string>

using namespace std;

int main()
{
	Word e(Word::english);

	std::cout << "e:\n\t";
	e.operator<<(std::cout) << "\n";
	
	Lexicon l(Word::english);
	
	l.getWord("hello").operator<<(std::cout << "hello not there\n\t") << "\n";
	
	l.add("hello") = e;
	
	l.getWord("hello").operator<<(std::cout << "hello there\n\t") << "\n";
	
	char* katze = "cat";
	
	Word kwort = l.add(katze);
	
	l.add(katze="mouse");
	
	l.add(katze="dog");
	
	std::cout << "katze:\n\t" << l.getString(kwort) << "\n";
	
	
}

