// Generate random data sets based on a model f(x) = mx+b

void RandomMeasuresP1(){
  TCanvas *tc=new TCanvas();
  TRandom2 tr;
  gSystem->Unlink("RandomMeasuresP1.gif"); // delete old file
  
  const Int_t n = 10;  // plot to points
  Double_t x[n];
  Double_t y[n];
  Double_t ex[n];
  Double_t ey[n];
  const double sigma=1.0;
  gStyle->SetOptStat(0);
  
  const Int_t ntrials=50;
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
  TF1 *fun=new TF1("f","[0]*x+[1]",0,n);
  fun->SetParameters(m,b);
  auto label=new TPaveLabel(-0.5,15,3.5,19,"");
  for (int nt=0;nt<ntrials;nt++){
    double chi2=0;
    for (int i=0;i<n;i++) {
      double ynom=m*x[i]+b;
      double yobs=ynom+tr.Gaus(0,sigma);
      tg->SetPoint(i,x[i],yobs);
      chi2+=(ynom-yobs)*(ynom-yobs)/sigma/sigma;
    }
    auto res= tg->Fit("pol1","S0Q");
    //cout << res->Chi2() << endl;
    //chi2=res->Chi2()
    h->Draw();
    fun->Draw("same");
    label->SetLabel(TString::Format("Chi^2 = %f",chi2));
    label->Draw();
    tg->Draw("P");
    tc->Update();
    if (nt<50) tc->Print("RandomMeasuresP1.gif+100");  // 100ms between frames
    gSystem->Sleep(500);
  }
  tc->Print("RandomMeasuresP1.gif+500");  // 500ms delay before repeat
}

