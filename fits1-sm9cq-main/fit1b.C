#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TRandom2.h"
#include "TROOT.h"
#include "TTree.h"
#include "TMath.h"


// fit1b.C
void fit1b(int entries=1000, bool save=false) {
   //Simple histogram fitting examples
  gROOT->Reset();  // useful to reset ROOT to a cleaner state

  TFile *tf=0;
  if (save) tf=new TFile("histo.root","recreate");

  

  
  /*TF1 *fh1 = h1->GetFunction("gaus");
  double par0 = fh1 ->GetParameter(0);
  double par1 = fh1 ->GetParameter(1);
  double par2 = fh1 ->GetParameter(2);*/
  TH1F *nchi2 = new TH1F("nchi2","Reduced Chi^2",100,0,2);
  TH1F *mean = new TH1F("mean","Mean",80,48,52);
  TH1F *sigma = new TH1F("sigma","Sigma",80,8,12);
  TH1F *prob = new TH1F("prob","Probability",100,0,1);
  TH2F *probvsnchi2 = new TH2F("probvsnchi2","Prob vs nChi2",100,0,2,100,0,1);
  //int nbin = h1->GetNbinsX();
  TH1F *randomHist1 = new TH1F("randomHist1", "Random Histogram", 100, 0, 100);
  TH1F *randomHist2= new TH1F("randomHist2", "Random Histogram 2",100,0,100);
  TH1F *meanLL = new TH1F("meanLL","MeanLL",80,48,52);
  TH1F *sigmaLL = new TH1F("sigmaLL","SigmaLL",80,8,12);
  TRandom2 *generator=new TRandom2(0);  // parameter == seed, 0->use clock
  auto tc=new TCanvas("tc","My Canvas");
//int ntrials=1000;

  for (int i=0; i<1000; i++){
    randomHist1->Reset();   // reset histogram bin content to 0

    for (int i=0 ; i<entries ; i++){
      randomHist1->Fill(generator->Gaus(50,10)); // params: mean, sigma
      // nchi2->Fill(fit
  }
  // simple fits may be performed automatically
  // gStyle->SetOptFit(111);  // show reduced chi2 and params

  gStyle->SetOptFit(1111); // show reduced chi2, probability, and params
  randomHist1->Fit("gaus","");
  randomHist2->Fit("gaus","L");
  randomHist1->Draw("e");  // "e" shows bin errors
  

  // Here we used a built in function, gaus, in the fit
  // This function will be associated with the histogram
  // and may be retrieved to get parameter information
  // Refer to http://root.cern.ch/root/html/TF1.html
  // for a complete list of TF1 methods
  
  TF1 *fitfunc = randomHist1->GetFunction("gaus");
  
  //TH1F *NLL = new TH1F("NLL","NLL",400,30,70);
  
  
    
    
    nchi2->Fill((fitfunc->GetChisquare())/fitfunc->GetNDF());
    mean ->Fill(fitfunc->GetParameter(1));
    prob -> Fill(fitfunc->GetProb());
    sigma -> Fill(fitfunc->GetParameter(2));
    probvsnchi2 -> Fill((fitfunc->GetChisquare())/fitfunc->GetNDF(),fitfunc->GetProb());

    
    cout << "\nFit Params and errors" << endl;
    cout << fitfunc->GetParameter(0) << " +- " << fitfunc->GetParError(0) << endl;
    cout << fitfunc->GetParameter(1) << " +- " << fitfunc->GetParError(1) << endl;
    cout << fitfunc->GetParameter(2) << " +- " << fitfunc->GetParError(2) << endl;
    cout << "Fit Probability: " << fitfunc->GetProb() << endl;
    // NLL-> Fill(LL);
    TF1 *fitfuncLL=randomHist2->GetFunction("gaus");
    
    meanLL->Fill(fitfuncLL->GetParameter(1));
    sigmaLL->Fill(fitfuncLL->GetParameter(2));
  }
    tc->Divide(2,3);
    tc->cd(1);
    mean->Draw();//nchi2->Draw();//"e");
    tc->cd(2);
    sigma->Draw();//mean->Draw();//"e");
    tc->cd(3);
    meanLL->Draw();//prob->Draw();//"e");
    tc->cd(4);
    sigmaLL->Draw();
    tc->cd(5);
    probvsnchi2->Draw("SCAT");
    tc->cd(6);
    //sigma->Draw();
    //randomHist1->Draw();//"e");
    //tc->cd(6);
    //NLL->Draw();
    
    TCanvas *c2 = new TCanvas("c2","c2",600,600);
    //some code
    TFile *f=new TFile("histo25.root");
    TH1F *h1=(TH1F*)f->Get("randomHist1");

    gStyle->SetOptFit(1111);
    h1->Fit("gaus","L");
    h1->Draw("e");

    TF1 *fh=h1->GetFunction("gaus");
    
    double par0 = fh->GetParameter(0);
    double par1 = fh->GetParameter(1);
    double par2 = fh->GetParameter(2);

    c2->Draw();
    TH1F *NLL=new TH1F("NLL","NLL",100,0,100);
    TH1F *probLL=new TH1F("probLL","Probability",100,0,1);

    TRandom2 *r = new TRandom2();

 

    for (int j=0;j<1000;j++){
      int nbins=h1->GetNbinsX();
      double LL = 0.0;
      for (int i=1;i<=nbins;i++){
      double expected = par0*exp(-0.5*pow((((h1->GetBinCenter(i))-par1)/par2),2));
      int n0bs = r->Poisson(expected);
      double prob = TMath::PoissonI(n0bs,expected);
      LL+=log(prob);
     
      }
    //tf1h->Fit("NLL");
    //cout<<"LL : "<<LogLikelihood;
      NLL->Fill(-LL);
    }
    TCanvas *c3=new TCanvas("C3","C3",600,600);
 
    gStyle->SetOptFit(1111);
    NLL->Fit("gaus");
    gMinuit->Command("SCAn 1");
    TGraph *gr = (TGraph*)gMinuit->GetPlot();
    gr->SetMarkerStyle(20);
    gr->SetTitle("Min NLL");
    gr->Draw("alp");
    //NLL->Draw();
    c3->Draw();

    TCanvas *c4=new TCanvas("C4","C4",600,600);
    NLL->Draw();

    TF1 *fNLL=NLL->GetFunction("gaus");
    double p0NLL=fNLL->GetParameter(0);
    double meanNLL=fNLL->GetParameter(1);
    double stdNLL=fNLL->GetParameter(2);
    double NLLmin=NLL->GetMinimum();
 
    cout<<"min : "<<NLLmin;
    
    
  if (save) {
    tf->Write();
    //  tf->Close();
  }
}
