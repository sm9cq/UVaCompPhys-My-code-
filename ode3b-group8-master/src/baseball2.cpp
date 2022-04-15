///
/// Starter template for second baseball problem
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
#include "TCanvas.h"
#include "TAxis.h"
#include "TMultiGraph.h"

using namespace std;

double B=4.1e-4;
double w=189;
double g=9.81;
double v0=38;
double theta0=M_PI/180;
double psi=0;

//xv
double f_rx(double x,const vector<double> &y,void *params=0){
  (void)x;
  return y[1];
}
//xa
double f_vx(double x,const vector<double> &y,void *params){
  (void) x;
  double v=sqrt(y[1]*y[1]+y[3]*y[3]+y[5]*y[5]);
  double Fv=0.0039+(0.0058/(1+exp((v-35)/5)));
  return (-Fv*v*y[1]+B*w*(y[5]*sin(psi)-y[3]*cos(psi)));
}

//yv
double f_ry(double x,const vector<double> &y,void *params=0){
  (void)x;
  return y[3];
}

//ya
double f_vy(double x,const vector<double> &y,void *params){
  (void) x;
  double v=sqrt(y[1]*y[1]+y[3]*y[3]+y[5]*y[5]);
  double Fv=0.0039+(0.0058/(1+exp((v-35)/5)));
  return (-Fv*v*y[3]+B*w*y[1]*cos(psi));
}

//zv
double f_rz(double x,const vector<double> &y,void *params=0){
  (void)x;
  return y[5];
}

//za
double f_vz(double x,const vector<double> &y,void *params){
  (void) x;
  double v=sqrt(y[1]*y[1]+y[3]*y[3]+y[5]*y[5]);
  double Fv=0.0039+(0.0058/(1+exp((v-35)/5)));
  return (-g-Fv*v*y[5]-B*w*y[1]*sin(psi));
}

double f_stop(double x, const vector<double> &y){
  (void) x;
  if (y[0]>18.27) return 1;
  return 0;
}  


int main(int argc, char **argv){


  // we have 6 initial conditions for this problem
  // y[0] = y[2] = y[4] = 0;  // init x,y,z
  // y[1] = v0*cos(theta0);   // vx  "x is line towards the plate
  // y[3] = 0;                // vy  "y" is measured as left/right divergence from line to plate
  // y[5] = v0*sin(theta0);   // vz  "z" is vertival measure
 vector<pfunc_t> v_fun(6); 
  v_fun[0]=f_rx;
  v_fun[1]=f_vx;
  v_fun[2]=f_ry;
  v_fun[3]=f_vy;
  v_fun[4]=f_rz;
  v_fun[5]=f_vz;
  vector<double> y0(6);


bool showPlot=true;
  // pitches
  // slider ip=0
  // curve ip=1
  // screwball ip=2
  // fast ip=3
  int ip=1;    // default pitch
  int c;
  while ((c = getopt (argc, argv, "p:n")) != -1)
    switch (c) {
    case 'p':
      ip = atoi(optarg);
      break;
    case 'n':
      showPlot=true;
      break;
    }

  TString title;
  if (ip==0){
    cout << "Setting up initial conditions for slider" << endl;
    //SetupSlider(y0);
    w=189;
    v0=38;
    psi=0;

  }
  else if (ip==1){
    cout << "Setting up initial conditions for curveball" << endl;
    //SetupCurve(y0);
    w=189;
    v0=38;
    psi=M_PI/4;
  }
  else if (ip==2){
    cout << "Setting up initial conditions for screwball" << endl;
    //SetupScrewball(y0);
   w=189;
    v0=38;
    psi=M_PI*3/4;
  }
  else {
    cout << "Setting up initial conditions for fastball" << endl;
    //SetupFastball(y0);
    w=189;
    v0=38;
    psi=M_PI*5/4;
  }

  TApplication theApp("App", &argc, argv); // init ROOT App for displays

  double xend=60;   // feet
  double yend=0;    // tbd
  double zend=0;    // tbd
  double vxend=0;
  double vyend=0;
  double vzend=0;


double xp[50000];
double yp[50000];
double zp[50000];


 y0[0]=0;
 y0[1]=v0*cos(theta0);
 y0[2]=0;
 y0[3]=0;
 y0[4]=0;
 y0[5]=v0*sin(theta0);
    
  double t=0;
  double h=0.0001;
  int i=0;

  do{
	y0=RK4StepN(v_fun,y0,t,h);
	t+=h;
	
	xp[i]=y0[0]/0.3048;
	yp[i]=y0[2]/0.3048;
	zp[i]=y0[4]/0.3048;
	i+=1;

  }while (f_stop(t,y0)==0);
  xend=y0[0]/0.3048;
  yend=y0[2]/0.3048;
  zend=y0[4]/0.3048;
  vxend=y0[1];
  vyend=y0[3];
  vzend=y0[5];
  // printf("time = (%lf)\n",t);

  // to compare to the plots in Fitzpatrick, output your results in **feet**
  // do not change these lines
  printf("********************************\n");
  printf("Coordinates when x=60 feet\n");
  printf("(x,y,x) = (%lf,%lf,%lf)\n",xend,yend,zend);
  printf("(vx,vy,vz) = (%lf,%lf,%lf)\n",vxend,vyend,vzend);
  printf("********************************\n");

  TCanvas*c1= new TCanvas("c1","",1200,900);
  //c1->Divide(2,2);
    
TMultiGraph *mg=new TMultiGraph();

    TGraph*mg1= new TGraph(i,xp,yp);
    TGraph*mg2= new TGraph(i,xp,zp);
    mg1->SetLineStyle(2);
    
      c1->Draw();
    if (ip==0){
      // c1->cd(1);
      //mg->Add(mg1,"C");
      //mg->Add(mg2,"C");
      mg->SetTitle("Slider");
    }
    else if (ip==1){
      // c1->cd(2);
      //mg->Add(mg1,"C");
      // mg->Add(mg2,"C");
      mg->SetTitle("Curveball");
    }
    else if (ip==2){
      //  c1->cd(3);
      //mg->Add(mg1,"C");
      //mg->Add(mg2,"C");
      mg->SetTitle("Screwball");
    }
    else if (ip==3){
      //c1->cd(4);
      //mg->Add(mg1,"C");
      //mg->Add(mg2,"C");
      mg->SetTitle("Fastball");
    }
    //  c1->Draw();
      mg->Add(mg1,"C");
     mg->Add(mg2,"C");

     mg->Draw("AC");

    mg->GetXaxis()->SetTitle("x (ft)");
    mg->GetYaxis()->SetTitle("y (ft) / z (ft)");

   
    
  // plot the trajectory.  See Fitzpatrick for plot details
  if (showPlot){

 
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    theApp.Run();
  }
  
  return 0;
}

