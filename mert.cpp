#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>

#include "cost.h"
#include "sentenceinfo.h"
#include "bleu.cpp"
#include "global.h"

/* === Strukturtyp StraightLine, um Geraden darzustellen === */
struct StraightLine {
	double gradient;	//Steigung
	double offset;		//Achsen-Abstand
	uint sentence;		//Position der entsprechenden Hypothese
};

/* === Strukturtyp Section, um Schnittpunkt und den im folgenden benutzten Satz zu speichern === */
struct Section {
	double intersection;	//Beginn der Section (also vorderer Schnittpunkt)
	uint sentence;		//Position der entsprechenden Hypothese
};

/* === wie Section, speichert aber nicht nur Position einer Hypothese, sondern mehrere === */
struct Global_Section {
	double begin;		//Beginn der Global_Section (also vorderer Schnittpunkt)
	std::vector<uint> sentences;	//Position der entsprechenden Hypothesen in richtiger Reihenfolge
};



/* ===== HILFSFUNKTIONEN ===== */

/* === Gibt eine zufällige Permutation der Zahlen 0 bis (modelNumber-1) zurück === */
std::list<int> randomPermutation(const int &modelNumber) {

	std::list<int> permutation;								//Spätere Rückgabeliste
	std::vector<int> vector_tmp;								//Hilfsvektor
	for (int index = 0; index < modelNumber; index++) vector_tmp.push_back(index);		//der die Zahlen 0 bis (modelnumber-1) enthält

	for (int index = 0; index < modelNumber; index++) {
		//int randomNumber = rand() % vector_tmp.size();					//Bestimme zufällige Zahl in vector_tmp
		int integer_tmp = vector_tmp[index];						//Ließ die Zahl aus
		permutation.push_back(integer_tmp);						//Füge sie ans Ende der Permutation ein			
		vector_tmp.erase(vector_tmp.begin()+index);					//Lösche sie aus dem Vektor
	}

	return permutation;
}


/* === Liefert einen Vector zurück, der nur die ersten modelNumber Modelle enthält === */
std::vector<Cost::Model> selectModels (const int &modelNumber) {

	std::vector<Cost::Model> selectedModels;							//Späterer Rückgabevektor

	for (int index = 0; index < modelNumber; index++)
		switch (index) {								//Wähle Modell gemäß Reihenfolge
			case 0:	selectedModels.push_back(Cost::source_to_target_phrase);
			case 1: selectedModels.push_back(Cost::target_to_source_phrase);
			case 2: selectedModels.push_back(Cost::source_to_target_unigram);
			case 3: selectedModels.push_back(Cost::target_to_source_unigram);
			case 4: selectedModels.push_back(Cost::phrase_penalty);
			case 5: selectedModels.push_back(Cost::word_penalty);
			case 6: selectedModels.push_back(Cost::single_count_bit);
			case 7: selectedModels.push_back(Cost::source_to_target_ratio);
			case 8: selectedModels.push_back(Cost::unigram_language_model);
		}
	
	return selectedModels;
}


/* === Gibt die Position der Gerade mit der gesuchten Steigung an. Existiert keine solche gibt die Funktion die Position der Geraden mit nächsthöherer oder nächstniedrigerer Steigung zurück === */
int searchLines(const std::deque<StraightLine> &lines, const double &key, const int &left, const int &right, const int &lastMid) {

	if (left > right) return lastMid;						//Wenn Binärsuche key nicht findet, gib letzte Mitte zurück
	int mid = (int) floor( ( (left+right)/2)+0.5);					//Bestimme Mitte der Grenzen left und right
	if (lines[mid].gradient == key) return mid;					//Key gefunden, gib Position zurück
	else if (lines[mid].gradient > key) return searchLines(lines, key, left, mid-1, mid);	//Rekursiver Aufruf
	else /*(lines[mid].gradient < key)*/ return searchLines(lines, key, mid+1, right, mid);	//Rekursiver Aufruf
}


/* === Liefert den Wert des zu currentParam gehörenden Modells zurück zurück === */
double findGradient ( Cost &cost, const int &currentParam, const std::vector<Cost::Model> &models) {

	/*std::vector<Model> model_tmp;							//Enthält das gesuchte Modell
	model_tmp.push_back(models[currentParam]);					//Bestimmen des zum aktuellen Parameter gehörenden Modell
	std::vector<double> gradient_tmp = cost.cost(model_tmp);			//Auslesen des zugehörigen Modellwertes aus cost
	return gradient_tmp.front();*/
	Cost::Model model_tmp = models[currentParam];
	double value_tmp = cost.cost(model_tmp);
	return value_tmp;
}


