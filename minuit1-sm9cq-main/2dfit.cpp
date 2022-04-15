// A simple C++ program to illustrate the use of ROOT class TMinuit
// for function minimization.  The example shows a Maximum Likelihood
// fit for the mean of an exponential pdf in which TMinuit
// minimizes - 2*log(L).   The user must specify what to minimize in the
// function fcn, shown in this file below.

// fcn passes back f = -2*ln L by reference; this is the function to minimize.
// The factor of -2 allows MINUIT to get the errors using the same
// recipe as for least squares, i.e., go up from the minimum by 1.

// TMinuit does not allow fcn to be a member function, and the function
// arguments are fixed, so one of the only ways to bring the data  
// into fcn is to declare a pointer to the data (xVecPtr) as global.

// For more info on TMinuit see root.cern.ch/root/html/TMinuit.html .

// Based on example by
// Glen Cowan
// RHUL Physics
// 4 December 2006

// Update Bob Hirosky: Sep 2013


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include <TMinuit.h>
#include <TRandom2.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include <TH1F.h>
#include <TF1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TF2.h>
#include <TString.h>
#include <TAxis.h>
#include <TLine.h>
#include <TFile.h>
#include "TRandom3.h"
#include "TVirtualFitter.h"
#include "TList.h"

#include <vector>
#include <map>


using namespace std;
// data need to be globals to be visible by fcn 

std::vector<std::pair<double, double> > coords;        
std::vector<double > values;        
std::vector<double > errors; 
// Declare pointer to data as global
// The use of global variables is a disfavored coding practice, but
// in this case it will allow us to avoid writing more complex code
TH1F *hdata;
TH1F *hdata1;
TH2D *h2data;
TH2D *h2data1;

// also passing the fit function, to make the objective fcn more generic
//TF1 *fparam;
TF1 *fparam1;
TF2 *f2param1;

//-------------------------------------------------------------------------
// The pdf to be fitted, here an exponential.
// This is a convenient interface to associating the model
// with a TF1 later



//double gaus fit
double Gaus(double* xPtr, double par[]){
  double x=*xPtr;
  double A1=par[0];
  double mu1=par[1];
  double sig1=par[2];
  double A2=par[3];
  double mu2=par[4];
  double sig2=par[5];

  return A1*exp(-0.5*pow((x-mu1)/sig1,2))+A2*exp(-0.5*pow((x-mu2)/sig2,2));  
}

double Gaus2d(double* xPtr, double *par){//, double* yPtr, double par[]){

   double z1 = double((xPtr[0]-par[1])/par[2]);
   double z2 = double((xPtr[1]-par[3])/par[4]);
   return par[0]*exp(-0.5*(z1*z1+z2*z2));
}   

double SG2D(double* xPtr, double par[]){
   double z1 = double((xPtr[0]-par[1])/par[2]);
   double z2 = double((xPtr[1]-par[3])/par[4]);
   
   double z3 = double((xPtr[0]-par[6])/par[7]);
   double z4 = double((xPtr[1]-par[8])/par[9]);
   return par[0]*exp(-0.5*(z1*z1+z2*z2))+par[5]*exp(-0.5*(z3*z3+z4*z4));

}
  


double Gumbel(double* xPtr, double par[]){
  double x=*xPtr;
  double a=par[0];
  double b=par[1];
  double xbar=par[2];
  double A=par[3];
  double z=(x-xbar)/a;
  double exponent=-b*(z+exp(-z));
  return A*exp(exponent)*1/a;
}

  
double my2Dfunc(double *x, double *par) {
   double *p1 = &par[0];
   double *p2 = &par[5];
   return Gaus2d(x,p1) + Gaus2d(x,p2);
}

//-------------------------------------------------------------------------
// return NLL given a histogram and function

