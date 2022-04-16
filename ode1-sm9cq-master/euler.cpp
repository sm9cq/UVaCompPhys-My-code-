#include <iostream>
#include <getopt.h>
using namespace std;


void usage(char **argv){
  fprintf(stderr, "\nUsage: %s [options]\n",argv[0]);
  fprintf(stderr, " -v vinit: initial speed [default 10 m/s]\n");
  fprintf(stderr, " -a theta0: initial angle above horizontal [45deg]\n");
  fprintf(stderr, " -t tstep: time step for approximation [0.01s]\n");
  fprintf(stderr, " -h: print this message\n");
}

int main(int argc, char **argv) {
  double vinit=10;    // m/s
  double theta0=45;   // deg
  double dt=0.01;     // time step [s]
  int opt;
  while ((opt = getopt(argc, argv, "v:a:t:h")) != -1) {
    switch (opt) {
    case 'v':
      vinit = atof(optarg);
      break;
    case 'a':
      theta0=atof(optarg);
      break;
    case 't':
      dt=atof(optarg);
      break;
    case 'h':
      usage(argv);
      return 0;
      break;
    default: /* '?' */
      ;
    }
  }

  printf("Simulating projectile motion with params:\n");
  printf("(vinit,theta0,dt)=(%7.2lf,%7.2lf,%7.2f)\n",vinit,theta0,dt);
  

  // fill in the blanks below

  // given the boundary conditions at t=0, namely:
  // x(0) = y(0) = 0
  // vx(0) = vinit * cos(theta0) , vy(0) = vinit * sin(theta0)
  // impliment Euler's method for stepping forward in time to calculate the
  // new position of the projectile in steps of dt
  // your simulation should stop at the last time step before y goes negative

  // eg, write out a data file containing at least the following information
  // t   x(t)   y(t)   vx(t)   vy(t)  
  
  
}


