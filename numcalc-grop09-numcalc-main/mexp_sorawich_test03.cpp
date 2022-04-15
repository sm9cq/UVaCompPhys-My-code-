// mexp.cpp
#include <iostream>
#include "mexp.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

// not a good solution, but try running it for x<1 to begin

const unsigned long long int MAXIT=1e3;//1000;      // max iterations for series
const long double EPS=1e-9;     // stop for corrections smaller than this

#ifdef EXTERNC
extern "C" {
#endif

unsigned long long int fact(unsigned int x){//int 
  if (x<=1) return 1;
  else return x*fact(x-1);
}


  long double value=1.0;
  long double value_2 =0;
  long double term = 0;


  long double mexp(double x, long double value, long double value_2, long double term){

 
  int sign;
  for (unsigned int n=1;n<MAXIT;n++){
    //n%2 ? sign=-1 : sign=1;
    term=pow(x,n)/fact(n); 
    value = value+ term;
    if (fabs(term/value) < EPS || !isfinite(value) ) break;
    }
  value_2=1/value;
  return value_2;//term;
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
  double val = mexp(x,1,0,0);
  double relerr=fabs(val-exp(-x))/exp(-x);
  printf("exp(-%lf) = %lg  , rel error = %lg\n",x,val,relerr);//%lg for double %Lf for long double
  return 0;
} 
// mexp.cpp

#include "mexp.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

// not a good solution, but try running it for x<1 to begin

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
