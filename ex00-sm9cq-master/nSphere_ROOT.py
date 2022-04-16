#!/usr/bin/env python

import ROOT as R
import sys


from math import pi, pow, gamma

def nsphereV(n, r=1): # volume of hypersphere
    if n<0: return 0
    return pow(pi,n/2.0) / gamma(n/2.0+1) * pow(r,n)

def ncubeV(n, L=2):   # volume of hypercube
    if n<0: return 0
    return pow(L,n)

# setup to scan 1.0 <= R <= 2.0 and 0<=n<=50 (nMax dimensions)
minR=1.0
maxR=2.0
nR=21
dR=(maxR-minR)/(nR-1)
nMax=50

graphs={}  # empty Python dictionary, similar to a map in C++ STL
for m in range(nR):
    r=minR+m*dR
    graphs[m]=R.TGraph()     # for each radius, store a graph of V(dimension)
    for n in range(nMax+1):
        graphs[m].SetPoint(n,n,nsphereV(n,r))


tc=R.TCanvas()
tc.SetLogy()
graphs[nR-1].SetTitle("Volume of Unit N-Sphere;n dimensions;V(n) [m^n]")
yMin=1e-3
graphs[nR-1].SetMinimum(yMin)
graphs[nR-1].Draw("alp")
t=R.TText()
t.SetTextAngle(-30)
t.SetTextSize(0.03);
for n in range(nR):
    graphs[n].Draw("lp")
    x=20+n
    y=graphs[n].Eval(x)
    if y>yMin: t.DrawText(x,y,"R="+str(n/20+1))
tc.Update()  

print("Hit return to exit")
sys.stdin.readline()

