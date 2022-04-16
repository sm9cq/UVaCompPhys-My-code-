// Generate ranrom Gaussian distributions and compare to PDF

void RandomGaus(){
  TCanvas *tc=new TCanvas();
  tc->Divide(1,2);
  TRandom2 tr;
  tc->cd(2)->SetLogy();
  gSystem->Unlink("RandomGaus.gif"); // delete old file

  
  const Int_t nsamples = 500;  
  gStyle->SetOptStat(0);
  
  const Int_t ntrials=100;
  TH1F *h=new TH1F("h","Samples from Normal Distribution",100,-5,5);
  h->Sumw2();
  h->SetMinimum(0.1);

  TF1 *fun=new TF1("fun","[2]*exp(-0.5*(x-[0])*(x-[0])/[1]/[1])",-5,5);
  fun->SetParameters(0,1,ntrials/sqrt(2*3.14159)/1.0/2);
  
  for (int nt=0;nt<ntrials;nt++){
    h->Reset();
    h->FillRandom("gaus",500);
    tc->cd(1);
    h->Draw();
    fun->Draw("same");
    tc->cd(2);
    h->Draw();
    fun->Draw("same");
    tc->Update();
    tc->Print("RandomGaus.gif+100");
    gSystem->Sleep(500);
  }
  tc->Print("RandomGaus.gif++500");
}

