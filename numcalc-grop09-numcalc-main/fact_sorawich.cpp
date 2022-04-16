#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;
const double EPS=1e-9;
 

//function to find factorial of given number
long double fact(int n)
{
 if (n == 0)
   return 1;
 return n * fact(n - 1);
}

                  

int main()
{
  double value=1.0;
  int sign;
  unsigned int x = 90; 
  for (int n=1;n<1000;n++){
    n%2 ? sign=-1 : sign=1;
    double term=sign*pow(x,n)/fact(n); 
    value += term;
    cout << fact(n) << endl;
    cout << term << endl;
    cout << n << endl;
    if (fabs(term/value) < EPS || !isfinite(value) ) break;
  }
  //relerr=fabs(value-exp(-x))/exp(-x);
  cout << value <<endl;
}
