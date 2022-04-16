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
#include <TString.h>
#include <TAxis.h>
#include <TLine.h>
#include <TFile.h>

using namespace std;

// Declare pointer to data as global
// The use of global variables is a disfavored coding practice, but
// in this case it will allow us to avoid writing more complex code
TH1F *hdata;
TH1F *hdata1;

// also passing the fit function, to make the objective fcn more generic
//TF1 *fparam;
TF1 *fparam1;


//-------------------------------------------------------------------------
// The pdf to be fitted, here an exponential.
// This is a convenient interface to associating the model
// with a TF1 later
/*double expPdf(double* xPtr, double par[]){        
  double x = *xPtr;
  double A = par[0];         // normalization
  double lam = par[1];       // mean of x
  double f = 0;
  f = A/fabs(lam) * exp(x/lam);
  return f;
}*/


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

//-------------------------------------------------------------------------
// return NLL given a histogram and function

double calcNLL(TH1F* h, TF1* f){
  double nll=0;
  for (int i=1; i<=h->GetNbinsX(); i++){
    double x=h->GetBinCenter(i);
    int n=(int)(h->GetBinContent(i));
    double mu=f->Eval(x);
    if (mu<1e-10) mu=1e-10;    // avoid log(0) problems!
    nll -= n * TMath::Log(mu) - mu  - TMath::LnGamma(n+1);
  }
  // cout << "nll "<< nll <<endl;
  return 2*nll;   // factor of -2 so minuit gets the errors right
}


