import ROOT as r
import sys

gDecay=r.TGraph("decay01.dat")
gDecay.SetTitle("Radioisotope decay plot;time [seconds];fraction of atoms remaining")
tfBin=r.TF1("tfbin","pow(0.99,x)",0,500)      # binomial model
tfBin.SetLineColor(r.kGreen)
tfBin.SetLineWidth(3)
tfPoisson=r.TF1("tfbin","exp(-0.01*x)",0,500) # Poisson model
tfPoisson.SetLineStyle(2)
tfPoisson.SetLineColor(r.kRed)

tc=r.TCanvas()
gDecay.Draw("AL*")    # see TGraph docs: plot (A)xes, connect points wil (Line), mark points with (*)
tfBin.Draw("same")
tfPoisson.Draw("same")
tl=r.TLegend(.65,.65,.9,.9)          # (xmin, ymin, xmax, ymax) for legend box
tl.AddEntry(gDecay,"MC model")       
tl.AddEntry(tfBin,"Binomial model")
tl.AddEntry(tfPoisson,"Poisson model")
tl.Draw()
tc.Draw()
tc.Print("decay.pdf")  # a canvas can be "Prined to many graphics formats"

print("Hit return to exit")
sys.stdin.readline()
