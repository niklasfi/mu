#include <assert.h>
#include <vector>
#include <math.h>
#include "sentencepool.h"
#include "math.h"
#include "bleu.cpp"

using namespace std;


int main(){

  vector<unsigned int>* vek=new vector<unsigned int> (6);
  vector<unsigned int>* vek2=new vector<unsigned int>(4);
  vector<unsigned int>* vek3=new vector<unsigned int> (3);
  vector<unsigned int>* vek4=new vector<unsigned int> (3);
  
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

  (*vek4)[0]=1;
  (*vek4)[1]=3;
  (*vek4)[2]=1;

  SentencePool sen;
  sen.reference.push_back(vek);
  sen.reference.push_back(vek3);
  sen.guess.push_back(vek2);
  sen.guess.push_back(vek4);

  double a = exp(1-(9./7.))*exp((1./3.)*log(6./7.)+(1./3.)*log(3./5.)+(1./3.)*log(1./3.));
  assert(fabs(bleu(sen,3)-a)<=1e-5); //Test mit kleiner Hypothesenlänge

  sen.reference[0]=(vek2);
  sen.reference[1]=(vek4);
  sen.guess[0]=(vek);
  sen.guess[1]=(vek3);

  double b =exp((1./3.)*log(6./9.)+(1./3.)*log(3./7.)+(1./3.)*log(1./5.));
  assert(fabs(bleu(sen,3)-b)<=1e-5);//Test mit Referenz kleiner als Hypothese

  vector<unsigned int>* vek5=new vector<unsigned int> (4);
  vector<unsigned int>* vek6=new vector<unsigned int> (5);  

  for(int i=0;i<4;i++) {
		(*vek5)[i] = 6;
		(*vek6)[i] = 6;
  }
  (*vek6)[4]=7;
  sen.reference.pop_back();
  sen.reference.pop_back();
  sen.guess.pop_back();
  sen.guess.pop_back();
  sen.reference.push_back(vek5);
  sen.guess.push_back(vek6); 

  double r = 0.25*log(4./5.)+0.25*log(3./4.)+0.25*(2./3.)+0.25*log(1./2.);
  double r2 = 0.25*(log(4./5.)+log(3./4.)+log(2./3.)+log(1./2.));
  double c=exp(r2);

  assert(fabs(bleu(sen,4)-c)<=1e-5);//Test auf 4-grams

  //sen.reference.pop_back();
  //sen.guess.pop_back();
  //assert(bleu(sen,4) ==-(1./0.));//Test auf leeren Pool

}
