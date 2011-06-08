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
	 bool phrasePenalty = false, wordPenalty = false, singleCountBit = false, stRatio = false; //stRatio =Source-Target-Ratio 

     PTree<PTree<double>> schwarz;

		if(argc<3){
		cout<<"usage: template.exe <phrasentabelle> <quelltext> [pwsr]\n";
		exit(1);
	}

	for(int i=3; i<argc;i++){
		std::string in = argv[i];
		if     (in == "p") phrasePenalty=1;
		else if(in == "w") wordPenalty = 1;
		else if(in == "s") singleCountBit = 1;
		else if(in == "r") stRatio = 1;
		else{
			cout << "invalid argument!\n";
			exit (1);
		}
	}

     igzstream in(argv[1]);
     string line,token;



		while(getline(in,line)){
			stringstream ist(line);


			double relfreq;
			vector<uint> ephrase, fphrase;

			ist >> relfreq  >> token  >>token; // token für "#" ACHTUNG das zweite Einlesen ist nur für die alten Trainingsdaten,da gibts relfreq zweimal

			while(ist>>token && token != "#"){
				fphrase.push_back(flex.getWord_or_add(token));
			}
			while(ist>>token && token != "#"){
				ephrase.push_back(elex.getWord_or_add(token));
			}
			if(phrasePenalty) relfreq+=1;
			if(wordPenalty) relfreq+= ephrase.size();
			if(singleCountBit){
				int singleCount;
				ist>>token>>singleCount;
				if(singleCount>1) relfreq+=1;
			}
			if(stRatio) relfreq+=(((double)fphrase.size())/((double)ephrase.size()));

			schwarz.traverse(fphrase,true,(1./0.))->c.traverse(ephrase,true,(1./0.))->c = relfreq;

		}

     aStar::set_max_SentenceTranslation(1);
     aStar::Suchalgorithmus(argv[2],&schwarz,&elex,&flex);
}