/* === Gibt die Summe aller gewichteten Werte der nicht zu currentParam gehörenden Modelle zurück === */
double findOffset (Cost &cost, const int &currentParam, const std::vector<Cost::Model> &models, const int &modelNumber) {
	
	double offset = 0;								//Gesamtkosten für alle Modelle bis auf eines
	for (int index = 0; index < modelNumber; index++) 

		if(!(index == currentParam)) {						//Zum aktuellen Parameter gehörendes Modell nicht dazurechnen
			double scale_tmp = cost.scale[models[index]];			//Skalierung auslesen
			std::vector<Cost::Model> model_tmp;					//Zu index gehörendes Modell auslesen
			model_tmp.push_back(models[index]);
				std::vector<double> value_tmp = cost.cost(model_tmp);			//Zugehörigen Modellwert auslesen
				offset += value_tmp.front() * scale_tmp;			//Offset um Produkt erhöhen
		}

	return offset;
}


/* === Liefert die für MERT nötigen Schnittpunkte und die zugehörigen Geraden zurück === */
std::deque<Section> findSections (const std::deque<StraightLine> &lines) {

	std::deque<Section> sections;							//Enthält später alle Sections

	Section section_tmp;								//Hilfs-Section
	section_tmp.intersection = 0;
	section_tmp.sentence = lines.front().sentence;
	
	sections.push_back(section_tmp);						//Erste Section ist zu Beginn festgelegt

	unsigned int index1 = 0;
	while (index1 != lines.size()-1) {						//Brich bei Erreichen der letzten Gerade ab

		double maxIntersection = 0;						//Speichert den Schnittpunkt mit höchstem y-Wert
		unsigned int index_tmp = index1;							//Merkt sich die zugehörige Gerade

		for (unsigned int index2 = index1+1; index2 < lines.size(); index2++) {

			double intersection_tmp = (lines[index2].offset - lines[index1].offset) / (lines[index1].gradient - lines[index2].gradient);
			double yValue_tmp = lines[index1].gradient * intersection_tmp + lines[index1].offset;
			double yValueMax = lines[index1].gradient * maxIntersection + lines[index1].offset;
			if (yValue_tmp > yValueMax) {					//Bestimme den höchsten Schnittpunkt
				maxIntersection = intersection_tmp;			//Speichere diesen
				index_tmp = index2;					//Und die zugehörige Gerade
			}
		}

		if (index_tmp != index1) {						//Wenn überhaupt ein Schnittpunkt gefunden wurde
			section_tmp.intersection = maxIntersection;
			section_tmp.sentence = lines[index_tmp].sentence;
			sections.push_back(section_tmp);				//Füge neu begonnene Section zum Vektor hinzu
			index1 = index_tmp;						//Fahre mit Gerade des aktuellen Schnittpunktes fort
		}
	}

	sections[0].intersection = sections[1].intersection-1;				//Stelle sicher, dass Beginn der ersten Section vor der zweiten liegt
	return sections;
}


/* === Verschmilzt die Sections aus globalSections und localSections und speichert sie in globalSections */
void mergeSections (std::deque<Global_Section> &globalSections, std::deque<Section> &localSections) {

	if (globalSections.size()) {	//Wenn globalSections nicht leer ist

		if (globalSections.front().begin < localSections.front().intersection)	//Minimale Sections anpassen, damit ein einheitlicher Minimalwert existiert
			localSections.front().intersection = globalSections.front().begin;
		else globalSections.front().begin = localSections.front().intersection;

		unsigned int index1 = 0;
		unsigned int index2 = 0;

		while ((index1 < localSections.size()) && (index2 < globalSections.size())) {

			Section currentSection = localSections[index1];
			Section nextSection;
			if (index1 != localSections.size()-1)
				nextSection = localSections[index1+1];
			else nextSection = { std::max(localSections.back().intersection, globalSections.back().begin)+1, 0 };

			Global_Section currentGlobalSection = globalSections[index2];
			Global_Section nextGlobalSection;
			if (index2 != globalSections.size()-1)
				nextGlobalSection = globalSections[index2+1];
			else nextGlobalSection = { std::max(localSections.back().intersection, globalSections.back().begin)+1, std::vector<uint>() };

			if (currentSection.intersection == currentGlobalSection.begin)		//Füge nur Satz hinzu
				currentGlobalSection.sentences.push_back(currentSection.sentence);

			else if (currentSection.intersection > currentGlobalSection.begin) {	//Füge neue global_Section ab currentSection.begin ein

				Global_Section globalSection_tmp = { currentSection.intersection, currentGlobalSection.sentences };
				globalSection_tmp.sentences.push_back(currentSection.sentence);
				globalSections.insert(globalSections.begin()+index2+1, globalSection_tmp);
				index2++;						//Index anpassen
				
			} else {							//Füge neue Global_Section ab currentGlobalSection.begin ein

				Global_Section globalSection_tmp = { currentGlobalSection.begin, currentGlobalSection.sentences };
				globalSection_tmp.sentences.push_back(currentSection.sentence);
				globalSections.insert(globalSections.begin()+index2+1, globalSection_tmp);
				index2++;						//Index anpassen
			}

			//Indizes für nächsten Durchlauf bestimmen
			if (nextSection.intersection == nextGlobalSection.begin) {
				index1++;
				index2++;
			} else if (nextSection.intersection < nextGlobalSection.begin)
				index1++;
			else index2++;
		}

	} else {									//Wenn globalSections noch leer ist, kopiere Inhalte aus localSections
		for (unsigned int index1 = 0; index1 < localSections.size(); index1++) {
			Global_Section globalSection_tmp = { localSections[index1].intersection, std::vector<uint>() };
			globalSection_tmp.sentences.push_back(localSections[index1].sentence);
			globalSections.push_back(globalSection_tmp);
		}
	}
}


