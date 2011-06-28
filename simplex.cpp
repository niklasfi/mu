
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
vector< vector<double> > Simplex(vector< pair < vector<uint>, vector <SentenceInfo> > > nBestList){
	vector <vector <double> > x;
	x.resize(5);
	
	
	double temp[]={0.75,0.75,0.75,0.75,0.25,0.25,0.25,0.7,0.8,-1,
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
	return x;
}

vector<uint> findtrans(vector< pair < vector<uint>, vector <SentenceInfo> > > nBestList,vector <double> x){
	double summe, min;
	uint min_id;
	summe =0;
	min=0;
	vector<uint> result;
	nBestList[0].second[0].cost.scale=x;

	for(int j=0;j<nBestList.size();j++){

		for(int k=0;k<nBestList[j].second.size();k++){

			summe=nBestList[j].second[k].cost.totalize();
			if(summe<min){ min=summe; min_id=k;}
			summe=0;
		}	
		result.push_back(min_id);
		
	}		
	return result;
}
	
vector<double> DownhillSimplex (vector< pair < vector<uint>,vector <SentenceInfo> > > nBestList){
	int bigcount=0;
	int count=0;
	vector<double> x0 (9,0);

	vector< vector<double> > punkte=Simplex(nBestList);//Punkte angelegt

	vector<double> bleupunkte(punkte.size());
	

	for(int i =0; i<punkte.size();i++){
		vector<uint> bestTrans =findtrans(nBestList, punkte[i]);
		bleupunkte[i]=membleu(nBestList,bestTrans);
	}	
	
	while(bigcount<100){
		sort(bleupunkte.begin(),bleupunkte.end(),great);
		
		for(int i=0;i<x0.size();i++){
			for(int j=0;j<(punkte.size()-1);j++){
				x0[i]+=(punkte[j][i]/(punkte.size()-1));
			}
		}
		vector<double> xr(9,0);
		//berechung von x_r
		for(int i=0;i<xr.size();i++) xr[i]=(x0[i] +1.0*(x0[i] - punkte[punkte.size()-1][i]));
		
		double temp=membleu(nBestList,findtrans(nBestList,xr));
		if(bleupunkte[0]>=temp && temp>=bleupunkte[punkte.size()-1]) bleupunkte[punkte.size()-1]=temp;

		else if(temp > bleupunkte[0]){

				vector<double> xe(9,0);
				//berechung von x_e
				for(int i=0;i<xe.size();i++) xe[i]=(x0[i] +2.0*(x0[i] - punkte[punkte.size()-1][i]));

				double temp2=membleu(nBestList,findtrans(nBestList,xe));
				if(temp2>=temp && temp>=bleupunkte[0])  bleupunkte[punkte.size()-1]=temp2;
				else  bleupunkte[punkte.size()-1]=temp;

		}else{
			vector <double> xc (9,0);
			//berechung von x_c
			for(int i=0;i<xc.size();i++) xc[i]=(punkte[punkte.size()-1][i] +0.5*(x0[i] - punkte[punkte.size()-1][i]));

			double temp3=membleu(nBestList,findtrans(nBestList,xc));

			if(temp3>=bleupunkte[punkte.size()-1]) bleupunkte[punkte.size()-1]=temp3;

			else
				for(int j=1;j<punkte.size();j++){
					for(int i=0;i<(punkte[j].size());i++){
						punkte[j][i]=punkte[0][i]+0.5*(punkte[j][i] - punkte[0][i]);
					}
				}
		}
/*		if(count==10){
			translate();
			count=0;
		}
*/
		bigcount++;
		count++;
	}

	return punkte[0];
}





