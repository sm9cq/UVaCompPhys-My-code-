#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TRandom2.h"
#include "TROOT.h"


// fit1a.C
void fit1a(int entries=1000, bool save=false) {
   //Simple histogram fitting examples
  gROOT->Reset();  // useful to reset ROOT to a cleaner state

  TFile *tf=0;
  if (save) tf=new TFile("histo.root","recreate");

  TH1F *randomHist1 = new TH1F("randomHist1", "Random Histogram", 100, 0, 100);
  
  TRandom2 *generator=new TRandom2(0);  // parameter == seed, 0->use clock
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
  randomHist1->Draw("e");  // "e" shows bin errors
  }

  // Here we used a built in function, gaus, in the fit
  // This function will be associated with the histogram
  // and may be retrieved to get parameter information
  // Refer to http://root.cern.ch/root/html/TF1.html
  // for a complete list of TF1 methods

  //TF1 *fitfunc = randomHist1->GetFunction("gaus");
  TH1F *nchi2 = new TH1F("nchi2","Reduced Chi^2",100,0,2);
  TH1F *mean = new TH1F("mean","Mean",100,48,52);
  TH1F *prob = new TH1F("prob","Probability",100,0,1);
  TH2F *probvsnchi2 = new TH2F("probvsnchi2","Prob vs nChi2",100,0,2,100,0,2);
  auto tc=new TCanvas("tc","My Canvas");
  for (int i=0; i<1000; i++){
    randomHist1->Reset();   // reset histogram bin content to 0
    //nchi2->Reset();
    //mean->Reset();
    //prob->Reset();
    for (int i=0 ; i<entries ; i++){
      randomHist1->Fill(generator->Gaus(50,10)); // params: mean, sigma
      
      
    }
    gStyle->SetOptFit(1111); // show reduced chi2, probability, and params
    
    randomHist1->Fit("gaus","");  
    //randomHist1->Draw("e");  // "e" shows bin errors
    TF1 *fitfunc = randomHist1->GetFunction("gaus");
    nchi2->Fill((fitfunc->GetChisquare())/fitfunc->GetNDF());
    mean ->Fill(fitfunc->GetParameter(1));
    prob -> Fill(fitfunc->GetProb());
    probvsnchi2 -> Fill((fitfunc->GetChisquare())/fitfunc->GetNDF(),fitfunc->GetProb());
  }
  tc->Divide(2,2);
    tc->cd(1);
    nchi2->Draw();//"e");
    tc->cd(2);
    mean->Draw();//"e");
    tc->cd(3);
    prob->Draw();//"e");
    tc->cd(4);
    probvsnchi2->Draw("SCAT");
    //randomHist1->Draw();//"e");
  
    /*
  cout << "\nFit Params and errors" << endl;
  cout << fitfunc->GetParameter(0) << " +- " << fitfunc->GetParError(0) << endl;
  cout << fitfunc->GetParameter(1) << " +- " << fitfunc->GetParError(1) << endl;
  cout << fitfunc->GetParameter(2) << " +- " << fitfunc->GetParError(2) << endl;
  cout << "Fit Probability: " << fitfunc->GetProb() << endl;
    */
  if (save) {
    tf->Write();
    //  tf->Close();
  }
}
