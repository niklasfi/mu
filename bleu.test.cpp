#include <assert.h>
#include <vector>
#include <cmath>
#include "sentencepool.h"
//#include "bleu.cpp"

double BLEU (SentencePool);

using namespace std;


int main(){

  vector<unsigned int>* vek=new vector<unsigned int> [6];
  vector<unsigned int>* vek2=new vector<unsigned int> [4];
  vector<unsigned int>* vek3=new vector<unsigned int> [3];
  vector<unsigned int>* vek4=new vector<unsigned int> [3];
  
  (*vek)[0]=2;
  (*vek)[1]=1;
  (*vek)[2]=3;
  (*vek)[3]=9;
  (*vek)[4]=2;
  (*vek)[5]=4;

  (*vek2)[0]=2;
  (*vek2)[1]=1;
  (*vek2)[2]=6;
  (*vek2)[3]=9;
  
  
  (*vek3)[0]=1;
  (*vek3)[1]=3;
  (*vek3)[2]=1;

  (*vek4)[3]=1;
  (*vek4)[4]=3;
  (*vek4)[5]=1;

  SentencePool sen;
  sen.reference[0]=vek;
  sen.reference[1]=vek3;
  sen.guess[0]=vek2;
  sen.guess[1]=vek4;

  double a = exp(1-(9/7))*exp(0.25*log(6/7)+0.25*log(3/7)+0.25*log(1/7));
  assert(BLEU(sen)==a); //Test mit kleiner Hypothesenlänge

  SentencePool sen2;
  sen2.reference[0]=vek2;
  sen2.reference[1]=vek4;
  sen2.guess[0]=vek;
  sen2.guess[1]=vek3;

  double b =exp(0.25*log(6/9)+0.25*log(3/9)+0.25*log(1/9));
  assert(BLEU(sen2)==b);//Test mit Referenz kleiner als Hypothese

  vector<unsigned int>* vek5=new vector<unsigned int> [4];
  vector<unsigned int>* vek6=new vector<unsigned int> [5];  

  for(int i=0;i<5;i++) {
		(*vek5)[i] = 6;
		(*vek6)[i] = 6;
  }
  (*vek6)[5]=7;
  SentencePool sen3;
  sen3.reference[0]=vek5;
  sen3.guess[0]=vek6; 

  double c=exp(0.25*log(5/6)+0.25*log(4/6)+0.25*(3/6)+0.25*log(1/6));

  assert(BLEU(sen3)==c);//Test auf 4-grams

  SentencePool sen4;
  assert(BLEU(sen4) ==0);//Test auf leeren Pool

  delete vek;
  delete vek2;
  delete vek3;
  delete vek4;
  delete vek5;
  delete vek6;
  

}
