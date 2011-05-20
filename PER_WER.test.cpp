#include <vector>
#include <iostream>
//#include "sentencepool.h"
//#include "Fehlerrate.cpp"

using namespace std;

class sentencepool;

double PER_global(sentencepool);
double PER(vector<int>* r, vector <int>* g);
double WER(vector<int>* r, vector <int>* g);
double WER_global(sentencepool);



int main(){
  
  bool ok=true;
  
  vector<int>* vek = new vector<int>;
  for (int i=0; i<10; i++){
     vek->push_back(i);
  }
  
  vector<int>* vek2=new vector<int>;
  (*vek2)=(*vek);
  
  if (PER(vek,vek2) != 0){	
    cout << "Es stimmt was mit dem PER nicht!" << endl;
    ok=false;
  }
  
  vector<int>* vek3=new vector<int>;
  for (int i=5; i<25; i++){
      vek3->push_back(i);
  }
  
  if (PER(vek,vek3) != 1.3){
    cout << "Es stimmt was nicht mit PER!" << endl;
    ok=false;
  }
  
  sentencepool sen;
  sen.reference[0]=vek;
  sen.reference[1]=vek2;
  sen.guess[0]=vek;
  sen.reference[1]=vek3;
  
  if (PER_global(sen) != 1.3){
    cout << "Es stimmt was nicht mit PER!" << endl;
    ok=false;
  }
  
  if(ok)	cout << "PER ist ok!" << endl;
  
  delete[] vek;
  delete[] vek3;
  
  vek=new vector<int> [6];
  vek3=new vector<int> [6];
  
  (*vek)[0]=2;
  (*vek)[1]=1;
  (*vek)[2]=3;
  (*vek)[3]=1;
  (*vek)[4]=3;
  (*vek)[5]=4;
  
  (*vek3)[0]=1;
  (*vek3)[1]=3;
  (*vek3)[2]=1;
  (*vek3)[3]=3;
  (*vek3)[4]=1;
  (*vek3)[5]=5;
  
  ok=true;
 
  if (WER(vek,vek3)	!= 0.5){
    cout << "Es stimmt was nicht mit WER!" << endl;
    ok=false;
  }
  
  sen.reference[0]=vek;
  sen.reference[1]=vek;
  sen.guess[0]=vek3;
  sen.guess[1]=vek3;
  
  if (WER_global(sen) != 1){
    cout << "Es stimmt was nicht mit WER!" << endl;
    ok=false;
  }
  
  if (ok)	cout << "WER ist ok!" << endl;
  
  return 0;
}