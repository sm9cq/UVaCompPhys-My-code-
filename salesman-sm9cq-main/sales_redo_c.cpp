#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include<math.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

typedef struct {
  double lon, lat;
} COORD;

// fill the array of city locations
int GetData(char* fname, COORD *cities){
  FILE* fp=fopen(fname,"r");
  const int bufsiz=1000;
  char line[bufsiz+1];
  int ncity=0;
  while(1){
    fgets(line,bufsiz,fp);
    if (line[0]=='#') continue;  // skip comments
    if (feof(fp)) break;
    // we only scan for two numbers at start of each line
    sscanf(line,"%lf %lf",&cities[ncity].lon,&cities[ncity].lat);    
    ncity++;
  }
  fclose(fp);
  return ncity;
}

double dist(double lat1, double lat2, double long1, double long2){
  /*R = earth’s radius (mean radius = 6,371km)
    Δlat = lat2 − lat1
    Δlong = long2 − long1
    a = sin²(Δlat/2) + cos(lat1)·cos(lat2)·sin²(Δlong/2)
    c = 2·atan2(√a, √(1−a))
    d = R·c */
  double d=0;
  double latavg=(lat1+lat2)/2;
  double latdel=(lat2-lat1)/2;
  double longdel=(long2-long1)/2;
  double factor=M_PI/180;
  double slatdel=sin(latdel*factor);
  double slongdel=sin(longdel*factor);
  double clat1=cos(lat1*factor);
  double clat2=cos(lat2*factor);
  double a=slatdel*slatdel+clat1*clat2*slongdel*slongdel;
  double c=2*atan2(sqrt(a),sqrt(1-a));
  d=c*6371;
  //Zone constraints//
  double x1,x2,y1,y2;

  //...........long1.........//
  if (long1>= -85.5){
    x1=0;
  }  
  else if(long1> -102){
    x1=1;
  }
  else if(long1> -114){
    x1=2;
  }
  else { 
    x1=3;
  }
  //............long2...........//
  if (long2>= -85.5){
    x2=0;
  }  
  else if(long2> -102){
    x2=1;
  }
  else if(long2> -114){
    x2=2;
  }
  else { 
    x2=3;
  }
  //...........lat1...........//
  if(lat1 >= 39.8){
    y1 = 0;
  } else{
    y1 = 1;
  }
  //............lat2............//
  if(lat2 >= 39.8){
    y2 = 0;
  } else{
    y2 = 1;
  }

  d = d + 2000*abs(y1-y2) + 2000*abs(x1-x2);

  return d;
}

void swap(COORD *cities,int size){
  int swap1=rand()%size;
  int swap2=rand()%size;
  while (swap1==swap2){ swap2 = rand()%size; }
  double firstlat=cities[swap1].lat;
  double secondlat=cities[swap2].lat;
  double firstlong=cities[swap1].lon;
  double secondlong=cities[swap2].lon;
  
  cities[swap1].lat=secondlat;
  cities[swap2].lat=firstlat;
  cities[swap1].lon=secondlong;
  cities[swap2].lon=firstlong;

}

int main(int argc, char *argv[]){
  const int NMAX=2500;
  int flag=0;
  double distmin=0, T=1, intdist=0;
  COORD cities[NMAX];
  int ncity=GetData(argv[1],cities);

  COORD minarr[NMAX];

  double step = 200000000;
  //dat file an
  ofstream an2c;
  an2c.open("an2c.dat");
  for (int j=0;j<step;j++){
    T =3+ 7*(double)(j)/step;
    double distnew=0;
    for (int i=1; i<ncity;i++){
      distnew+=dist(cities[i-1].lat,cities[i].lat,cities[i-1].lon,cities[i].lon);
    
    }

    distnew+=dist(cities[ncity-1].lat,cities[0].lat,cities[ncity-1].lon,cities[0].lon);
    if (j==0){
      intdist=distnew;
      distmin=distnew; 
      cout<<"Initial Distance : "<<intdist<<"\n";

    }
    if(distnew<distmin){
      distmin=distnew;
      flag=1;
      for (int k=0;k<NMAX;k++){
        
        minarr[k].lat=cities[k].lat;
        minarr[k].lon=cities[k].lon;
      }
    } else {
      if((rand()%10000)/10000+.1 < exp(-T*(distnew-distmin)/1000) ) {
        distmin=distnew;
        flag=1;
        for (int k=0;k<NMAX;k++){
          
          minarr[k].lat=cities[k].lat;
          minarr[k].lon=cities[k].lon;
	}
      }

    }
    if(flag==1){
      for (int k=0;k<NMAX;k++){
        
        cities[k].lat=minarr[k].lat;
        cities[k].lon=minarr[k].lon;
      }
    }
    swap(cities,ncity);
    if(j%10000 == 0){
      cout << j << "\n";
      
      an2c<<setprecision(9)<<distmin<<"   "<<T<<endl;
    }
  }
  cout<<"New Distance : "<<distmin<<"\n";
  //close ofile
  an2c.close();
  //writing a dat file
  ofstream salescities2cmin;
  salescities2cmin.open("salescities2cmin.dat");
  for (int i=0; i<ncity; i++){
     salescities2cmin<<minarr[i].lon<<"\t"<<minarr[i].lat<<"\n";
     printf("%lf %lf\n",	minarr[i].lon,minarr[i].lat); 
  }
  salescities2cmin.close();
  return 0;
}
