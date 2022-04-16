# first.py
# pythonic version

import ROOT as R
import sys

my_hist = R.TH1F("my_hist", "My First Histogram", 100, -5, 5);
my_hist.FillRandom("gaus", 10000);
my_hist.Draw();


#alternatively, we can make our TCanvas and subdivide it if we want
tc=R.TCanvas("tc","My Canvas")
tc.Divide(2,2)   # columns, rows  2x3 TPads in this case
tc.cd(1)         # TPads are numbered going left to right, top to bottom
my_hist.Draw()
tc.cd(2)
my_hist.Draw("E")           # draw with error bars
tc.cd(3)
my_hist.Draw("E")           # draw with error bars
my_hist.Draw("L,same")      # overlay with lines through the points
tc.cd(4)
my_hist.Draw("HBAR")        # draw as horizontal bar chart
tc.Update()
tc.Print("first.pdf")       # save our canvas as a PDF

# let's save our pretty canvas
tf=R.TFile("first.root","recreate")
tc.Write()

print("Hit return to exit")
sys.stdin.readline()

tf.Close()
