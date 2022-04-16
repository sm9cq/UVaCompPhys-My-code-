// Generate random data sets based on a model f(x) = mx+b
// apply a linear fit, display the chi^2 per experiment

void RandomMeasuresAndFitP1(){
  TCanvas *tc=new TCanvas();
  tc->Divide(1,2);
  TRandom2 tr;
  gSystem->Unlink("RandomMeasuresAndFitP1.gif"); // delete old file
  
  const Int_t n = 10;  // plot to points
  Double_t x[n];
  Double_t y[n];
  Double_t ex[n];
  Double_t ey[n];
  const double sigma=1.0;
  gStyle->SetOptStat(0);
  
  const Int_t ntrials=300;
  const double m=1.5;
  const double b=0.75;
  
  for (int i=0;i<n;i++) {
    x[i]=i;
    ex[i]=0;
    ey[i]=sigma;
  }
  TGraphErrors *tg=new TGraphErrors(n,x,y,ex,ey);
  tg->SetLineWidth(2);
  tg->SetMarkerColor(4);
  tg->SetMarkerStyle(21);
  TH2F *h=new TH2F("h","Pseudoexperiments",5,-0.5,n+0.5,5,b-4*sigma,20);
  TH1F *hchi2m=new TH1F("hchi2","Chi^2 distribution",20,n-3*sqrt(2*n),n+3*sqrt(2*n));
  hchi2m->SetLineStyle(2);
  TH1F *hchi2f=new TH1F("hchi2","Chi^2 distribution",20,n-3*sqrt(2*n),n+3*sqrt(2*n));
  TF1 *fun=new TF1("f","[0]*x+[1]",0,n);
  fun->SetLineStyle(2);
  fun->SetParameters(m,b);
  auto labelm=new TPaveLabel(-0.5,17,4.0,19,"");
  auto labelf=new TPaveLabel(-0.5,15,4.0,17,"");
  auto tl=new TLegend(0.1,0.6,0.3,0.75);
  tl->AddEntry(hchi2m,"#chi^{2} wrt truth","l");
  tl->AddEntry(hchi2f,"#chi^{2} wrt fit","l");

  
  double chi2model;  // chi2 based on the the true model/PDF
  for (int nt=0;nt<ntrials;nt++){
    chi2model=0;
    for (int i=0;i<n;i++) {
      double ynom=m*x[i]+b;
      double yobs=ynom+tr.Gaus(0,sigma);
      tg->SetPoint(i,x[i],yobs);
      chi2model+=(ynom-yobs)*(ynom-yobs)/sigma/sigma;
    }
    tc->cd(1);
    h->Draw();
    auto res= tg->Fit("pol1","S0Q");
    tg->Draw("same,P");
    tg->GetFunction("pol1")->Draw("same");
    fun->Draw("same");
    double chi2fit=res->Chi2();  // chi^2 from the 2-parameter fit
    labelm->SetLabel(TString::Format("Chi^2 wrt model (dashed) = %5.2lf",chi2model));
    labelm->Draw();
    labelf->SetLabel(TString::Format("Chi^2 wrt fit (solid)   = %5.2lf",chi2fit));
    labelf->Draw();
    tg->Draw("P");
    tc->cd(2);
    hchi2m->Fill(chi2model);
    hchi2f->Fill(chi2fit);
    hchi2m->Draw();
    hchi2f->Draw("same");
    tl->Draw();
    tc->Update();
    if (nt<50) tc->Print("RandomMeasuresAndFitP1.gif+100");  // 100ms between frames
    gSystem->Sleep(250);  // pause between trials 500ms
  }
  tc->cd(1);
  labelm->SetLabel(TString::Format("<Chi^2> model = %f",hchi2m->GetMean()));
  labelm->Draw();
  labelf->SetLabel(TString::Format("<Chi^2> fit   = %f",hchi2f->GetMean()));
  labelf->Draw();

  tc->Print("RandomMeasuresAndFitP1.gif+1000");  // 1000ms delay before repeat
}

