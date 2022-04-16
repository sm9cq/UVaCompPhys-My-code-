from math import *
import numpy as np
import matplotlib.pyplot as plt
import array

n1=np.linspace(0,50,51)
rad1=np.linspace(1,2,21)
# V vs n
plt.figure()
for rad in rad1:
    Vol1=[]
    for n in n1:
        V = pi**(n/2)*rad**(n)/gamma(n/2+1);
        Vol1.append(V)
    #print(len(n1))
    #print(len(Vol))
    plt.plot(n1,Vol1,label='rad='+str(rad))

plt.xlabel('n')
plt.ylabel('Vol')
plt.title('Vol n-sphere')
#plt.legend()
plt.legend(loc='best')
plt.savefig('images/plot1.png', format='png')
plt.show()

# V vs R
plt.figure()
for n in n1:
    Vol2=[]
    for rad in rad1:
        V = pi**(n/2)*rad**(n)/gamma(n/2+1);
        Vol2.append(V)
    #print(len(n1))
    #print(len(Vol))
    plt.plot(rad1,Vol2,label='rad='+str(n))


plt.xlabel('rad')
plt.ylabel('Vol')
plt.title('Vol n-sphere')
#plt.legend()
plt.legend(loc='best')
plt.savefig('images/plot2.png', format='png')
plt.show()


#fig, axs = plt.subplots(2)
#fig.suptitle('Vertically stacked subplots')
#axs[0].plot(n1,Vol1)
#axs[1].plot(rad1,Vol2)

       #print("n=",n,"rad=",rad,V)
