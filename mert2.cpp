#include "mert2.h"

class BleuInfo;

using namespace std;

/* Strukturen aus dem Decoder
 * 
 * typedef std::vector<SentenceInfo> nBestList;
 * 
 * struct hypRefPair{
			Sentence* reference;
			nBestList* nBest;  
			hypRefPair(Sentence* ref, nBestList* hyp);
			~hypRefPair();
		};

 === Strukturtyp StraightLine, um Geraden darzustellen === 
struct StraightLine {
	double gradient;	//Steigung
	double offset;		//Achsen-Abstand
	uint sentence_pos;		//Position der entsprechenden Hypothese
	double bleu;
	
	StraightLine(double gradient, double offset, uint sentence): gradient(gradient), offset(offset), sentence(sentence_pos){}
};

 === Strukturtyp Section, um Schnittpunkt und den im folgenden benutzten Satz zu speichern === 
struct Section {
	double intersection;	//Beginn der Section (also vorderer Schnittpunkt)
	vector<uint> translation;		//Position der entsprechenden Hypothesen
	double bleu;
};

 === wie Section, speichert aber nicht nur Position einer Hypothese, sondern mehrere === 
struct Global_Section {
	double begin;		//Beginn der Global_Section (also vorderer Schnittpunkt)
	std::vector<uint> sentences;	//Position der entsprechenden Hypothesen in richtiger Reihenfolge
};*/
/* === Gibt eine zufällige Permutation der Model1e zurück === */
list<int> models_permutated(){
	vector<int> models; //enthält alle gesetzten Models
	for (unsigned int i=0; i< 10; i++){
		if (Cost::set((Cost::Model)(i)))	models.push_back(i);
	}
		
	std::list<int> permutation; //Spätere Rückgabeliste

	for (unsigned int index = 0; index < models.size(); index++) {
		int randomNumber = rand() % models.size(); //Bestimme zufällige Zahl in models
		int integer_tmp = models[randomNumber]; //Ließ die Zahl aus
		permutation.push_back(integer_tmp); //Füge sie ans Ende der Permutation ein
		models.erase(models.begin()+randomNumber); //Lösche sie aus dem Vektor
	}

	return permutation;
}
/************************************************
 * 												*
 * 		wir initialiseren unsere Geraden		*
 * 												*
 ************************************************/



/* ======= initialisert alle Geraden für einen Satz ==============*/
void Mert::init_lines_single(hypRefPair& einSatz, unsigned int sentence_nr){

	
	
	for (unsigned int i = 0; i<einSatz.nBest->size(); i++){ //iteriert über jede Übersetzung
		Cost::setScale((Cost::Model) aktParam, 1);
		double gradient = (-1)*((*einSatz.nBest)[i].cost.cost((Cost::Model) aktParam));
		cout << " Steigung  " << gradient << endl;
		Cost::setScale((Cost::Model) aktParam, 0); //den aktuellen Parameter in Cost auf 0 setzen, damit Cost die Kosten ohne diesen Parameter ausrechnet
		double offset = (-1)*((*einSatz.nBest)[i].cost.cost());
		cout<< " abstand  " << offset << endl;
		lines.push_back(new StraightLine(gradient, offset, sentence_nr, i));
		Cost::setScale((Cost::Model) aktParam, aktParam_value);
	}
}

void Mert::init_lines(){
	while(!lines.empty()){ lines.pop_front();}
	for (unsigned int sentence_nr=0; sentence_nr < translation->size(); sentence_nr++){
		init_lines_single((*translation)[sentence_nr], sentence_nr);
	}
	
}

/************************************************************
 * 															*
 * 		wir berechnen Schnittpunkte mit dem Sweepline		*
 * 															*
 ************************************************************/

/* sortiert die Geraden nach Steigung*/

bool gradient_greater(StraightLine* l1, StraightLine* l2){
	return l1->gradient < l2->gradient;
}

bool x_value (StraightLine* l1, StraightLine* l2){
	return l1->x < l2->x;
}

void Mert::sort(){
	std::sort(lines.begin(), lines.end(), gradient_greater);
}


struct Schnittpunkt{
	double x,y; //Koordinaten
	double gradient; //Steigung der anderen Gerade
	
	bool operator<(Schnittpunkt& s)	{return (y < s.y);}
};

