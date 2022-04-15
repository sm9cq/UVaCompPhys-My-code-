#include "TRandom2.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGClient.h"
#include "Riostream.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"


#include <iostream>
using namespace std;

using TMath::Log;

//parms
const double xmin=1;
const double xmax=20;
const int npoints=12;
const double sigma=0.2;
int nbin=1e3;

double f(double x){
  const double a=0.5;
  const double b=1.3;
  const double c=0.5;
  return a+b*Log(x)+c*Log(x)*Log(x);
}


void getX(double *x){
  double step=(xmax-xmin)/npoints;
  for (int i=0; i<npoints; i++){
    x[i]=xmin+i*step;
  }
}

void getY(const double *x, double *y, double *ey){
  static TRandom2 tr(0);
  for (int i=0; i<npoints; i++){
    y[i]=f(x[i])+tr.Gaus(0,sigma);
    ey[i]=sigma;
  }
}

TMatrixD SolveLSQ(const TMatrixD &A, const TMatrixD &y){
  TMatrixD AT=(A);  
  AT.T();            // A transpose
  TMatrixD ATAi(AT,TMatrixD::kMult,A);
  ATAi.Invert();
  TMatrixD Adag(ATAi,TMatrixD::kMult,AT);  // (A^T A)^(-1) A^T
  TMatrixD theta(Adag,TMatrixD::kMult,y);
  return theta;
}

void leastsq(){
  double x[npoints];
  double y[npoints];
  double ey[npoints];
  getX(x);
  getY(x,y,ey);
  auto tg = new TGraphErrors(npoints,x,y,0,ey);
  tg->Draw("alp");
}

