#include "global.h"
#include "language.h"
#include "lexicon.h"
#include "word.h"
#include "ptree.h"
#include "wordinfo.h"

#include "gzstream/gzstream.h"

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 
#include <unordered_map>

int main(int argc, char* argv[]) {
	
	if (argc != 4) {
		std::cout << "Aufruf mit Parametern: <französiche Trainigsdaten> <englische Trainingsdaten> <Alignment der Trainingsdaten>\n"
			<< "Folgende Ausgabe: relfreq_f relfreq_e # quellphrase # zielphrase # singlecf singlece # source_to_target target_to_source # unigram-sprachmodell\n";
		return 0;
	}
	Lexicon flex(french);
	Lexicon elex(english);
	PTree<std::pair<int, PTree<int> > > pTree;
	PTree<unsigned int> eSinglecount;

	uint eCount = 0;				//Gesamtzahl der englischen Wörter

	std::unordered_map<uint,Wordinfo> ef_pair, fe_pair;	//Einzelwortbasierte Übersetzungshäufigkeit von e nach f (und umgekehrt)
	
	igzstream f_in(argv[1]), e_in(argv[2]), a_in(argv[3]);
	std::string f_line, e_line, a_line;

	while (getline(f_in, f_line) && getline(e_in, e_line)) {
		
		/*==========Lies Wörter beider Sätze, sowie zugehörige Alignments aus entsprechenden Dateien aus==========*/
		std::string token;
		std:: istringstream f_ist(f_line), e_ist(e_line);
		std::vector<std::pair<uint, std::vector<unsigned int> > > f_vec, e_vec;	//Speichern alle Wörter jeweiliger Sprache und ihre Alignments
		
		//Füge alle französichen Wörter in ein Lexicon ein und schreibe ihre IDs in einen Vektor
		while(f_ist >> token) {
			uint id = flex.getWord_or_add(token);
			std::pair<uint, std::vector<unsigned int> > pair_tmp;
			pair_tmp.first = id;
			f_vec.push_back(pair_tmp);
		}

		//Füge alle englischen Wörter in ein Lexicon ein und schreibe ihre IDs in einen Vektor
		while (e_ist >> token) {
			uint id = elex.getWord_or_add(token);
			std::pair<uint, std::vector<unsigned int> > pair_tmp;
			pair_tmp.first = id;
			e_vec.push_back(pair_tmp);
			eCount++;
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

				uint& f_id = f_vec[f_ind].first, e_id = e_vec[e_ind].first;
				fe_pair[f_id].pairs[e_id]++;		//Paircount für f nach e erhöhen
				fe_pair[f_id].singlecount++;		//Singlecount für f nach e erhöhen
				ef_pair[e_id].pairs[f_id]++;		//Paircount für e nach f erhöhen
				ef_pair[e_id].singlecount++;		//Singlecount für e nach f erhöhen
			}
		} while(true);

		/*==========Beide Sätze liegen nun in Vektoren gespeichert vor, die Alignments jedes Wortes sind in einem Vektor gespeichert==========
		 *==========Führe darauf nun den vorgegebenen Algorithmus aus, um alle Phrasen zu finden und im Präfixbaum zu speichern==========*/
		
		for(unsigned int j1 = 0; j1 < f_vec.size(); j1++)
			for(unsigned int j2 = j1; j2 < std::min(j1+3,(unsigned int)f_vec.size()); j2++) {	//Länge der Quellphrase maximal 3
				unsigned int i1, i2;
				bool set_i = false;						//hält mit, ob i1 und i2 gesetzt wurden, oder nicht.
				for(unsigned int k = j1; k <= j2; k++) 
					if(f_vec[k].second.size() && set_i) {
						i1 = std::min(i1, f_vec[k].second.front());	//Minimales Alignment innerhalb der Phrase finden => i1
						i2 = std::max(i2, f_vec[k].second.back());	//Maximales Alignment innerhalb der Phrase finden => i2
					} else if(f_vec[k].second.size() && !(set_i)) {
						i1 = f_vec[k].second.front();
						i2 = f_vec[k].second.back();
						set_i = true;
					}

				if (set_i){ 					//leere Phrasen werden nicht geprüft sondern direkt verworfen
					if(j1 == j2) {					//Einzelwortphrasen auf Quellseite werden IMMER extrahiert
						
						std::vector<uint> f_vec_tmp, e_vec_tmp;
						for (unsigned int k = j1; k <= j2; k++)
							f_vec_tmp.push_back(f_vec[k].first);	//Quellphrase in Vektor zusammenstellen
						for (unsigned int k = i1; k <= i2; k++)
							e_vec_tmp.push_back(e_vec[k].first);	//Zielphrase in Vektor zusammenstellen
						std::pair<int, PTree<int> > pair_tmp;
						pair_tmp.first = 0;
						pTree.traverse(f_vec_tmp,true,pair_tmp)->c.first++;				//Quellphrase in Baum einfügen
						pTree.traverse(f_vec_tmp,false)->c.second.traverse(e_vec_tmp,true,0)->c++;	//Zielphrase in "Unter-Baum" einfügen
						eSinglecount.traverse(e_vec_tmp,true,0)->c++;
					} else if (i2-i1 < 4) {	//Länge der Zielphrase maximal 4

						unsigned int j1_test, j2_test;
						bool set_j_test = false;			//hält mit, ob j1_test und j2_test gesetzt wurden, oder nicht
						for (unsigned int k = i1; k <= i2; k++)
							if (e_vec[k].second.size() && set_j_test) {
								j1_test = std::min(j1_test, e_vec[k].second.front());
								j2_test = std::max(j2_test, e_vec[k].second.back());
							} else if (e_vec[k].second.size() && !(set_j_test)) {
								j1_test = e_vec[k].second.front();
								j2_test = e_vec[k].second.back();
								set_j_test = true;
							}
						
						if (set_j_test)					//leere Phrasen werden nicht geprüft sondern sofort verworfen
						if ((j1_test >= j1) && (j2_test <= j2)) {	//Phrasen, die den Test bestehen, werden extrahiert
							
							std::vector<uint> f_vec_tmp, e_vec_tmp;
							for (unsigned int k = j1; k <= j2; k++)
								f_vec_tmp.push_back(f_vec[k].first);
							for (unsigned int k = i1; k <= i2; k++)
								e_vec_tmp.push_back(e_vec[k].first);
							std::pair<int, PTree<int> > pair_tmp;
							pair_tmp.first = 0;
							pTree.traverse(f_vec_tmp,true,pair_tmp)->c.first++;			//Quellphrase in Baum einfügen
							pTree.traverse(f_vec_tmp,false)->c.second.traverse(e_vec_tmp,true,0)->c++;	//Zielphrase in "Unter-Baum" einfügen
							eSinglecount.traverse(e_vec_tmp,true,0)->c++;
						}
					}
				}
				
			}
			/*==========Jetzt sind alle erlaubten Phrasen aus diesem Satzpaar im Präfixbaum gespeichert==========*/
			/*==========Damit ist die Bearbeitung dieses Satzpaares abgeschlossen und das nächste rückt nach==========*/
	}
	/*==========Nun sind alle erlaubten Phrasen aller Satzpaare - also der gesamten Testdaten - im Präfixbaum gespeichert==========*/
	/*==========Im Anschluss muss also der Präfixbaum in eine Phrasentabelle ausgegeben werden==========*/

	for (PTree<std::pair<int, PTree<int> > >::iterator itor1 = pTree.begin(); itor1 != pTree.end(); itor1++) {	//Durchlaufe den Baum

		int singlecount_f = (&*itor1) -> c.first;		//Zähler für Quellphrase auslesen
		if (singlecount_f) {
			std::vector<uint> source_id = (&*itor1) -> phrase();//Quellphrase (in IDs) auslesen
			std::string source_phrase = "";
			for (unsigned int k = 0; k < source_id.size(); k++)	//ID-Phrase in Stringphrase umwandeln
				source_phrase += flex.getString(source_id[k]) + " ";
			
			for(PTree<int>::iterator itor2 = (&*itor1) -> c.second.begin(); itor2 != (&*itor1) -> c.second.end(); itor2++) {	//Durchlaufe den "Unter-Baum"

				int paircount = (&*itor2) -> c;			//Zähler für Zielphrase auslesen
				if(paircount != 0) {
					std::vector<uint> target_id = (&*itor2) -> phrase();//Zielphrase (in IDs) auslesen
					std::string target_phrase = "";
					for (unsigned int k = 0; k < target_id.size(); k++)	//ID-Phrase in Stringphrase umwandeln
						target_phrase += elex.getString(target_id[k]) + " ";

					double source_to_target = 1;
					for (unsigned int k = 0; k < target_id.size(); k++) {
						double  sum_stt = 0;
						for (unsigned int l = 0; l < source_id.size(); l++) {
							sum_stt += (double) fe_pair[source_id[l]].pairs[target_id[k]] / (double) fe_pair[source_id[l]].singlecount;
						}
						source_to_target *= sum_stt / source_id.size();
					}
					source_to_target = -log(source_to_target);

					double target_to_source = 1;
					for (unsigned int k = 0; k < source_id.size(); k++) {
						double sum_tts = 0;
						for (unsigned int l = 0; l < target_id.size(); l++) {
							sum_tts += (double) ef_pair[target_id[l]].pairs[source_id[k]] /  (double) ef_pair[target_id[l]].singlecount;
						}
						target_to_source *= sum_tts / target_id.size();
					}
					target_to_source = -log(target_to_source);

					uint singlecount_e = eSinglecount.traverse(target_id)->c;
					double relFreqF = log(singlecount_f) - log(paircount);	//Bestimmen der relativen Wahrscheinlichkeit (negativer Logarithmus)
					double relFreqE = log(singlecount_e) - log(paircount);
					
					double unigram = log(eCount) - log(singlecount_e);
					
					std::cout << relFreqF << " " << relFreqE << " # " << source_phrase << "# " << target_phrase << "# " << singlecount_f << " "<< singlecount_e << " # " << source_to_target << " " << target_to_source << " # " << unigram << "\n";	//Ausgabe
				}
			}
		}
	}
	return 0;	
}
