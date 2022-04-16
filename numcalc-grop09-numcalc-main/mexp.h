// mexp.h
#pragma once

#ifdef EXTERNC
extern "C" {
#endif
long double fact(int x);
double mexp(double x);
void usage(char* name);
#ifdef EXTERNC
}
#endif
