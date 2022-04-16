#include <cstdio>
#include <cmath>

// Volume of nSphere
double nsphereV(double n, double R=1.0){
    if (n<0) return 0;
    return pow(M_PI,n/2.0) / tgamma(n/2.0+1) * pow(R,n);
}

int main(){
  const int nDim=25;
  const double r0=1.0;
  const double dr=0.05;
  const int nR=21;

  printf("#d V(r1) V(r2) ... r1=%lf, rmax=%lf\n",r0,r0+dr*(nR-1));
  for (int n=0; n<=nDim; ++n){
    printf("%3d ",n);
    for (int m=0; m<nR; ++m){
      double r=r0+m*dr;
      printf("%5.3le " ,nsphereV(n,r));
    }
    printf("\n");
  }
  return 0;
}