/* === Vergrößert den Hypothesenraum mit den aktuell optimierten Parameterwerten === */
bool addNewHypothesis(std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > &nBestList, const std::vector<double> &paramValues, const int &modelNumber) {

	bool newHypothesis = false;
	for (unsigned int index1 = 0; index1 < nBestList.size(); index1++) {					//Iteriere über alle Sätze

		std::vector<SentenceInfo> vec_tmp;							//Hilfsvektor

		for (unsigned int index2 = 0; index2 < nBestList[index1].second.size(); index2++) {		//Iteriere über alle Hypothesen

			bool alreadySeen = false;							//Hält fest, ob neue Hypothese bereits vorhanden ist
			SentenceInfo sentenceInfo_tmp = nBestList[index1].second[index2];		//Hilfsvariable
			for (int index4 = 0; index4 < modelNumber; index4++)				//Skalierungen übernehmen
				sentenceInfo_tmp.cost.scale[index4] = paramValues[index4];

			for (unsigned int index3 = index2+1; index3 < nBestList[index1].second.size(); index3++) {	//Iteriere über alle folgenden Hypothesen

				SentenceInfo currentSentenceInfo = nBestList[index1].second[index3];	//Aktuelle VergleichsInfo

				bool equalSentences = true;						//Stellt fest, ob Hypothesen gleich sind
				if (sentenceInfo_tmp.sentence.size() == currentSentenceInfo.sentence.size())
					for (unsigned int index4 = 0; index4 < sentenceInfo_tmp.sentence.size(); index4++)
						equalSentences = (equalSentences && (sentenceInfo_tmp.sentence[index4] == currentSentenceInfo.sentence[index4]));
				else equalSentences = false;
					
				if (equalSentences)							//Falls Hypothese gleich ist
					for (int index4 = 0; index4 < modelNumber; index4++)
						alreadySeen = (alreadySeen || (sentenceInfo_tmp.cost.scale[index4] == paramValues[index4]));	//Skalierungen vergleichen
			}

			if (!alreadySeen) {		//Wenn Hypothese noch nicht vorhanden, dann übernimm sie
				vec_tmp.push_back(sentenceInfo_tmp);
				newHypothesis = true;
			}
		}

		nBestList[index1].second.insert(nBestList[index1].second.begin(), vec_tmp.begin(), vec_tmp.end());	//Füge neue Hypothesen in die nBestLise des aktuellen Satzes ein
	}

	return newHypothesis;
}


/* === gibt die optimierten Parameter zurück === */
void printall(const std::vector<double> &paramValues, const int &modelNumber) {
	std::cout << "Optimierte Parameter-Werte:";
	for (int index1 = 0; index1 < modelNumber; index1++)
		std::cout << " " << paramValues[index1];
}