/* === Liefert die für MERT nötigen Schnittpunkte und speichert diese in lines === */
void Mert::findSections () {
	
	sort();
	/*for (list<StraightLine>::iterator it=lines.begin(); it != lines.end(); it++){
		StraightLine currentLine = *it;								//Hilfs-Section
		
		
		//schnittpunkte mit der aktuellen gerade berechnen
		priority_queue<Schnittpunkt, vector<Schnittpunkt> > schnittpunkteSammlung; 
		for (list<Straighline>::iterator it2=lines.begin(); it2 != lines.end(); it2++){
			Schnittpunkt schnittpunkt;
			
			schnittpunkt.x= it->offset - it2->offset;
			schnittpunkt.x/=(it2->gradient-it->gradient);
			schnittpunkt.y=it->gradient*schnittpunkt.x+it->offset;
			schnittpunkt.gradient=it2->gradient;
			
			schnittpunkteSammlung.push(schnittpunkt);
		}
		
		double gradient_best=schnittpunkteSammlung.top().gradient; //die Steigung der Geraden mit dem besten Schnittpunkt
		
		//Lösche alle Geraden mit niedrigerer Steigung als gradient_best
		for (list<Straightline>::iterator it2=lines.begin(); it2->gradient < gradient_best; it2++){
		lines.remove_if(
	}
		*/
	
	int j=0, K=lines.size();
	for (int i=0; i< K; i++){
		StraightLine* l=lines[i];
		l->x=-(1./0.);
		
		if (0 < j ){
			if (lines[j-1]->gradient == l->gradient){
				if (l->offset <= lines[j-1]->offset)	continue;
				j--;
			}
			while(0 < j){
				l->x=(l->offset) - (lines[j-1]->offset);
				l->x/=((lines[j-1]->gradient) - (l->gradient));
				//cout << "schnittpunkt x " << l-> x << " berechnet mit " << (l->offset)<< " - " <<(lines[j-1]->offset) << " / " << (lines[j-1]->gradient)<< " - " << (l->gradient) <<endl;
				cout << "j " << j << endl;
				
				if ((lines[j-1]->x) < (l->x)){break;}
				else {j--;}
			}
			//cout << " j nach der while "<< j << endl; 
			if (0 == j)	l->x=- 1. / 0.;

			l->pre_sentence_pos=lines[j]->sentence_pos;
			l->pre_sentence_best=lines[j]->sentence_best;
			//cout << "oben   "<<l->x<< "   j    "<< j<<endl;
			
			lines[j++]=l;
		}
		else{
			l->pre_sentence_pos=lines[j]->sentence_pos;
			l->pre_sentence_best=lines[j]->sentence_best;	

			lines[j++]=l;
		}
	}
	lines.resize(j);
	std::sort(lines.begin(), lines.end(), x_value); 
}

double Mert::bleu_optimize(){
	double best_bleu=0;
	double best_scale;
	
	vector<unsigned int> picks(translation->size());
	for (unsigned int i=0; i< translation->size(); i++)	picks[i]=0;
	   
	BleuInfo bleu=BleuInfo::membleu(*translation, picks);
	BleuInfo::N=1;

	unsigned int aktline;
	double scale_aktParam;
	unsigned int aktParam_picks;
	cout << " Schnittpunkte  " << lines.size() << endl;
	cout << " Bleu  " << bleu() << endl;
	
	for (unsigned int i=0; i< lines.size(); i++){

		if (i==0){
			scale_aktParam=(-1)*((lines[i+1]->x)-1.0);
			aktParam_picks=lines[i+1]->pre_sentence_best;
			aktline=lines[i+1]->pre_sentence_pos;
		}
		else{
			if (i==lines.size()-1){
				scale_aktParam=(-1)*((lines[i]->x)+1.0);
				aktParam_picks=lines[i]->sentence_best;
				aktline=lines[i]->sentence_pos;
			}
			else{
				scale_aktParam=(-1)*(lines[i]->x+lines[i+1]->x)/2;
				aktParam_picks=lines[i]->sentence_best;
				aktline=lines[i]->sentence_pos;
			}
		}
		hypRefPair* akthypRefPair = &(*translation)[aktline];
		BleuInfo* bleu_ptr=(*akthypRefPair->nBest)[aktParam_picks].bleu;
		
		if(!bleu_ptr){	bleu_ptr=new BleuInfo(*(akthypRefPair->reference), (*akthypRefPair->nBest)  	[aktParam_picks].sentence);}

		cout << " Bleu2  " << bleu() << endl;
		
		bleu-=*((*akthypRefPair->nBest)[0].bleu); //die übersetzung für diesen satz rausnehmen
		bleu+=*bleu_ptr; //unsere reinnehmen
		
		if (bleu() > best_bleu){
			best_bleu=bleu();
			best_scale=scale_aktParam;
		}
		cout << " Bleu3  " << bleu() << endl;
		
		//veränderungen an bleu rückgängig machen
		bleu-=*bleu_ptr;
		bleu+=*((*akthypRefPair->nBest)[0].bleu);


	}
	
	Cost::setScale((Cost::Model) aktParam, best_scale);
	return best_scale;
}
void Mert::print_Schnitt(){
	for( unsigned int i=0; i<lines.size();i++){
		cout<< "x-Wert:   " <<lines[i]->x<< endl;
	}
}   
	


vector<double> Mert::optimize(){
	vector<double> res;
	while (true){
		//berechne reihenfolge
list<int> models= models_permutated();
		
		//for (int i=0; i<3; i++){ /zum debuggen, dann kennt man die Reihenfolge der Parameter
		while (!models.empty()){
			aktParam=models.front();
			models.pop_front();
			aktParam_value=Cost::getScale((Cost::Model)aktParam);
			init_lines();
			findSections();
			print_Schnitt();
			double opt= bleu_optimize();
			res.push_back(opt);
			//cout << i << " " << opt << endl;
			//for(unsigned int j =0;j<lines.size(); j++) lines[j]=0;
		}
		break;
	}
// 	delete translation;
// 	for (unsigned int i=0; i< lines.size(); i++)	delete lines[i];
	
	return res;
}
