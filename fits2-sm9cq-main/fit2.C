#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TRandom2.h"
#include "TROOT.h"
// fit_user.C
// example of fitting with a user defined function

// A function producing two peaks on top of an exponentialy 
// falling background.  Depends on several parameters.
// Note: this need not be a 1D function
// Generic interface for fcn of n input-values and m parameters
// Functions with this interface may be used to construct a "TFunction"
Double_t myfunction(Double_t *xin, Double_t *par) {
  Double_t x=xin[0];
  Double_t bkgScale=par[0];
  Double_t alpha=par[1];
  Double_t beta=par[2];
  Double_t background =TMath::Exp(-0.5*(x-alpha)*(x-alpha)/beta/beta);// pow(x/beta,-1.0*alpha);
  Double_t A1=par[3];
  Double_t mu1=par[4];
  Double_t sig1=par[5];
  Double_t peak1=A1*TMath::Exp(-0.5*(x-mu1)*(x-mu1)/sig1/sig1);
  Double_t A2=par[6];
  Double_t mu2=par[7];
  Double_t sig2=par[8];
  Double_t peak2=A2*exp(-0.5*(x-mu2)*(x-mu2)/sig2/sig2);
//Double_t A3=par[9];
  //Double_t mu3=par[10];
  //Double_t sig3=par[11];
  //Double_t peak3=A3*exp(-0.5*(x-mu3)*(x-mu3)/sig3/sig3);
  Double_t peaks=par[9]*sin(par[10]*x)*exp(-0.5*(x-par[9])*(x-par[9])/par[10]/par[10]);
  return bkgScale*background+peak1+peak2+peaks;
  /*Double_t x=xin[0];
  Double_t bkgScale=par[0];
  Double_t alpha=par[1];
  Double_t beta=par[2];
  Double_t A1=par[3];
  Double_t mu1=par[4];
  Double_t sig1=par[5];
  Double_t peak1=A1*TMath::Exp(-0.5*(x-mu1)*(x-mu1)/sig1/sig1);
  Double_t omega=par[6];
  Double_t peaks=alpha*sin(omega*x+beta);
  return bkgScale+peaks+peak1;*/
}


void fit2(int entries=100000) {
   //Simple histogram fitting examples

  //TFile *f=new TFile("data1.root","recreate");
  auto tf=new TFile("datadist.root");
  
  TH1F *h1=(TH1F*)tf->Get("h");
  
  /////////////////////////////////////////////////////
  // Here we simulate some physics process
  /////////////////////////////////////////////////////
  // define a TF1 using 9 parameters
  TF1 *f1 = new TF1("f1",myfunction,0,12,11);//300,1000,9);
  // set the parameter values
  f1->SetParameters(10,17,5,2,20,5,8,9,10,8,10);
  // fill a histogram with random data using f1 as a pdf
  /*TH1F *ranHist1 = new TH1F("ranHist1", "Random Histogram",500,300,1000);
  ranHist1->FillRandom("f1",entries);
  ranHist1->Draw("e");*/
  //TFile *f=new TFile("datadist.root");
  //TH1F *h1=(TH1F*)f->Get("h");
  h1->Draw();
  /////////////////////////////////////////////////////
  
  // Now "pretend" that we don't know the paramaters used to generate
  // the data.

  // all fits begin with initial guesses at the best parameter values
  //f1->SetParameters(1e6,4,80,200,550,3,200,700,20);
  f1->SetParameters(1,1,5,1,2,5,8,9,10,10);
  f1->Draw("same");

  // uncomment this section once you have good fit parameters for examples 
  // of accessing details on the fit results

  /**/
  //f1->SetParameters(1,1,1,1,1,1,1,1,1,1);
  f1->SetParameters(7,9,5,10,12,2,9,9,5,8,1.5);
  //f1->SetParameters(20,20,20,20,20,20,20,20,20,20);
  // perform the fit here
  h1->Fit("f1");
  h1->Fit("f1","EV");
  //ranHist1->Fit("f1");
  //ranHist1->Fit("f1","EV");

  // retrieve central parameter values and errors
  TF1 *myfunc = h1->GetFunction("f1");
  myfunc->Print();
  cout << "fit parameters" << endl;
  for (int i=0;i<11;i++) {
  cout << i << "\t" << myfunc->GetParameter(i) 
       << " +- " << myfunc->GetParError(i) << endl;
  }

  
  // get chi^2 pvalue
   Double_t chi2 = myfunc->GetChisquare();  
  cout << "chi2 / ndf: " << chi2 << " / " << myfunc->GetNDF()
       << " reduced chi2: " << chi2/myfunc->GetNDF() << endl;
  cout << " Pvalue: " 
       << TMath::Prob(chi2,myfunc->GetNDF()) << endl;
  
  /**/

  tf->Write();
}

