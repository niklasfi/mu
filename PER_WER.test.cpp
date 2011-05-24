#include <vector>
#include <iostream>
#include "sentencepool.h"
#include "PER_WER.cpp"
#include <math.h>
#include <assert.h>

using namespace std;


int main(){
  
    
  vector<unsigned int>* vek = new vector<unsigned int>;
  for (unsigned int i=1; i<11; i++){
     vek->push_back(i);
  }
  
  vector<unsigned int>* vek2=new vector<unsigned int>;
  (*vek2)=(*vek);
	
  
  assert (PER(vek,vek2) == 0);
  
  vector<unsigned int>* vek3=new vector<unsigned int>;
  for (unsigned int i=6; i<26; i++){
      vek3->push_back(i);
  }
  
  assert (fabs(PER(vek,vek3) - 1.5)<0.00001);
  
  SentencePool sen;
  sen.reference.push_back(vek);
  sen.reference.push_back(vek);
  sen.guess.push_back(vek3);
  sen.guess.push_back(vek3);
  
  assert (fabs(PER_global(sen)-1.5)<0.00001);
  
  
  
  vek=new vector<unsigned int> (6);
  vek3=new vector<unsigned int> (6);
  
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
  
 
  assert (fabs(WER(vek,vek3)- 0.5)<0.00001);
  
  (*vek2)=(*vek);
  vector<unsigned int>* vek4=new vector<unsigned int>(6);  
  (*vek4)=(*vek3);
  sen.reference[0]=(vek);
  sen.reference[1]=(vek2);
  sen.guess[0]=(vek3);
  sen.guess[1]=(vek4);
  
  assert (fabs(WER_global(sen) - 0.5)<0.000001);
  

}
