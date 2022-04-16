/* 
******************************************************************************
* ising1d.c                                                                  *
* =========                                                                  *
*    Uses Metropolis algorithm to generate thermal ensemble for the          *
*    one-dimensional ising chain with free boundary conditions               *
*    in a magnetic field h.                                                  *
*								             *
* Storage: The state of the lattice is stored as spins +-1 in elements 1     *
*    through N of an array of length N+2.  The free boundary conditions      *
*    are handled by fixing elements 0 and N+1 of the array to be zero.       *
*  								             *
******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
 
/* if you don't have drand48 uncomment the following two lines */
/*    #define drand48 1.0/RAND_MAX*rand
      #define srand48 srand                */


/* update_spin (s, env)
** -----------
**   Do a metropolis update on a spin *s whose "environment" in env.
** The environment should be set beforehand so that the dependence
** of the total beta*energy on the value of the selected spin is
** given by
**               (selected spin)*env .
**
** sweep (spin, N, beta, h)
** -----
**   Sweep once through all the sites of the lattice spin of length N,
** trying an update at each site with inverse temperature beta and external
** magnetic field parameter h.
*/

void update_spin(int *s, double env){
  int spin = *s;
  int newspin = ( drand48() < 0.5 ? 1 : -1 );
  double DeltaBetaE = -(newspin-spin)*env;             // beta*(E(new) - E(old)) 
  if ( DeltaBetaE <= 0 || drand48() < exp(-DeltaBetaE) ) *s = newspin;  // Metropolis method
}

void sweep(int *spin, int N, double beta, double h){
  int ns;

  for(ns=1; ns<=N; ns++) {
    update_spin(&(spin[ns]), beta*(spin[ns-1] + spin[ns+1]) + h);
  }
}

/* InitializeHot (spin, N)
** =======================
**   Initialize all the N spins of the lattice "spin" randomly.  Also
** initialize the two fake "boundary" spins to zero to implement free
** boundary conditions.
*/

void InitializeHot(int *spin, int N) {
  int ns;

  for(ns=1; ns<=N; ns++) spin[ns] = (drand48() < 0.5 ? 1 : -1);
  spin[0] = spin[N+1] = 0;
}


int main(){
  unsigned ntherm=1000;   // number of thermalization runs

  FILE *output;
  const char *OutputFileName = "ising1d_vs_T.dat";

  output = fopen(OutputFileName,"w");
  fprintf(output,"# T       sigma\n");

  printf("Program calculates <sigma> vs. T for a chain of N spins\n");
  printf("with free boundary conditions.\n\n");

  srand48((long)time(0));		      // seed the generator 

  unsigned N, nsweep, ntemp;
  double h, Tmax;
  printf("Enter number of spins in chain:\n");
  scanf("%d", &N); 

  printf("Enter # of sweeps per sample:\n");
  scanf("%d", &nsweep); 

  printf("Enter value of magnetic field parameter h:\n");
  scanf("%lf", &h);

  printf("Enter starting value (maximum) of temperature T (=1/beta):\n");
  scanf("%lf", &Tmax);

  printf("Enter # of temperatures to simulate:\n");
  scanf("%i", &ntemp);

  // make room for N spins and two fake boundary spins
  int *spin = (int*)malloc((N+2)*sizeof(int)); 
  InitializeHot(spin, N);

  // now run ntemp temperatures between Tmax and 0
  int nmag, ntotal;
  fprintf(stderr,"Temp step:");
  for (unsigned itemp=ntemp; itemp>0; itemp--) {
    fprintf(stderr," %d ",ntemp-itemp+1);
    double T = (Tmax*itemp)/ntemp;
    if (T<1e-6) T=1e-6;  // protect agains overflow
    double beta = 1/T;
    // sweep ntherm times to thermalize system at new temperature 
    for(unsigned n=0; n<ntherm; n++) sweep(spin,N,beta,h);
    // then sweep through lattice nsweep times for that temperature 
    nmag=ntotal=0;
    for(unsigned n=0; n<nsweep; n++) {
      sweep(spin,N,beta,h);
      for(unsigned ns=1; ns<=N; ns++) {
	nmag += spin[ns];
	ntotal++;
      }
    }
    fprintf(output, "%lf %lf\n", T, (double)nmag/ntotal);
  }
  printf("\nOutput file is %s\n",OutputFileName);

  return(0);
}
