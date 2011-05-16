#include <vector>
#include <iostream>
#include "lexicon.h"


class Levenshtein{

private:
	int _height;
	int _width;

	int max(int i, int j);

public:
	unsigned int* data;
	unsigned int** matrix;
	
	Levenshtein(std::vector<int>* reference, std::vector<int>* guess);//Eingabe sind die Referenzübersetzung und die zu bewertende Übersetzung, allerdings in ID's 
	~Levenshtein();
	
	void print(Lexicon* elex);//gibt die nötigen Veränderungen von guess auf reference aus.

	int distance(); //gibt die Levenshtein-Distance aus
};
