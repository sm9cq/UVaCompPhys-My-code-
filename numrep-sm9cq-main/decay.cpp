//  decay.cpp: Spontaneous radioactive decay simulation

#include <cstdio>
#include <cstdlib>
#include <ctime>
      
int main() {
  const double lambda=0.01;        // the decay constant P(decay in t interval)
  const unsigned nStart=100000;    // number of atoms at t=0
  const unsigned nSteps=500;       // number of time steps
 
  FILE *output;			   // save data in decay.dat
  output = fopen("decay01.dat","w");

  srand48(time(NULL));             // seed number generator
  
  unsigned nRemaining=nStart;
  // Brute force simulation: for each atom that remains undecayed
  // At each time step, let's see if each atom decays
  // This simple model produces a good exponential decay curve
  // approximation for lambda<<1
  fprintf(output, "%d\t%f\n", 0, (double)nRemaining/nStart);
  for(unsigned i=1; i<=nSteps; i++) {                       // time loop
    unsigned nDecay=0;
    for (unsigned atom=1; atom<=nRemaining; atom++) {      // loop over atoms
      double u = drand48();          // drand48=[0,1)
      if(u < lambda) nDecay++;       // an atom decays in this t interval
    }
    nRemaining -= nDecay;
    // write fraction of remaining atoms
    fprintf(output, "%d\t%f\n", i, (double)nRemaining/nStart);
  }

  printf("data stored in decay01.dat\n");
  fclose(output);
  return 0;
}
 
