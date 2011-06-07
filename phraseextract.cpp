#include "global.h"
#include "language.h"
#include "lexicon.h"
#include "word.h"
#include "ptree.h"

#include "gzstream/gzstream.h"

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 

int main(int argc, char* argv[]) {
	
	if (argc != 4) {
		std::cout << "Aufruf mit Parametern: <französiche Trainigsdaten> <englische Trainingsdaten> <Alignment der Trainingsdaten>";
		return 0;
	}
	Lexicon flex(french);
	Lexicon elex(english);
	PTree<std::pair<int, PTree<int> > > pTree;
	
	igzstream f_in(argv[1]), e_in(argv[2]), a_in(argv[3]);
	std::string f_line, e_line, a_line;

	while (getline(f_in, f_line) && getline(e_in, e_line)) {
		
		/*==========Lies Wörter beider Sätze, sowie zugehörige Alignments aus entsprechenden Dateien aus==========*/
		std::string token;
		std:: istringstream f_ist(f_line), e_ist(e_line);
		std::vector<std::pair<uint, std::vector<int> > > f_vec, e_vec;	//Speichern alle Wörter jeweiliger Sprache und ihre Alignments
		
		//Füge alle französichen Wörter in ein Lexicon ein und schreibe ihre IDs in einen Vektor
		while(f_ist >> token) {
			uint id = flex.getWord_or_add(token);
			std::pair<uint, std::vector<int> > pair_tmp;
			pair_tmp.first = id;
			f_vec.push_back(pair_tmp);
		}

		//Füge alle englischen Wörter in ein Lexicon ein und schreibe ihre IDs in einen Vektor
		while (e_ist >> token) {
			uint id = elex.getWord_or_add(token);
			std::pair<uint, std::vector<int> > pair_tmp;
			pair_tmp.first = id;
			f_vec.push_back(pair_tmp);
		}
		
		getline(a_in, a_line);	//"SEND:" abfangen
		do {
			getline(a_in, a_line);
			if(a_line == "") break;	//Alignment eine Satzes zu Ende
			else {
				std::istringstream a_ist(a_line);
				int f_ind, e_ind;
				std::string s;
				a_ist >> s >> f_ind >> e_ind;
				f_vec[f_ind].second.push_back(e_ind);	//Speichere einzelnes Alignment in f_vec
				e_vec[e_ind].second.push_back(f_ind);	//Speichere einzelnes Alignment in e_vec
			}
		} while(true);

		/*==========Beide Sätze liegen nun in Vektoren gespeichert vor, die Alignments jedes Wortes sind in einem Vektor gespeichert==========
		 *==========Führe darauf nun den vorgegebenen Algorithmus aus, um alle Phrasen zu finden und im Präfixbaum zu speichern==========*/
		
		for(int j1 = 0; j1 < f_vec.size(); j1++)
			for(int j2 = j1; j2 < std::min(j1+3, (int) f_vec.size()); j2++) {	//Länge der Quellphrase maximal 3
				int i1 = f_vec[j1].second.front();
				int i2 = f_vec[j1].second.back();
				for(int k = j1+1; k <= j2; k++) {
					i1 = std::min(i1, f_vec[k].second.front());	//Minimales Alignment innerhalb der Phrase finden => i1
					i2 = std::max(i2, f_vec[k].second.back());	//Maximales Alignment innerhalb der Phrase finden => i2
				}

				if(j1 == j2) {	//Einzelwortphrasen auf Quellseite werden IMMER extrahiert
					
					std::vector<uint> f_vec_tmp, e_vec_tmp;
					for (int k = j1; k <= j2; k++)
						f_vec_tmp.push_back(f_vec[k].first);	//Quellphrase in Vektor zusammenstellen
					for (int k = i1; k <= i2; k++)
						e_vec_tmp.push_back(e_vec[k].first);	//Zielphrase in Vektor zusammenstellen

					std::pair<int,PTree<int> >* scontent = &pTree.traverse(f_vec_tmp,true,std::pair<int,PTree<int>>(0,PTree<int>()))->c;
					scontent->first++;
					scontent->second.traverse(e_vec_tmp,true,0)->c++;

					/*
					
					das da oben sollte eine kurze Version von dem hier sein:

					PTree<std::pair<int, PTree<int> > >* tree_tmp1 = new PTree<std::pair<int, PTree<int> > >();
					PTree<int>* tree_tmp2 = new PTree<int>();
					tree_tmp1 = pTree.traverse(f_vec_tmp);			//Quellphrase in Baum einfügen
					tree_tmp1 -> c.first++;					//Zähler für Quellphrase um eins erhöhen
					tree_tmp2 = tree_tmp1 -> c.second.traverse(e_vec_tmp);	//Zielphrase in "Unter-Baum" einfügen
					tree_tmp2 -> c++;						//Zähler für Zielphrase um eins erhöhen
					delete tree_tmp1;
					delete tree_tmp2;
					*/

				} else if (i2-i1 < 4) {	//Länge der Zielphrase maximal 4

					int j1_test = e_vec[i1].second.front();
					int j2_test = e_vec[i1].second.back();
					for (int k = i1+1; k <= i2; k++) {
						j1 = std::min(j1, e_vec[k].second.front());
						j2 = std::max(j2, e_vec[k].second.back());
					}

					if ((j1_test >= j1) && (j2_test <= j2)) {	//Phrasen, die den Test bestehen, werden extrahiert
						
						std::vector<uint> f_vec_tmp, e_vec_tmp;
						for (int k = j1; k <= j2; k++)
							f_vec_tmp.push_back(f_vec[k].first);
						for (int k = i1; k <= i2; k++)
							e_vec_tmp.push_back(e_vec[k].first);
						PTree<std::pair<int, PTree<int> > >* tree_tmp1 = new PTree<std::pair<int, PTree<int> > >();
						PTree<int>* tree_tmp2 = new PTree<int>();
						tree_tmp1 = pTree.traverse(f_vec_tmp);			//Quellphrase in Baum einfügen
						tree_tmp1 -> c.first++;					//Zähler für Quellphrase um eins erhöhen
						tree_tmp2 = tree_tmp1 -> c.second.traverse(e_vec_tmp);	//Zielphrase in "Unter-Baum" einfügen
						tree_tmp2 -> c++;						//Zähler für Zielphrase um eins erhöhen
						delete tree_tmp1;
						delete tree_tmp2;
					}
				}
			}
			/*==========Jetzt sind alle erlaubten Phrasen aus diesem Satzpaar im Präfixbaum gespeichert==========*/
			/*==========Damit ist die Bearbeitung dieses Satzpaares abgeschlossen und das nächste rückt nach==========*/
	}
	/*==========Nun sind alle erlaubten Phrasen aller Satzpaare - also der gesamten Testdaten - im Präfixbaum gespeichert==========*/
	/*==========Im Anschluss muss also der Präfixbaum in eine Phrasentabelle ausgegeben werden==========*/

	for (PTree<std::pair<int, PTree<int> > >::iterator itor1 = pTree.begin(); itor1 != pTree.end(); itor1++) {	//Durchlaufe den Baum

		int source_count = itor1 -> c.first;		//Zähler für Quellphrase auslesen
		std::vector<uint> source_id = itor1 -> phrase();//Quellphrase (in IDs) auslesen
		std::string source_phrase = "";
		for (int k = 0; k < source_id.size(); k++)	//ID-Phrase in Stringphrase umwandeln
			source_phrase += flex.getString(source_id[k]) + " ";
		
		for(PTree<int>::iterator itor2 = itor1 -> c.second.begin(); itor2 != itor1 -> c.second.end(); itor2++) {	//Durchlaufe den "Unter-Baum"

			int target_count = itor2 -> c;			//Zähler für Zielphrase auslesen
			std::vector<uint> target_id = itor2 -> phrase();//Zielphrase (in IDs) auslesen
			std::string target_phrase = "";
			for (int k = 0; k < target_id.size(); k++)	//ID-Phrase in Stringphrase umwandeln
				source_phrase += elex.getString(target_id[k]) + " ";

			double relFreq = log(source_count) - log(target_count);	//Bestimmen der relativen Wahrscheinlichkeit (negativer Logarithmus)
			std::cout << relFreq << " # " << source_phrase << " # " << target_phrase << "\n";	//Ausgabe
		}
	}
	return 0;	
}
