// mexp.h
#pragma once

#ifdef EXTERNC
extern "C" {
#endif
int fact(int x);
double mexp(double x);
void usage(char* name);

// Add a version based on a C++ class
class MEXP {
public:
  double eval(double x);        // example of a typical method
  double operator()(double x);  // same feature implemented with an operator
};
  
#ifdef EXTERNC
}
#endif
  
