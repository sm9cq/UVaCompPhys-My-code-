// mexp.cpp
// a very simple and poorly implemented calculation of exp(-x)
// produces OK results only for x<<1

#include "mexp.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

const int MAXIT=1000;      // max iterations for series
const double EPS=1e-9;     // stop for corrections smaller than this

#ifdef EXTERNC
extern "C" {
#endif

int fact(int x){ 
  if (x<=1) return 1;
  else return x*fact(x-1);
}

double mexp(double x){
  double value=1.0;
  int sign;
  for (int n=1;n<MAXIT;n++){
    n%2 ? sign=-1 : sign=1;
    double term=sign*pow(x,n)/fact(n); 
    value += term;
    if (fabs(term/value) < EPS || !isfinite(value) ) break;
  }
  return value;
}

// implement our method
  double MEXP::eval(double x) {return mexp(x);}
  double MEXP::operator()(double x) {return mexp(x);}
  
#ifdef EXTERNC
}
#endif


void usage(char* name){
  printf("Usage %s <positive value>\n",name);
}

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
  printf("exp(-%lf) = %lf  , rel error = %lg\n",x,val,relerr);
  return 0;
} 
