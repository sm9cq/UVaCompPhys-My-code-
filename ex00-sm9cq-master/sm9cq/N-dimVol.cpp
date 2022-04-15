//Calculate the N-dim sphere 
//V=pi^(n/2)*R^n/(Gamma(n/2+1))
//Vn=pi^(n/2)/(n/2)!
#include<stdio.h>
#include<iostream>
#include<math.h>
using namespace std;

int main() {
	double rad[21],n[51],V[21][51];  // Double precision variables

	for (int i=0; i<51; i++)
		{
		n[i]=i;
		//cout << n[i] << endl;
		}
	for (int j=0;j<21;j++)
		{
		rad[j]=1+j*0.05;
		//cout << rad[j] << endl;
		}
	FILE *fp=fopen("N-ball.dat","w");	
	for (int k=0 ; k<51; k++)
	{
		for(int l=0;l<21;l++)
		{
		V[l][k]= pow(M_PI,n[k]/2)*pow(rad[l],n[k])/tgamma((n[k]/2)+1);
		fprintf(fp,"%8.4f %8.4f %8.4f\n",n[k],rad[l],V[l][k]);
		//cout<< V[l][k]<<endl;
		}
	fprintf(fp,"\n");	
	
	}
//V= pow(M_PI,n/2)*pow(rad,n)/tgamma(n/2+1);

// Calculate Volume
//
// printf("%s :: dimension n =%lf,  radius r= %lf, Volume V = %lf\n", __FILE__,n, rad, V);  // Print results
	fclose(fp);
	return 0;
 } // End main