/* ===== EIGENTLICHE MERT-FUNKTION ===== */
void mert(std::vector<std::pair<std::vector<uint>,std::vector<SentenceInfo> > > nBestLists, int mNumber = 9) {

	int modelNumber;									//Anzahl der betrachteten Modelle
	if (mNumber > 9) modelNumber = 9; else modelNumber = mNumber;				//Abfangen zu großer Eingaben

	std::list<int> paramPermutation;							//Optimierungsreihenfolge der Parameter

	std::vector<double> paramValues;							//Aktuelle Zahlenwerte der Parameter
	for (int index1 = 0; index1 < modelNumber; index1++)
		paramValues.push_back(0.1);							//Mit 0.1 initialisieren
	
	std::vector<Cost::Model> models;							//Beinhaltet die betrachteten Modelle
	models = selectModels(modelNumber);							//Wähle alle betrachteten Modelle aus

	bool newHypothesis = true;								//Speichert, ob im letzten Durchgang, neue Hypothesen gefunden wurden, initialisiert mit true, damit Schleife überhaupt betreten wird
	
	while(newHypothesis) {
		
		bool paramChange = true;

		while(paramChange) {

			paramChange = false;
			paramPermutation = randomPermutation(modelNumber);

			while (!(paramPermutation.empty())) { 					//Für jeden Parameter

				int currentParam = paramPermutation.front();			//Wähle ersten Parameter in der Reihenfolge
				paramPermutation.pop_front();					//Entferne ihn aus der Reihenfolge
				
				std::deque<Global_Section> globalSections;			//Enthält später alle Sections (also unterteilte Abschnitte) mit allen im jeweiligen Abschnitt zu betrachtenden Hypothesen

				for (unsigned int index1 = 0; index1 < nBestLists.size(); index1++) {	//Iteriere über Anzahl der Sätze im Text
						std::deque<StraightLine> lines;			//Beinhaltet alle Geraden (Hypothesen) eines Satzes

					for (unsigned int index2 = 0; index2 < nBestLists[index1].second.size(); index2++) {	//Iteriere über die Anzahl der Hypothesen für einen Satz
						StraightLine line_tmp;
						line_tmp.gradient = findGradient(nBestLists[index1].second[index2].cost, currentParam, models);
						line_tmp.offset = findOffset(nBestLists[index1].second[index2].cost, currentParam, models, modelNumber);
						line_tmp.sentence = index2;

						unsigned int position_tmp = searchLines(lines, line_tmp.gradient, 0, lines.size()-1, lines.size());

						if (lines[position_tmp].gradient == line_tmp.gradient){	//Bei gleicher Steigung:
							if (lines[position_tmp].offset < line_tmp.offset) {	//Wähle Gerade mit höchstem Offset
								lines[position_tmp].offset = line_tmp.offset;	
								lines[position_tmp].sentence = line_tmp.sentence;
							}
						}
						else if (lines[position_tmp].gradient > line_tmp.gradient)	//Ansonsten: An richtiger Stelle einfügen
							lines.insert(lines.begin()+position_tmp, line_tmp);
						else lines.insert(lines.begin()+position_tmp+1,line_tmp);
						
						std::deque<Section> localSections = findSections(lines);	//Finde alle relevanten Schnittpunkte (als Sections gespeichert)
						mergeSections(globalSections, localSections);			//localSection zu globalSection hinzufügen
					}
				}
				double currentParamValue;							//Hält den aktuell besten Parameter-Wert fest
				double currentParamValueBleu = 0;						//BLEU-Wert für aktuellen Parameter-Wert
				
				for (unsigned int index3 = 0; index3 < globalSections.size()-1; index3++) {		//Iteriere über alle GlobalSections, bis auf die letzte
					int mid_tmp = (globalSections[index3].begin + globalSections[index3+1].begin) / 2;	//Bestimme mitte der aktuellen GobalSection
					double bleu_tmp = membleu(nBestLists, globalSections[index3].sentences);//Bestimme BLEU für aktuellen Parameter-Wert
					if (bleu_tmp > currentParamValueBleu) {					//Finde aktuell besten Parameter-Wert
						currentParamValue = mid_tmp;
						currentParamValueBleu = bleu_tmp;
					}
				}

				int mid_tmp = globalSections.back().begin + 1;
				double bleu_tmp = membleu(nBestLists, globalSections.back().sentences);
				if (bleu_tmp > currentParamValueBleu) {						//Betrachte nun auch die letzte GlobalSection
						currentParamValue = mid_tmp;
						currentParamValueBleu = bleu_tmp;
				}
				paramChange = (paramChange || (paramValues[currentParam] != currentParamValue));//Hat sich der Parameter-Wert verändert?
				paramValues[currentParam] = currentParamValue;
			}
		}
		newHypothesis = addNewHypothesis(nBestLists, paramValues, modelNumber);
	}
	printall(paramValues, modelNumber);
}
