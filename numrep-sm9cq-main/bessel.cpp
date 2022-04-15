//************************************************************************
// bessel.cpp: Calculate spherical Bessel functions via
// upward and downward recursion relations     
// Usage: bessel <order>
//************************************************************************

#include <cstdio>
#include <cmath>
#include <cstdlib>

double down(double x, int n);	        	// downward algorithm
double up(double x, int n);		 	// upward algorithm 

int main(int argc, char* argv[]){
  const double xmax=40.0;                     // max of x 
  const double xmin=0.25;                     // min of x >0  
  const double step=0.1;                      // delta x 

  int order=0;                                // order of Bessel function 
  if (argc>1) order = atoi(argv[1]);
  printf("Calculating J%d(x) for %f <= x < %f\n",order,xmin,xmax);

  FILE *out;
  out = fopen("bessel.dat","w");
  fprintf(out, "#Calculating J%d(x) for %f <= x < %f\n",order,xmin,xmax);
  fprintf(out,"# x\t\t J%d(x) [down]\t J%d(x) [up]\n",order,order);
  for(double x=xmin; x<=xmax+step/2; x+=step) {  
    fprintf(out, "%f\t%f\t%f\n", x, down(x,order), up(x,order));
  }
  printf("data stored in bessel.dat.\n");
  fclose(out);
  return 0;
}


// function using upward recursion
double up (double x, int l) {
  double j0=sin(x)/x;
  if (l==0) return j0;
  double j1=(j0-cos(x))/x;
  if (l==1) return j1;
  // else use recusion relation
  l--;
  return (2l+1)/x * up(x,l) - up(x,l-1);
}


// function using downward "recursion"
double down (double x, int n){
  const int lMax=50;
  double scale, j[lMax+2];     // maximum order for downward calculation   

  j[lMax+1] = j[lMax] = 1.;    // start with "something"
  for (int k=lMax; k>0 ; k--)
    j[k-1] = ((2.*(double)k + 1.)/x)*j[k] - j[k+1];   // recur. rel.
  scale = ((sin(x))/x)/j[0];		         // scale the result
  return(j[n]*scale);
}


