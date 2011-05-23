#include "global.h"
#include <iostream>
#include "sentencepool.h"
#include <string>
#include "bleu.cpp"
#include "PER_WER.cpp"
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, const char* argv[]){
	if (argc != 3){
			cout << "Usage: <ref-Uebersetzung> <Hypothese>" << endl;
			return EXIT_FAILURE;
	}


	SentencePool sp;
	ifstream ref(argv[1]);
	ifstream guess(argv[2]);

	string line;
	while(getline(ref,line)){
		sp.addSentence(SentencePool::ref,line);
	}
	while(getline(guess,line)){
		sp.addSentence(SentencePool::gue,line);
	}
	if(sp.reference.size() != sp.guess.size()){
			cout << "Es gibt unterschiedlich viele Zeilen der Hypothesen und Referenzen!" << endl;
			return EXIT_FAILURE;
	}
	cout << "PER für die Übersetzung " << PER_global(sp) << endl;
	cout << "WER für die Übersetzung " << WER_global(sp) << endl;
	for (int i=1; i<5; i++){
		cout << "Bleu-Score für die Übersetzung mit N= "<< i << " " << bleu(sp,i) << endl;
	}
}