double calcNLL(TH2D* h, TF2* f){ //TH1F
  double nll=0;
  for (int i=1; i<=h->GetNbinsX(); i++){
    for (int j=1; j<h -> GetNbinsY(); j++){
    double x=h->GetXaxis()->GetBinCenter(i);
    double y=h->GetYaxis()->GetBinCenter(j);
    int n=(int)(h->GetBinContent(i,j));
    double mu=f->Eval(x,y);
    if (mu<1e-10) mu=1e-10;    // avoid log(0) problems!
    nll -= n * TMath::Log(mu) - mu  - TMath::LnGamma(n+1);
    }
  
  }
  // cout << "nll "<< nll <<endl;
  return 2*nll;   // factor of -2 so minuit gets the errors right
}


// return Chi2 given a histogram and function
double calcChi2(TH2D* h, TF2* f){
  double chi2=0;
  double delta=0;
  for (int i=1;i<=h->GetNbinsX();i++){
  double  x=h->GetBinCenter(i);
double  n=h->GetBinContent(i);
double mu=f->Eval(x);
double er=h->GetBinError(i);
if (er>0){
delta=(n-mu)/er;
        //cout<<"delta : "<<delta<<endl;
}
chi2+=delta*delta;
  }
  return chi2;
}



//-------------------------------------------------------------------------
// Minuit fcn: calculates value of the function to be minimized using
// the data and the model function
// This is the interface used to define our objective function.  We use
// only a sub set of the input parameters below.
// npar: number of parameters
// par: array of aprameter values
// f: the value of the objective function

void fcn(int& npar, double* deriv, double& f, double par[], int flag){
  for (int i=0; i<npar; i++){
    f2param1->SetParameter(i,par[i]);
  }
  f = calcNLL(h2data,f2param1);
 
}

void myFcn(Int_t & /*nPar*/, Double_t * /*grad*/ , Double_t &fval, Double_t *p, Int_t /*iflag */  )
{
  int n = coords.size();
  double chi2 = 0; 
  double tmp,x[2]; 
  for (int i = 0; i <n; ++i ) { 
    x[0] = coords[i].first;
    x[1] = coords[i].second;
    chi2 -= values[i]*TMath::Log(my2Dfunc(x,p)) - my2Dfunc(x,p) - TMath::LnGamma(values[i]+1);
  }

  fval = 2*chi2;
}
  

/*void fcn(int& npar, double* deriv, double& f, double par[], int flag){
 
  for (int i=0; i<npar; i++){
    fparam1->SetParameter(i,par[i]);
  }
 
  f = calcChi2(hdata,fparam1);
  //cout<<"CCC : "<<f<<endl;

}
*/

//-------------------------------------------------------------------------

