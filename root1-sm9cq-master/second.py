# second.py
# Here we are just writing a python module to import user functions

import ROOT as R

def randomHist(entries=1000):
    my_hist = R.TH1F("my_hist", "My First Histogram", 100, -5, 5)
    my_hist.FillRandom("gaus", entries)
    return my_hist


