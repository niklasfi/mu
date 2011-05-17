#include "levenshtein.h"

using namespace std;

int Levenshtein:: max(int i, int j){ //gibt den Wert für i,j zurück
	
	int max=matrix[i-1][j-1]+1;//Kosten für substituieren
	
	if (matrix[i-1][j]+1 > max)	max = matrix[i-1][j]; //Deletion
	if (matrix[i][j-1]+1 > max)	max = matrix[i][j-1]; //Insertion
	return max;
}


Levenshtein::Levenshtein(vector<int>* reference, vector<int>* guess){
	_height=reference->size()+1;
	_width=guess->size()+1;	

	data = new unsigned int [_height*_width];
	matrix=new unsigned int*[_height];
	for(int i=0; i<_height; i++)	matrix[i]=data+i*_width;

	for(int i=0; i<_height; i++)	matrix[i][0]=i; //triviale Veränderungen hinzufügen
	for(int j=0; j<_width; j++)		matrix[0][j]=j;

	for (int i=1; i<_height; i++)
		for (int j=1; i<_width; j++){
			if((*reference)[i]==(*guess)[i])	matrix[i][j]=matrix[i-1][j-1]; //die Wörter entsprechen sich schon			
			else	matrix[i][j]=max(i,j);		//füllt die Matrix auf
		}
}

Levenshtein::~Levenshtein(){
	delete[] data;
	delete[] matrix;
}

void Levenshtein::print(Lexicon* elex){
}

int Levenshtein::distance(){
  return matrix[_height][_width];
}

