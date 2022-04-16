// mexp.cpp
//test
//#include "mexp.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include<iostream>
using namespace std;

// not a good solution, but try running it for x<1 to begin

const int MAXIT=1000;      // max iterations for series
const long double EPS=1e-9;     // stop for corrections smaller than this

#ifdef EXTERNC
extern "C" {
#endif

long double fact(int x){ 
  if (x<=1) return 1;
  else return x*fact(x-1);
}

double mexp(double x){
  long double value=1.0;
  //int sign;
  if(x<93){
  for (int n=1;n<MAXIT;n++){
    long double term=pow(x,n)/fact(n);
    value += term;
    if (fabs(term/value) < EPS || !isfinite(value) ) break;
  }}
//here i alter the factorial slightly so that the value is carried along
//within the factorial
  if(x>=93){
  for (int n=1;n<MAXIT;n++){
    //double term=sign*pow(x,n)/fact(n); 
    long double term = 1;
    for (int i =1;i<n;i++){
	term = term*x/i;
	}

	
//    long double term=pow(92,n)/fact(n)+pow(x-92,n)/fact(n);
    value += term;
    if (fabs(term/value) < EPS || !isfinite(value) ) break;
  }}
  return 1/value;
}
 
void usage(char* name){
  printf("Usage %s <positive value>\n",name);
}

#ifdef EXTERNC
}
#endif

  
int main(int argc, char *argv[]){
  if (argc<2) {
    usage(argv[0]);
    return -1;
  }
  
  double x=atof(argv[1]);
  if (x<0) {
    usage(argv[0]);
    return -1;
  }
  double val = mexp(x);
  double relerr=fabs(val-exp(-x))/exp(-x);
  printf("exp(-%lf) = %lg  , rel error = %lg\n",x,val,relerr);
  return 0;
} 