int main(int argc, char **argv){
  TApplication theApp("App", &argc, argv); // init ROOT App for displays

  // ******************************************************************************
  // ** this block is useful for supporting both high and std resolution screens **
  UInt_t dh = gClient->GetDisplayHeight()/2;   // fix plot to 1/2 screen height  
  //UInt_t dw = gClient->GetDisplayWidth();
  UInt_t dw = 1.1*dh;
  // ******************************************************************************
  TCanvas *tc = new TCanvas("c1","DEQ solutions",dw,dh);

  double lx[npoints];
  double ly[npoints];
  double ley[npoints];

  getX(lx);
  getY(lx,ly,ley);
  auto tgl = new TGraphErrors(npoints,lx,ly,0,ley);
  tgl->SetTitle("Pseudoexperiment;x;y");
  
  // An example of one pseudo experiment
  tgl->Draw("alp");
  tc->Draw();

  //Write code here

  // Define vectors and matrices.<br>
  // Make the vectors 'use' the data : they are not copied, 
  // the vector data pointer is just set appropriately
  
  TVectorD x; x.Use(npoints,lx);
  TVectorD y; y.Use(npoints,ly);
  TVectorD e; e.Use(npoints,ley);

  TMatrixD A(npoints,3);       // A matrix

  // Fill the A matrix
  //cannot do Log of vector
  double logx[npoints],log2x[npoints];
  for (int i=0; i < npoints; i++){
    logx[i]=log(x[i]);
    log2x[i]=log(x[i])*log(x[i]);
  }
  TVectorD vlogx; vlogx.Use(npoints,logx);
  TVectorD vlog2x; vlog2x.Use(npoints,log2x);
  TMatrixDColumn(A,0) = 1.0;
  TMatrixDColumn(A,1) = vlogx;//Log(x);
  TMatrixDColumn(A,2) = vlog2x;//Log(x)*Log(x);
  cout << "A = ";
  A.Print();

  // apply weights
  TMatrixD yw(A.GetNrows(),1);   // error-weighted data values

  for (Int_t irow = 0; irow < A.GetNrows(); irow++) {
    TMatrixDRow(A,irow) *= 1/e(irow);
    TMatrixDRow(yw,irow) += y(irow)/e(irow);
  }
  cout << "A weighted = ";
  A.Print();
  cout << "y weighted = ";
  yw.Print();

  TMatrixD theta=SolveLSQ(A, yw);
  cout << "Param vector = ";
  theta.Print();
  TF1 *fn1 = new TF1("fn1","[0]+[1]*log(x)+[2]*log(x)*log(x)",xmin,xmax);
//a+b*Log(x)+c*Log(x)*Log(x)


// View the resulting fit
  fn1->SetParameters(theta[0][0],theta[1][0],theta[2][0]);
  tgl->Draw("alp*");
  fn1->Draw("same");
  tc->Draw();

//Let random
  TH1F *a= new TH1F("a","a",nbin,0.0,5.0);
  TH1F *b= new TH1F("b","b",nbin,0.0,5.0);//(TH1F*)a->Clone();
  TH1F* c= new TH1F("c","c",nbin,0.0,5.0);//(TH1F*)a->Clone();
  TH1F* chisq= new TH1F("chisq","chisq",50,0.0,50.0);

  TH2F* ba= new TH2F("ba", "2D b vs a", nbin,0,5,nbin,0,5);
  TH2F* ca= new TH2F("ca", "2D c vs a", nbin,0,5,nbin,0,5);
  TH2F* cb= new TH2F("cb", "2D c vs b", nbin,0,5,nbin,0,5);
  TH1F* rchisq= new TH1F("rchisq","Reduced chisq",50,0.0,5.0);

  for( int k=0;k<nbin;k++){
  double lx[npoints];
  double ly[npoints];
  double ley[npoints];

  getX(lx);
  getY(lx,ly,ley);
  TVectorD x; x.Use(npoints,lx);
  TVectorD y; y.Use(npoints,ly);
  TVectorD e; e.Use(npoints,ley);

  TMatrixD A(npoints,3);       // A matrix

  // Fill the A matrix
  //cannot do Log of vector
  double logx[npoints],log2x[npoints];
  for (int i=0; i < npoints; i++){
    logx[i]=log(x[i]);
    log2x[i]=log(x[i])*log(x[i]);
  }
  TVectorD vlogx; vlogx.Use(npoints,logx);
  TVectorD vlog2x; vlog2x.Use(npoints,log2x);
  TMatrixDColumn(A,0) = 1.0;
  TMatrixDColumn(A,1) = vlogx;//Log(x);
  TMatrixDColumn(A,2) = vlog2x;//Log(x)*Log(x);
  //cout << "A = ";
  //A.Print();

  // apply weights
  TMatrixD yw(A.GetNrows(),1);   // error-weighted data values

  for (Int_t irow = 0; irow < A.GetNrows(); irow++) {
    TMatrixDRow(A,irow) *= 1/e(irow);
    TMatrixDRow(yw,irow) += y(irow)/e(irow);
  }
  //cout << "A weighted = ";
  //A.Print();
  //cout << "y weighted = ";
  //yw.Print();

  TMatrixD theta=SolveLSQ(A, yw);
  //cout << "Param vector = ";
  //theta.Print();
  TF1 *fn1 = new TF1("fn1","[0]+[1]*log(x)+[2]*log(x)*log(x)",xmin,xmax);
//a+b*Log(x)+c*Log(x)*Log(x)
  
  

// View the resulting fit
  fn1->SetParameters(theta[0][0],theta[1][0],theta[2][0]);
  a->Fill(theta[0][0]);
  b->Fill(theta[1][0]);
  c->Fill(theta[2][0]);
  ba->Fill(theta[1][0],theta[0][0]);
  ca->Fill(theta[2][0],theta[0][0]);
  cb->Fill(theta[2][0],theta[1][0]);
  
  
    //
    //TH2F* ba= new TH2F("ba", "2D b vs a", nbin,0,5,nbin,0,5);
  //TH2F* ca= new TH2F("ba", "2D c vs a", nbin,0,5,nbin,0,5);
  //TH2F* cb= new TH2F("ba", "2D c vs b", nbin,0,5,nbin,0,5);
  //TH1F* rchisq= new TH1F("rchisq","Reduced chisq",nbin,0.0,50.0);
  //
  double sum=0;
  double aa=theta[0][0];
  double bb=theta[1][0];
  double cc=theta[2][0];
  for(int j=0;j<npoints;j++){
    sum=sum+(y[j]-(aa+bb*log(x[j])+cc*log(x[j])*log(x[j])))/e[j]*(y[j]-(aa+bb*log(x[j])+cc*log(x[j])*log(x[j])))/e[j];
  
  }
  chisq -> Fill(sum);
  rchisq -> Fill(sum/9);
  }
  

  //get 2x2 canvas 1-3 the distribution of a,b,c 4 calculate chi_sq
  auto tc2=new TCanvas("tc2","My Canvas1");
  tc2->Divide(2,2);
  tc2->cd(1);
  a->Draw();
  tc2->cd(2);
  b->Draw();
  tc2->cd(3);
  c->Draw();
  tc2->cd(4);
  chisq->Draw();


  //get 2x2 canvas 1 b vs a 2d hist cloz
            //2 c vs a // 3 c vs b // 4 distrivutionof reduced xhisq 
  auto tc3=new TCanvas("tc3","My Canvas2");
  tc3->Divide(2,2);
  tc3->cd(1);
  ba->Draw("colz");
  tc3->cd(2);
  ca->Draw("colz");
  tc3->cd(3);
  cb->Draw("colz");
  tc3->cd(4);
  rchisq->Draw();
  //my end
  
  cout << "Press ^c to exit" << endl;
  theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
  theApp.Run();
}
