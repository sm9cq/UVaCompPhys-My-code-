// first.C
// This is an unnamed macro.  It essentially functions as a script
// to specify a series of ROOT commands.
// Usage in ROOT: .X first.C
{
  delete my_hist;
  TH1F *my_hist = new TH1F("my_hist", "TMath::Gaus(x,200,15)", 100, -5, 5);
  //writing here
  //1st canvas
  TH1D h("h", "Gaus(x,200,15)", 100, 150, 250);//10000 bin , xmin, xmax
  TRandom3 rndgen;
  for (int i = 0; i < 100; ++i) h.Fill(rndgen.Gaus(200,15));
  h.Draw();

  //2nd canvas
  TH1D h2("h", "Uniform", 100, 150, 250);//10000 bin , xmin, xmax
  //TRandom3 rndgen;
  for (int i = 0; i < 100; ++i) h2.Fill(rndgen.Uniform(150,250));
  h2.Draw();

  //3rd canvas
  TH1D h3("h", "Gaus(x,200,15)+Uniform", 100, 150, 250);//10000 bin , xmin, xmax
  //TRandom3 rndgen;
  for (int i = 0; i < 100; ++i) h3.Fill(rndgen.Gaus(200,15));
  for (int i = 0; i < 100; ++i) h3.Fill(rndgen.Uniform(150,250));
  h3.Draw();

  //4th canvas
  
  TH1D h4("h", "exp(-5)*exp(-x)", 100, 0, 10);// bin , xmin, xmax
  TF1 fun2("fun2", "exp(-5)*exp(-x)", 0, 10);
  //fun2.Eval(5);
  h4.FillRandom("fun2", 100);
  
  //form1 = TFormula( "form1", "exp(-x)" );
  //form1.Eval( 5 );
  //fun2.Eval(5);
  h4.Fit("expo");
  h4.Draw();

  //5th canvas

  TH2D h5("histo", "2D normal distribution", 100,-50,50,100,-50,50);
  for(double i = 0; i < 200000; i++) h5.Fill(rndgen.Gaus(0,10), rndgen.Gaus(1,20));
  h5.Draw("lego1");

  //6th canvas
  TGraph* gr1 = new TGraph("Temp.txt","%lg %lg %*lg %*lg");
  TGraph* gr2 = new TGraph("Temp.txt","%lg %*lg %lg %*lg");
  TGraph* gr3 = new TGraph("Tempmin.txt","%lg %lg");
  gr1->Draw("apl");
  gr1->SetMarkerColor(4);
  gr1->SetMarkerStyle(21);
  gr2->Draw("pl same");
  gr2->SetMarkerColor(5);
  gr2->SetMarkerStyle(20);
  gr3->Draw("pl same");
  gr3->SetMarkerColor(6);
  gr3->SetMarkerStyle(22);
  gr1->GetXaxis()->SetRangeUser(0, 35);
  gr1->GetYaxis()->SetRangeUser(45, 120);

  leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->SetHeader("The Legend Title");
  leg->AddEntry("gr1","Maxs");
  leg->AddEntry("gr2","Avgs");
  leg->AddEntry("gr3","Mins");
  leg->Draw();
  
  //TRandom3 rndgen;
  //for (int i = 0; i < 50000; ++i) h3.Fill(rndgen.Gaus(200,15));
  //for (int i = 0; i < 50000; ++i) h3.Fill(rndgen.Uniform(150,250));
  //h3.Draw();

  //
  //my_hist->FillRandom("my_hist", 10000);
  //my_hist->Draw();  // the creates a default TCanvas and draws the histogram
  
  // alternatively, we can make our TCanvas and subdivide it if we want
  auto tc=new TCanvas("tc","My Canvas");  // return pointer to a new TCanvas
  tc->Divide(3,2);   // columns, rows  3x2 TPads in this case
  tc->cd(1);  // TPads are numbered going left to right, top to bottom
  h.Draw();
  //my_hist->Draw();
  
  tc->cd(2);
  h2.Draw();
  //my_hist->Draw("E");   // draw with error bars
  tc->cd(3);
  h3.Draw();
  //my_hist->Draw("E");   // draw with error bars
  //my_hist->Draw("hist,same");   // overlay with lines through the points
  // the command  my_hist->Draw("hist,E") is equivalent to the two above
  tc->cd(4);
  h4.Draw();
  //my_hist->Draw("hbar");     // draw as horizontal bar chart
  tc->cd(5);
  h5.Draw("lego1");
  //my_hist->Draw("hbar");     // draw as horizontal bar chart
  tc->cd(6);
  gr1->Draw("apl");
  gr2->Draw("pl same");
  gr3->Draw("pl same");
  leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->SetHeader("The Legend Title");
  leg->AddEntry("gr1","Maxs");
  leg->AddEntry("gr2","Avgs");
  leg->AddEntry("gr3","Mins");
  leg->Draw();
  //my_hist->Draw("hbar");     // draw as horizontal bar chart
  tc->Update()
}
