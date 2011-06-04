#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "lexicon.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "ptree.h"
#include "aStar.h"

using namespace std;

int main (int argc, char* argv[]){
     Lexicon elex;
     Lexicon flex;
     
     PTree<PTree<double>> schwarz;
     
     if(argc!=3){
	  cout<<"usage: template.exe <phrasentabelle> <quelltext>\n";
	  exit(1);
     }
     
     igzstream in(argv[1]);
     string line,token;
     
     
     
     while(getline(in,line)){ 
	  stringstream ist(line);
	  
	  double relfreq;
	  vector<uint> ephrase, fphrase;
	  
	  ist >> relfreq >> token >>token; // token für "#" ACHTUNG das zweite Einlesen ist nur für die alten Trainingsdaten,da gibts relfreq zweimal
	  
	  while(ist>>token && token != "#"){
	      fphrase.push_back(flex.getWord_or_add(token));
	  }
	  while(ist>>token){
	      ephrase.push_back(elex.getWord_or_add(token));
	  }
	  
	  PTree<double>* blau=new PTree<double>(); //wir alloquieren speicher für den blauBaum 
	  blau->traverse(ephrase,true)->c = relfreq; 
	  schwarz.traverse(fphrase,true)->c=*blau; //und befüllen ihn mit informationen
	  
	  //cout << "content im blaubaum "<< blau.traverse(ephrase)->c << endl;
     }
     
     aStar::Suchalgorithmus(argv[2],&schwarz,&elex,&flex);
     
     
     
     
}
