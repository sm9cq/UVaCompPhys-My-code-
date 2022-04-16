#include <stdio.h>
// Using an input distribution of (xin,yin) values, use Lagrange interpolation to approximate y(x)
// x: absissa for interpolation
// xin,yin: arrays of the data points
// assuming xin is in ascending order
// npts: length of xin,yin
// order: interpolation order, number of points used in fit = order+1

double LgInterp(double x, double *xin, double *yin, int npts, int order=2){
  if ( x<xin[0] || x>xin[npts-1] ) {
    printf("interp: x is out of range: %lf\n",x);
    return -999.0;
  }
  if (order+1>npts) {
      printf("interp: not enough points for %d order calculation\n",order);
      return -999;
  }
  if (order<1) order=1;
  // find interval containing x
  int interval=0;
  while (x>xin[interval+1]) interval++;
  //  printf("%lf located at interval %d  ",x,interval);
  int range=(order+1)/2;  // number of points used above x
  int stop=interval+range;  // index of last point to use
  int start=stop-(order);   // index of first point to use

  // make sure indicies of all points used are valid
  int shift=0;
  if (stop>(npts-1)) shift = (npts-1) - stop;
  if (start<0) shift = -1*start;
  start += shift;
  stop += shift;
  //  printf("%d %d %d\n",start,stop,shift);
  // apply method
  double interp=0;
  for (int i=start; i<=stop; i++){
    double lambda=1;
    for (int j=start; j<=stop; j++){
      if (i==j)continue;
      lambda *= (x-xin[j])/(xin[i]-xin[j]);
    }
    interp += yin[i] * lambda;
  }
  return interp;
}
