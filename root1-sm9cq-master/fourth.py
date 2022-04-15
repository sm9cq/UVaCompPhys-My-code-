# forth.py
# Here we specify a named function for ROOT.
# The function is effectively added to the ROOT framework

import ROOT as R
import sys

# ".L" loads the code into ROOT's shared memory
# Adding the "+" sign causes ROOT to run the c++ compiler to
# generate a shared library that is then linked with ROOT
R.gROOT.ProcessLine(".L fourth.C+")
R.randomHist4(9999)



