#include "levenshtein.h"

using namespace std;

int Levenshtein:: min(int i, int j){ //gibt den Wert für i,j zurück
	
	unsigned int min=matrix[i-1][j-1]+1;//Kosten für substituieren
	
	if (matrix[i-1][j]+1 < min)	min = matrix[i-1][j]+1; //Deletion
	if (matrix[i][j-1]+1 < min)	min = matrix[i][j-1]+1; //Insertion
	return min;
}

void Levenshtein:: back(int& i, int& j, Lexicon* elex){ //gibt den nächsten Schritt im Rückweg an
	if (i ==0){
	       j--;
	       cout << "Löschung von  " << elex->getString((*guess)[j]) << endl;
     }	
	else{
	     if (j==0){
		i--;
		cout << "Einfügen von  " << elex->getString((*reference)[i]) << endl;
	     }
	     else{
		  bool insertion=true; //gibt an, ob es sich um ein Einfügen handelt
		  i--; //betrachte die Zelle über dem akt. Standpunkt
		  if (matrix[i][j-1] < matrix[i][j]){  //die Zelle schräg über dem akt. Standpunkt ist besser, d.h. Substitution
		       j--; 
		       insertion=false;
		       if(matrix[i+1][j] < matrix[i][j]){ //Löschen ist am besten, akt=Substition
			      i++;
			      cout << "Löschung von " << elex->getString((*guess)[j]) << endl;
			}
			else //Substituieren ist am besten
			     if ((*guess)[j] == (*reference)[i])	cout << "Passend bei " << elex->getString((*guess)[j])<<endl;
			     else	cout << "Substituieren von " << elex->getString((*guess)[j]) << " durch " << elex->getString((*reference)[i])<< endl;
		  }
		  if (matrix[i+1][j-1] < matrix[i][j]){ //die Zelle links ist besser (akt pos. ist einfügen), also Löschen
			i++;
			j--;
			insertion=false;
			cout << "Löschung von " << elex->getString((*guess)[j]) << endl;
		  }
		  if (insertion)	cout << "Einfügen von " << elex->getString((*reference)[i])<< endl;
		       
	       }
	}
}       

Levenshtein::Levenshtein(vector<unsigned int>* reference, vector<unsigned int>* guess){
	this->reference=reference;
	this->guess=guess;
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

void Levenshtein::print(Lexicon* elex){
	int i=_height-1, j=_width-1;
	while(i!=0 || j != 0){
	  back(i, j, elex);
	}
}

int Levenshtein::distance(){
  return matrix[_height-1][_width-1];
}

