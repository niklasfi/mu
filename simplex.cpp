
#include <vector>
#include <algorithm>
#include "bleu.cpp"
#include "global.h"
#include "sentenceinfo.h"
#include <stdio.h>
#include "cost.h"

using namespace std;

bool great(double i,double j) { return (i>j); }

/*double vecnorm(vector<double> vec){
	double summe=0;
	for(int i=0;i<vec.size();i++){
		summe+=(vec[i]/vec.size());
	}
}*/
vector< vector<double> > Simplex(){
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
	int M=punkte.size()-1;
	int bigcount=0;
	int count=0;
	vector<double> x0 (9,0);
	vector< vector<double> > punkte=Simplex();//Punkte angelegt
	vector<double> bleupunkte(punkte.size());

	for(unsigned int i =0; i<punkte.size();i++){
		vector<uint> bestTrans =findtrans(nBestList, punkte[i]);
		bleupunkte[i]=membleu(nBestList,bestTrans);
	}	
	
	while(bigcount<3){
		for (int i=0; i<x0.size(); i++)	x0[i]=0;
		sort(bleupunkte.begin(),bleupunkte.end(),great);
		

		for(int i=0;i<x0.size();i++){
			for(int j=0;j<M;j++){
				x0[i]+=(punkte[j][i]/M); //berechne Schwerpunkt aller PUnkte außer x_m+1
			}
		}
		vector<double> xr(9,0);
		//berechung von x_r= x_0+alpha(x_0-x_M)
		for(int i=0;i<xr.size();i++) xr[i]=(x0[i] +1.0*(x0[i] - punkte[M][i]));

		
		double bleu_r=membleu(nBestList,findtrans(nBestList,xr));
		if(bleupunkte[0]>=bleu_r && temp>=bleupunkte[M]) punkte[M]=xr; //reflektieren

		else if(bleu_r > bleupunkte[0]){
				
				vector<double> xe(9,0);

				//berechung von x_e=x_0+gamma(x_0-x_M)
				for(int i=0;i<xe.size();i++) xe[i]=(x0[i] +2.0*(x0[i] - punkte[M][i]));


				double bleu_e=membleu(nBestList,findtrans(nBestList,xe));
				if(bleu_e>=bleu_r && bleu_r>=bleupunkte[0])  punkte[M]=xe;//expandiere
				else  punkte[M]=xr;//reflektiere

		}else{
			vector <double> xc (9,0);
			//berechung von x_c=x_M+gamma(x_0-x_M)
			for(int i=0;i<xc.size();i++) xc[i]=(punkte[M][i] -0.5*(x0[i] - punkte[M][i]));


			double bleu_c=membleu(nBestList,findtrans(nBestList,xc));

			if(bleu_c>=bleupunkte[M]) punkte[M]=xc;//kontrahiere

			else
				for(int j=1;j<punkte.size();j++){
					//x_j=x_0+sigma(x_j-x_0)
					for(int i=0;i<(punkte[j].size());i++){
						punkte[j][i]=punkte[0][i]+0.5*(punkte[j][i] - punkte[0][i]);//komprimiere

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

	return punkte[0];
}





