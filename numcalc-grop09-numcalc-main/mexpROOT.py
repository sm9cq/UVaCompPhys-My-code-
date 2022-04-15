#!/usr/bin/env python 

# Here we will use the functions in our mexp.cpp file inside of python!
# **version using ROOT**

import ROOT as r  # this loads a HUGE amount of features (plots, math, ML, etc)
from math import exp
from numpy import isfinite
import sys
import argparse

parser = argparse.ArgumentParser(description='mexp program checker')
parser.add_argument('-p', '--doplot', default=None, action="store_true", help='Make a plot')
parser.add_argument('-x', '--xtest', type=float, default=0.1, help='Value of x for exp(-x) [0.1]')
parser.add_argument('-m', '--xmin', type=float, default=0.001, help='minimum x for scan [0.001]')
parser.add_argument('-M', '--xmax', type=float, default=100, help='maximum x for scan [100]')
args = parser.parse_args()

# Load the C++ code (this will create our .so and bindings)
r.gSystem.Load("mexp.so")
r.gInterpreter.Declare('#include "mexp.h"')  # define the interfaces
# note: ROOT will automatically define our interfaces to python from the
# information in our header file.  This is a very convenient and rather
# unique feature of ROOT.   If we have many functions in our header file
# ROOT will automatically define all the interfaces for PYTHON!

x=args.xtest
val=r.mexp(x)

print("exp(",x,")=",val,"rel_error=",abs(val-exp(-x))/exp(-x))

# plot results for a range of x values
if not args.doplot: sys.exit()


tg=r.TGraph()
tg.SetTitle("Relative error in exp(-x) calculation;x;Rel. error")
tg.SetLineWidth(2)
x=args.xmin
N=0
delta=(args.xmax-args.xmin)/100
while x<args.xmax:  # loop over x-range and test our mexp fcn
    val=r.mexp(x)
    err=abs(val-exp(-x))/exp(-x)
    if isfinite(err):
        tg.SetPoint(N,x,err)
        N=N+1
    else: print("x=",x,"error is unbounded")
    x=x+delta

#tg.Print()
tc=r.TCanvas()
tc.cd(1).SetLogy()
tg.Draw("ALP")
tc.Draw()

print("Hit return to exit")
sys.stdin.readline()




