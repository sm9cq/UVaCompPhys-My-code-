#!/usr/bin/env python

import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import sys

from math import pi, pow, gamma

def nsphereV(n, r=1): # volume of hypersphere
    if n<0: return 0
    return pow(pi,n/2.0) / gamma(n/2.0+1) * pow(r,n)

def ncubeV(n, L=2):   # volume of hypercube
    if n<0: return 0
    return pow(L,n)

import matplotlib
import numpy as np
import matplotlib.pyplot as plt

# setup to scan 1.0 <= R <= 2.0 and 0<=n<=50 (nMax dimensions)
nMax=50

x = np.arange(0, nMax+1)
y = np.zeros(nMax+1)
radii = np.arange(1,2.05,0.05)
for r in radii:
    for n in x:
        y[n] = nsphereV(n,r)
    plt.plot(x,y)
plt.yscale('log')
plt.xlabel('n dimensions')
plt.ylabel('V(dimension)')
plt.title('Volume of n-sphere')
plt.text(55, 10, 'Increasing R ==>',
         rotation=90,
         horizontalalignment='center',
         verticalalignment='top')
plt.text(35,2e-10,'R=1.0')
plt.text(45,5e3,'R=2.0')
print("control-C to exit")

plt.show()
