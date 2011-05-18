#include "levenshtein.h"

using namespace std;

int Levenshtein:: min(int i, int j){ //gibt den Wert für i,j zurück
	
	int min=matrix[i-1][j-1]+1;//Kosten für substituieren
	
	if (matrix[i-1][j]+1 < min)	min = matrix[i-1][j]+1; //Deletion
	if (matrix[i][j-1]+1 < min)	min = matrix[i][j-1]+1; //Insertion
	return min;
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
		for (int j=1; j<_width; j++){
			if((*reference)[i-1]==(*guess)[j-1])	matrix[i][j]=matrix[i-1][j-1]; //die Wörter entsprechen sich schon			
			else	matrix[i][j]=min(i,j);		//füllt die Matrix auf
		}
	
}

Levenshtein::~Levenshtein(){
	delete[] data;
	delete[] matrix;
}

//void Levenshtein::print(Lexicon* elex){
//}

int Levenshtein::distance(){
  return matrix[_height-1][_width-1];
}