int main(int argc, char **argv) {

  // This allows you to view ROOT-based graphics in your C++ program
  // If you don't want view graphics (eg just read/process/write data files),
  // this line can be removed
  TApplication theApp("App", &argc, argv);
  /*
  TCanvas* canvas = new TCanvas();
  canvas->Divide(2,1);
  canvas->cd(1);
  // ***************************************
  // not important for this example
  // Set a bunch of parameters to make the plot look nice
  canvas->SetFillColor(0);
  canvas->UseCurrentStyle();
  canvas->SetBorderMode(0);        
  canvas->SetFrameBorderMode(0);  
  //gROOT->SetStyle("Plain");
  canvas->UseCurrentStyle();
  gROOT->ForceStyle();

  gStyle->SetOptStat(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleSize(0.04);
  gStyle->SetTitleFont(42, "hxy");      // for histogram and axis titles
  gStyle->SetLabelFont(42, "xyz");      // for axis labels (values)
  gROOT->ForceStyle();
  // ***************************************

  */
  // Use this to generate some random data
  TRandom2 r(0);
 
  // create a histogram and fill it w/ randomly distributed data
  // based on an exponential PDF
 /* double xmin = 0.0;
  double xmax = 5.0;
  int nentries = 1000;
  TH1F *hexp=new TH1F("hexp","exponential distribution;x;# events",100,xmin,xmax);
*/

  TFile* tf=new TFile("fitInputs.root");
  TH2D* h1=(TH2D*)tf->Get("hdata");
  TH2D* hbkg=(TH2D*)tf->Get("hbkg");
  TCanvas* c2 = new TCanvas();
  c2->Divide(2,2);
  c2->cd(1);
  h1->Draw("lego");
  //c2->cd(2);
  //hbkg->Draw("lego");
  
  double xmin1 = 0;//h1->GetXaxis()->GetXmin();
  double xmax1 = 6;//h1->GetXaxis()->GetXmax();  
  double ymin1 = 0;//h1->GetYaxis()->GetXmin();
  double ymax1 = 6;//h1->GetYaxis()->GetXmax(); 
  
  // Initialize minuit, set initial values etc. of parameters.
  const int npar = 10;              // the number of parameters
 
  TMinuit minuit(npar);
  minuit.SetFCN(fcn);              // the fcn to be minized (objective fcn)
  // this calculates chi^2 or NLL, it is not the parameterization of the data!

  // Make a TF1, we can use this to fit the data and plot the results
  TF2* myfunc = new TF2("myfunc", SG2D, xmin1, xmax1,ymin1,ymax1, npar);  //Gaus2d//my2Dfunc
  double par[npar];               // the free parameters
  double stepSize[npar];          // (limiting) step sizes for parameters
  double minVal[npar];            // minimum/maximum bounds on parameters
  double maxVal[npar];            
  TString parName[npar];          // Optional names for nicer output!

  // Initial parametes MUST be set by some means to get things started
  par[0] = h1->GetMaximum();
  par[1] = 3.5;//h1->GetXaxis()->GetMean();       // guesses for starting the fit
  par[2] = 2;
  par[3] = 1;//h1->GetYaxis()->GetMean();
  par[4] = 1;

  par[5] = 3;
  par[6] = 1;
  par[7] = 6;
  par[8] = 4;
  par[9] = 3;
  
                     
  stepSize[0] = TMath::Abs(par[0]*0.1);   // usually 10% of initial guess is OK for starting step size, YMMV
  stepSize[1] = TMath::Abs(par[1]*0.1);   // step sizes MUST be positive!
  stepSize[2] = TMath::Abs(par[2]*0.1);
  stepSize[3] = TMath::Abs(par[3]*0.1);
  stepSize[4] = TMath::Abs(par[4]*0.1);
  //
  stepSize[5] = TMath::Abs(par[5]*0.1);
  stepSize[6] = TMath::Abs(par[6]*0.1);
  stepSize[7] = TMath::Abs(par[7]*0.1);
  stepSize[8] = TMath::Abs(par[8]*0.1);
  stepSize[9] = TMath::Abs(par[9]*0.1);

  minVal[0] = 0;      // if min and max values = 0, parameter is unbounded.
  maxVal[0] = 0;
  minVal[1] = 0;
  maxVal[1] = 0;
  minVal[2] = 0;
  maxVal[2] = 0;
  minVal[3] = 0;
  maxVal[3] = 0;
  minVal[4] = 0;
  maxVal[4] = 0;
  //
  minVal[5] = 0;
  maxVal[5] = 0;
  minVal[6] = 0;
  maxVal[6] = 0;
  minVal[7] = 0;
  maxVal[7] = 0;
  minVal[8] = 0;
  maxVal[8] = 0;
  minVal[9] = 0;
  maxVal[9] = 0;

  parName[0] = "Normalization";
  parName[1] = "Mean x";       // let's give our fit parameters useful names
  parName[2] = "SD x";
  parName[3] = "Mean y";
  parName[4] = "SD y";
  //
  parName[5] = "Bkg";
  parName[6] = "BkgMean x";       // let's give our fit parameters useful names
  parName[7] = "BkgSD x";
  parName[8] = "BkgMean y";
  parName[9] = "BkgSD y";

  // initialize the parameters
  for (int i=0; i<npar; i++){
    minuit.DefineParameter(i, parName[i].Data(),
  par[i], stepSize[i], minVal[i], maxVal[i]);
  }

  // here we define the pointers to pass information to Minuit's fcn
  // not pretty, but simple
  h2data=h1;     // histogram to fit
  f2param1=myfunc;  // our model

  // Do the minimization!
  minuit.Migrad();       // Minuit's best minimization algorithm

  // Get the result
  double outpar[npar], err[npar];
  for (int i=0; i<npar; i++){
    minuit.GetParameter(i,outpar[i],err[i]);
  }
  // run a minos error analysis
  // this perfoms a brute force scan around the minimum of the objective function
  // to better estimates of the errors on the fit parameters
  gMinuit->mnmnos();

  // store the fit parameters in out TF1, decorate our function
  myfunc->SetParameters(outpar);

  myfunc->SetLineStyle(1);             //  1 = solid, 2 = dashed, 3 = dotted
  myfunc->SetLineColor(1);             //  black (default)
  myfunc->SetLineWidth(2);

  myfunc->GetXaxis()->SetTitle("x");
  myfunc->GetYaxis()->SetTitle("f(x;#lambda)");

  //PValue
  //double chi2Gaus = calcChi2(hdata,fparam1);
  //cout <<"Chi2 : "<<chi2Gaus<< "   PValue Gaus Fit: "<<TMath::Prob(chi2Gaus,94)<<endl;
  TH2D *hfit=new TH2D("hfit","hfit",h1->GetNbinsX(),0,6,h1->GetNbinsY(),0,6);
  hfit->FillRandom("myfunc", 10000);//1000000
  TH1D* sub = (TH1D*) hfit->Clone("sub");
  sub->Add(h1,-1);
  // Plot the result.
  c2->cd(2);
  //myfunc->SetTitle("My Function");
  //h1->Draw("lego");
  //myfunc->Draw("lego");
  hfit->Draw("lego");
  c2->cd(3);
  for (int i=1; i<=sub->GetNbinsX(); i++){
    for (int j=1; j<sub -> GetNbinsY(); j++){
      double v=sub->GetBinContent(i,j);
      sub->SetBinContent(i,j,abs(v));
    }
  
  }
  sub->Draw("lego");
  
  //hdata-fit of bkg
  //TF2* bkg = new TF2("bkg", Gaus2d, xmin1, xmax1,ymin1,ymax1, 5);
  TF2* signal = new TF2("signal", SG2D, xmin1, xmax1,ymin1,ymax1, npar);  //Gaus2d//my2Dfunc
  signal->SetParameters(outpar);
  signal->SetParameters(0,0,0,0,0,outpar[5],outpar[6],outpar[7],outpar[8],outpar[9]);
  TF2* bkg = new TF2("bkg", SG2D, xmin1, xmax1,ymin1,ymax1, npar);  //Gaus2d//my2Dfunc
  bkg->SetParameters(outpar);
  bkg->SetParameters(outpar[0],outpar[1],outpar[2],outpar[3],outpar[4],0,0,0,0,0);
  c2->cd(4);
  TH2D *hfitb=new TH2D("hfitb","hfitb",h1->GetNbinsX(),0,6,h1->GetNbinsY(),0,6);
  hfitb->FillRandom("bkg", 10000);//1000000
  TH1D* hsignal = (TH1D*) h1->Clone("hsignal");
  hsignal->Add(hfitb,-1);
  for (int i=1; i<=hsignal->GetNbinsX(); i++){
    for (int j=1; j<hsignal-> GetNbinsY(); j++){
      double v=hsignal->GetBinContent(i,j);
      hsignal->SetBinContent(i,j,abs(v));
    }
  
  }
  hsignal->Draw("lego");
  //bkg->Draw("lego");
  //signal->Draw("lego","same");
  TCanvas* c1 = new TCanvas();
  c1->Divide(3,1);
  c1->cd(1);
  myfunc->Draw("lego");
  c1->cd(2);
  bkg->Draw("lego");
  c1->cd(3);
  signal->Draw("lego");
  //c1->Draw();
  Double_t num=hsignal->Integral();//0,6,0,6);
  double er=sqrt(num);
  cout << "Total number="<< num <<endl;
  
  cout << "Error="<< er <<endl;
  

  
  
  //////////////////////////////////////////////////////////////////



  cout << "\nTo exit, quit ROOT from the File menu of the plot (or use control-C)" << endl;
			      
  theApp.SetIdleTimer(300,".q");  // set up a failsafe timer to end the program
  theApp.Run(true);
  c2->Close();

  return 0;

}
