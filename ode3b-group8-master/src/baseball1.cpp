///
/// Starter template for first baseball problem
/// Solve for the initial speed of the pitch given the initial parameters
/// xend : distance to home plate [18.5] m
/// z0 : height of release of ball [1.4] m
/// theta0 : angle of release above horizontal [1] degree
///
///  Do not change the interface for running the program
///  Fill in the value of vPitch in the print statement with your solution
///  at the end of main()
///

#include "RKn.hpp"
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGClient.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

struct Params {
  double g;   // acceleration [m/s^2]
  double m;   // mass of object [kg], nb proj. In vacuum funcs do not depend on the mass
  double d;   // m diameter of ball
  double b;   // b,c params for air resistance
  double c;
};

double g = 9.81;
double m=0.145;    
double d=0.0075;   
double b=1.6e-4;  
double c=0.25;
  
double xend=18.5;       // meters to plate
double z0=1.4;             // height of release [m]
double theta0=1;  

double f_ri(double x, const vector<double> &y, void *params=0){ 
  (void) x;   // prevent unused variable warning
  return y[1];
}


double f_vi(double x, const vector<double> &y, void *params){ 
    (void) x;
    return -((b*d* sqrt(y[1]*y[1] + y[3]*y[3])+c*d*d*(y[1]*y[1]+y[3]*y[3])) / m)*(y[1]/sqrt(y[1]*y[1] + y[3]*y[3]));
  	// return 0;  // if no air, no forces/acceleration along i direction in this problem
  }

double f_rj(double x, const vector<double> &y, void *params=0){  
  	(void) x;   // prevent unused variable warning
  	return y[3];
  }
double f_vj(double x, const vector<double> &y, void *params){  
  	(void) x;
  	return -((b*d*sqrt(y[1]*y[1] + y[3]*y[3])+c*d*d*(y[1]*y[1]+y[3]*y[3])) / m)*(y[3]/sqrt(y[1]*y[1] + y[3]*y[3]))- g;

  }



int main(int argc, char **argv){

  // examples of parameters
  Params pars;
  pars.g=9.81;
  pars.m=0.145;    
  pars.d=0.0075;   
  pars.b=1.6e-4;  
  pars.c=0.25;
  void *p_par = (void*) &pars;

  double xend=18.5;       // meters to plate
  double z0=1.4;             // height of release [m]
  double theta0=1;         // angle of velocity at release (degrees)
                                      // convert to radians before using!
  bool showPlot=false;    // keep this flag false by default
  
  // allow changing the parameters from the command line
  int c;
  while ((c = getopt (argc, argv, "x:z:t:p")) != -1)
    switch (c) {
    case 'x':
      xend = atof(optarg);
      break;
    case 'z':
      z0 = atof(optarg);
      break;
    case 't':
      theta0 = atof(optarg);
      break;
    case 'p':
      showPlot=true;   
    case '?':
      fprintf (stderr, "Unknown option `%c'.\n", optopt);
    }
  TApplication theApp("App", &argc, argv); // init ROOT App for displays

  
  double vPitch = 0;   // m/s of pitch needed to land in strike zone at 0.9 meters
  // write code to solve for vPitch here
  //while(){//while x-0.9 at y=0 < 0.00001
  vector<pfunc_t> v_fun(4);   // 4 element vector of function pointers
  v_fun[0]=f_ri;
  v_fun[1]=f_vi;
  v_fun[2]=f_rj;
  v_fun[3]=f_vj;

  vector<double> y(4);
  y[0]=0;   // init position on i-axis
  y[1]=0;  // init velocity along i axis
  y[2]=z0;   // repeat for j-axis
  y[3]=0;

 double v0=0;
 double i=0.1;
  while(i<500){
    v0=0.1*i;
    y[0]=0;   // init position on i-axis
    y[1]=v0*cos(theta0*3.14159/180);  // init velocity along i axis
    y[2]=1.4;   // repeat for j-axis
    y[3]=v0*sin(theta0*3.14159/180);
    
    double t=0;
    double h=0.001;  // step size  
    do {
      y=RK4StepN(v_fun,y,t,h);
      t+=h;
      if(y[2]<=0){
        break;}
	}while(18.49>=y[0]); 
        
	if (0.89<=y[2]){ 
     
      vPitch=v0;
      break;}

    
    i=i+1;
  }
  
  
  // do not change these lines
  printf("********************************\n");
  printf("(xend,z0,theta0) = (%lf,%lf,%lf)\n",xend,z0,theta0);
  printf("v_pitch = %lf m/s\n",vPitch);
  printf("********************************\n");

  if (showPlot){
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    theApp.Run();
  }
  
  return 0;
}
