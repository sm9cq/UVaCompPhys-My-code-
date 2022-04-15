#include <iostream>

using namespace std;

// A simple function that takes a while to calculate!
// We'll calculate the slowly converging series 1 - 1/2 + 1/3 - 1/4 + ...
// The series converges to ln(2)
double longcalc(unsigned long ncalcs=1000*1000){
  double val=0.0;
  for (unsigned long i=1; i<=ncalcs; i++){
    val += 1.0/i * (2.0*(i%2)-1);
  }
  return val;
}
