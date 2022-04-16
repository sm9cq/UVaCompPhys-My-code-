# Here we will use the functions from our mexp.cpp file inside of python!
# **version using PyROOT**

import ROOT as R  # this loads a HUGE amount of features (plots, math, ML, etc)
from math import exp
from numpy import isfinite
import sys
import argparse

parser = argparse.ArgumentParser(description='mexp program checker')
parser.add_argument('-p', '--doplot', default=None, action="store_true", help='Make a plot')
parser.add_argument('-x', '--xtest', type=float, default=0.1, help='Value of x for exp(-x)')
parser.add_argument('-m', '--xmin', type=float, default=0.001, help='minimum x for scan')
parser.add_argument('-M', '--xmax', type=float, default=100, help='maximum x for scan')
args = parser.parse_args()

# Load the C++ code (this will create our .so and bindings)
R.gSystem.Load("mexp.so")
R.gInterpreter.Declare('#include "mexp.h"')  # define the interfaces
# note: ROOT will automatically define our interfaces to Python from the
# information in our header file.  This is a very convenient and rather
# unique feature of ROOT.   If we have many functions in our header file
# ROOT will automatically define all the interfaces for PYTHON!

x=args.xtest
mexp=R.MEXP()
val=mexp(x)

print("exp(",x,")=",val,"rel_error=",abs(val-exp(-x))/exp(-x))

# plot results for a range of x values
if not args.doplot: sys.exit()


tg=R.TGraph()
tg.SetTitle("Relative error in exp(-x) calculation;x;Rel. error")
tg.SetLineWidth(2)
x=args.xmin
N=0
delta=1.01
while x<args.xmax:  # loop over x-range and test out mexp fcn
    val=R.mexp(x)
    err=abs(val-exp(-x))/exp(-x)
    if isfinite(err):
        tg.SetPoint(N,x,err)
        N=N+1
    x=x*delta

#tg.Print()
tc=R.TCanvas()
tc.cd(1).SetLogy()
tg.Draw("ALP")
tc.Update()

print("Hit return to exit")
sys.stdin.readline()




