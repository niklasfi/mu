

#include <vector>
#include <algorithm>
#include "bleu.cpp"
#include "global.h"
#include "sentenceinfo.h"
#include <stdio.h>
#include "cost.h"

using namespace std;

struct BleuAndScale{
	double bleu;
	vector<double> x;
	
	BleuAndScale(): bleu(), x(){}	
	BleuAndScale(double bleu, vector<double>& x): bleu(bleu), x(x){}
	
	BleuAndScale& operator=(const BleuAndScale& b){
		bleu=b.bleu;
		x=b.x;
		return *this;
	}
};

bool great(BleuAndScale i,BleuAndScale j) { return (i.bleu>j.bleu); }

/*double vecnorm(vector<double> vec){
	double summe=0;
	for(int i=0;i<vec.size();i++){
		summe+=(vec[i]/vec.size());
	}
}*/
vector< vector<double> > Simplex(){
	return std::vector<std::vector <double>>{
		std::vector<double>{0.75, 0.75, 0.75, 0.75, 0.25, 0.25, 0.25, 0.7,  0.8},
		std::vector<double>{0.6,  0.5,  0.6,  0.5,  1,    0,    0,    1,    0.8},
		std::vector<double>{0.5,  2,    0.3,  5,    0.6,  0.2,  0.2,  3,    3},
		std::vector<double>{1,    1,    0,    0,    0.55, 4,    0.3,  0.8,  1},
		std::vector<double>{0.2,  0.2,  0.7,  4,    0.77, 0.66, 0.22, 0.44, 1}
	};
	
	vector <vector <double> > x;
	x.resize(5);
	
	double temp[]={0.75,0.75,0.75,0.75,0.25,0.25,0.25,0.7,0.8,-1, //die "zufälligen Anfangsdaten von Simplex
				0.6,0.5,0.6,0.5,1,0,0,1,0.8,-1,
				0.5,2,0.3,5,0.6,0.2,0.2,3,3,-1,
				1,1,0,0,0.55,4,0.3,0.8,1,-1,
				0.2,0.2,0.7,4,0.77,0.66,0.22,0.44,1
				};
				
		
	int tsize = sizeof(temp)/sizeof(temp[0]);
	
	int vec =0;
	for(int i=0; i<tsize; i++){

		if(temp[i]==-1) vec++;
		else if(temp[i]!=-1) x[vec].push_back(temp[i]);

	}	
	/*for(int i=0;i<x.size();i++){
		for(int j=0;j<x[i].size();j++){
			cout<< x[i][j] <<"  ";
		}	
		cout<<endl;
	}*/
	return x;
}

vector<uint> findtrans(vector< pair < vector<uint>, vector <SentenceInfo> > > &nBestList,const vector <double> &x){
	double summe, min;
	uint min_id;
	summe =0;
	min=1./0.;
	vector<uint> result;
	Cost::scale=x;

	for(unsigned int j=0;j<nBestList.size();j++){
		for(unsigned int k=0;k<nBestList[j].second.size();k++){
			summe=nBestList[j].second[k].cost.totalize();
			if(summe<min){ min=summe; min_id=k;}
			summe=0;
		}	
		result.push_back(min_id);
	
	}		
	return result;
}
//vector< pair < vector<uint>,vector <SentenceInfo> > > nBestList	
vector<double> DownhillSimplex (std::vector<std::pair<std::vector<uint>, std::vector<SentenceInfo> > > &nBestList){
	int bigcount=0;
	int count=0;
	vector<double> x0 (9,0);
	vector< vector<double> > punkte=Simplex();//Punkte angelegt
	uint M=punkte.size()-1;
	vector<BleuAndScale> bleupunkte(punkte.size());

	for(unsigned int i =0; i<bleupunkte.size();i++){//Bleu der Punkte berechnen
		bleupunkte[i].x=punkte[i];
		bleupunkte[i].bleu=membleu(nBestList,findtrans(nBestList, punkte[i]));
	}	
	
	while(bigcount<100){
		for (unsigned int i=0; i<x0.size(); i++)	x0[i]=0;
		sort(bleupunkte.begin(),bleupunkte.end(),great);
		
		cout << bigcount << endl;
		for (uint i=0; i<bleupunkte.size(); i++)
			cout << bleupunkte[i].bleu << " ";
		cout << endl;
		
		for(unsigned int i=0;i<x0.size();i++){
			for(unsigned int j=0;j<M;j++){
				x0[i]+=(bleupunkte[j].x[i]/M); //berechne Schwerpunkt aller PUnkte außer x_m+1
			}
		}
		vector<double> xr(9);
		//berechung von x_r= x_0+alpha(x_0-x_M)
		for(uint i=0;i<xr.size();i++) xr[i]=(x0[i] +1.0*(x0[i] - bleupunkte[M].x[i]));
		
		double bleu_r=membleu(nBestList,findtrans(nBestList,xr));
		if(bleupunkte[0].bleu>=bleu_r && bleu_r>=bleupunkte[M].bleu) bleupunkte[M]=BleuAndScale(bleu_r,xr); //reflektieren

		else if(bleu_r > bleupunkte[0].bleu){
				
				vector<double> xe(9);

				//berechung von x_e=x_0+gamma(x_0-x_M)
				for(uint i=0;i<xe.size();i++) xe[i]=(x0[i] +2.0*(x0[i] - bleupunkte[M].x[i]));


				double bleu_e=membleu(nBestList,findtrans(nBestList,xe));
				if(bleu_e>=bleu_r && bleu_r>=bleupunkte[0].bleu)  bleupunkte[M]=BleuAndScale(bleu_e,xe);//expandiere
				else  bleupunkte[M]=BleuAndScale(bleu_r,xr);//reflektiere

		}else{
			vector <double> xc (9);
			//berechung von x_c=x_M+gamma(x_0-x_M)
			for(uint i=0;i<xc.size();i++) xc[i]=(bleupunkte[M].x[i] -0.5*(x0[i] - bleupunkte[M].x[i]));


			double bleu_c=membleu(nBestList,findtrans(nBestList,xc));

			if(bleu_c>=bleupunkte[M].bleu) bleupunkte[M]=BleuAndScale(bleu_c,xc);//kontrahiere

			else
				for(uint j=1;j<punkte.size();j++){
					//x_j=x_0+sigma(x_j-x_0)
					for(uint i=0;i<(bleupunkte[j].x.size());i++){
						bleupunkte[j].x[i]=bleupunkte[0].x[i]+0.5*(bleupunkte[j].x[i] - bleupunkte[0].x[i]);//komprimiere

					}
				}
		}
		/*if(count==10){
			translate();
			count=0;
		}
*/
		bigcount++;
		count++;
	}

	return bleupunkte[0].x;
}





