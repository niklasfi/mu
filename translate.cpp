#include <vector>
#include <map>
#include <iostream>
#include <ifstream>
#include <string>

#include "lexicon.h"
#include "gzstream/gzstream.h"
#include "global.h"
#include "ptree.h"

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
     
     stringstream ist;
     
     while(getline(in,line)){ 
	  ist(line);
	  
	  double relfreq;
	  vector<uint> ephrase, fphrase;
	  
	  ist >> relfreq >> token; // token für "#"
	  
	  while(ist>>token && token != "#"){
	      fphrase.push_back(flex.getWord_or_add(token));
	  }
	  while(ist>>token){
	      ephrase.push_back(elex.getWord_or_add(token));
	  }
	  
	  schwarz.traverse(fphrase,true)->contents.traverse(ephrase,true)->contents = relfreq;
     }
     
     
     
     
}