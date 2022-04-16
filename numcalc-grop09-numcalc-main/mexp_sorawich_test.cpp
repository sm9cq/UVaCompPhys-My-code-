// mexp.cpp
#include <iostream>
#include "mexp.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

// not a good solution, but try running it for x<1 to begin

const int MAXIT=1000;//1000;      // max iterations for series
const long double EPS=1e-9;     // stop for corrections smaller than this

#ifdef EXTERNC
extern "C" {
#endif

unsigned long int fact(unsigned int x){//int 
  if (x<=1) return 1;
  else return x*fact(x-1);
}

double mexp(double x){
  long double value=1.0;
  int sign;
  for (unsigned int n=1;n<MAXIT;n++){
    n%2 ? sign=-1 : sign=1;
    long double term=sign*pow(x,n)/fact(n); 
    value += term;
    cout << "n:" << n <<  "value:" << value << endl;
    cout << "term:" << term << endl;
    cout << "term/value=" << fabs(term/value) << endl;
    if (fabs(term/value) < EPS || !isfinite(value) ) break;//term/value
  //problem is when x>=1,n go up and term go very small until it cannot be stored -> inf 
  //relative term/value is very small
  }
  return value;
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