// return Chi2 given a histogram and function
double calcChi2(TH1F* h, TF1* f){
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

/*void fcn(int& npar, double* deriv, double& f, double par[], int flag){

  for (int i=0; i<npar; i++){
    fparam->SetParameter(i,par[i]);
  }

  f = calcNLL(hdata,fparam);
 
}*/

void fcn(int& npar, double* deriv, double& f, double par[], int flag){
 
  for (int i=0; i<npar; i++){
    fparam1->SetParameter(i,par[i]);
  }
 
  f = calcChi2(hdata,fparam1);
  //cout<<"CCC : "<<f<<endl;

}


//-------------------------------------------------------------------------

int main(int argc, char **argv) {

  // This allows you to view ROOT-based graphics in your C++ program
  // If you don't want view graphics (eg just read/process/write data files),
  // this line can be removed
  TApplication theApp("App", &argc, argv);

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


  // Use this to generate some random data
  TRandom2 r(0);
 
  // create a histogram and fill it w/ randomly distributed data
  // based on an exponential PDF
 /* double xmin = 0.0;
  double xmax = 5.0;
  int nentries = 1000;
  TH1F *hexp=new TH1F("hexp","exponential distribution;x;# events",100,xmin,xmax);
*/

  TFile* tf=new TFile("distros.root");
  TH1F* h1=(TH1F*)tf->Get("dist1");
 // hh->Draw();

  double xmin1 = 0;
  double xmax1 = 200;  

/*  for (int i=0; i<nentries; i++){
    hexp->Fill(r.Exp(3.14159));
  }
*/  
 
  // Initialize minuit, set initial values etc. of parameters.
  const int npar = 6;              // the number of parameters
 
  TMinuit minuit(npar);
  minuit.SetFCN(fcn);              // the fcn to be minized (objective fcn)
  // this calculates chi^2 or NLL, it is not the parameterization of the data!

  // Make a TF1, we can use this to fit the data and plot the results
  TF1* myfunc = new TF1("myfunc", Gaus, xmin1, xmax1, npar);  
  double par[npar];               // the free parameters
  double stepSize[npar];          // (limiting) step sizes for parameters
  double minVal[npar];            // minimum/maximum bounds on parameters
  double maxVal[npar];            
  TString parName[npar];          // Optional names for nicer output!

  // Initial parametes MUST be set by some means to get things started
  par[0] = h1->GetMaximum();
  par[1] = h1->GetMean();       // guesses for starting the fit
  par[2] = 6;
  par[3] = par[0]*0.1;
  par[4] = par[1]+6;
  par[5] = 8;
                     
  stepSize[0] = TMath::Abs(par[0]*0.1);   // usually 10% of initial guess is OK for starting step size, YMMV
  stepSize[1] = TMath::Abs(par[1]*0.1);   // step sizes MUST be positive!
  stepSize[2] = TMath::Abs(par[2]*0.1);
  stepSize[3] = TMath::Abs(par[3]*0.1);
  stepSize[4] = TMath::Abs(par[4]*0.1);
  stepSize[5] = TMath::Abs(par[5]*0.1);


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
  minVal[5] = 0;
  maxVal[5] = 0;

  parName[0] = "Amp 1";
  parName[1] = "mean 1";       // let's give our fit parameters useful names
  parName[2] = "standard deviation 1";
  parName[3] = "Amp 2";
  parName[4] = "mean 2";
  parName[5] = "standard deviation 2";

  // initialize the parameters
  for (int i=0; i<npar; i++){
    minuit.DefineParameter(i, parName[i].Data(),
  par[i], stepSize[i], minVal[i], maxVal[i]);
  }

  // here we define the pointers to pass information to Minuit's fcn
  // not pretty, but simple
  hdata=h1;     // histogram to fit
  fparam1=myfunc;  // our model

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
  double chi2Gaus = calcChi2(hdata,fparam1);
  cout <<"Chi2 : "<<chi2Gaus<< "   PValue Gaus Fit: "<<TMath::Prob(chi2Gaus,94)<<endl;
 
  // Plot the result.
  hdata->SetTitle("Sum of two Gaussians Fit");
  hdata->Draw("e");
  myfunc->Draw("same");
  

  //////////////////////////////////////////////////////////////////


  canvas->cd(2);
 
  TFile* tf1=new TFile("distros.root");
  TH1F* h2=(TH1F*)tf1->Get("dist1");

  double xmin2 = 0;
  double xmax2 = 200;  
 
  const int npar1 = 4;              // the number of parameters
 
  TMinuit minuit1(npar1);
  minuit1.SetFCN(fcn);              
  TF1* myfunc1 = new TF1("myfunc1", Gumbel, xmin2, xmax2, npar1);  
  double par1[npar1];               // the free parameters
  double stepSize1[npar1];          // (limiting) step sizes for parameters
  double minVal1[npar1];            // minimum/maximum bounds on parameters
  double maxVal1[npar1];            
  TString parName1[npar1];          // Optional names for nicer output!

  // Initial parametes MUST be set by some means to get things started
  par1[0] = 7;
  par1[1] = 1;       // guesses for starting the fit
  par1[2] = h2->GetMean();
  par1[3] = 16000;
                     
  stepSize1[0] = TMath::Abs(par1[0]*0.1);   // usually 10% of initial guess is OK for starting step size, YMMV
  stepSize1[1] = TMath::Abs(par1[1]*0.1);   // step sizes MUST be positive!
  stepSize1[2] = TMath::Abs(par1[2]*0.1);
  
  stepSize1[3] = TMath::Abs(par1[3]*0.1);

  minVal1[0] = 0;      // if min and max values = 0, parameter is unbounded.
  maxVal1[0] = 0;
  minVal1[1] = 0;
  maxVal1[1] = 0;
  minVal1[2] = 0;
  maxVal1[2] = 0;
  minVal1[3] = 0;
  maxVal1[3] = 0;

  parName1[0] = "b";
  parName1[1] = "a";       // let's give our fit parameters useful names
  parName1[2] = "mean";
  parName1[3] = "Amp";  

  // initialize the parameters
  for (int i=0; i<npar1; i++){
    minuit1.DefineParameter(i, parName1[i].Data(),
  par1[i], stepSize1[i], minVal1[i], maxVal1[i]);
  }

  // here we define the pointers to pass information to Minuit's fcn
  // not pretty, but simple
  hdata1=h2;     // histogram to fit
  fparam1=myfunc1;  // our model

  // Do the minimization!
  minuit1.Migrad();       // Minuit's best minimization algorithm

  // Get the result
  double outpar1[npar1], err1[npar1];
  for (int i=0; i<npar1; i++){
    minuit1.GetParameter(i,outpar1[i],err1[i]);
  }
  // run a minos error analysis
  // this perfoms a brute force scan around the minimum of the objective function
  // to better estimates of the errors on the fit parameters
  gMinuit->mnmnos();

  // store the fit parameters in out TF1, decorate our function
  myfunc1->SetParameters(outpar1);

  myfunc1->SetLineStyle(1);             //  1 = solid, 2 = dashed, 3 = dotted
  myfunc1->SetLineColor(1);             //  black (default)
  myfunc1->SetLineWidth(2);

  myfunc1->GetXaxis()->SetTitle("x");
  myfunc1->GetYaxis()->SetTitle("f(x;#lambda)");

  // Plot the result.
  hdata1->SetTitle("Gumbel Fit");
  hdata1->Draw("e");
  myfunc1->Draw("same");

  double chi2Gumbel = calcChi2(hdata,fparam1);
  //cout<<"xbins : " <<hdata1->GetNbinsX()<<endl;
  cout <<"chi2 : " <<chi2Gumbel <<"   PValue Gumbel Fit: "<<TMath::Prob(chi2Gumbel,96)<<endl; 

  canvas->Draw();

  cout << "\nTo exit, quit ROOT from the File menu of the plot (or use control-C)" << endl;
  theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program
  theApp.Run(true);
  canvas->Close();

  return 0;

}
