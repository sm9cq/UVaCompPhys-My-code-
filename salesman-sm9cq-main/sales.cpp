#include<cstdio>
#include<cstdlib>
#include <iostream>
#include <fstream>
#include<math.h>
#include <stdlib.h>
#include <iomanip> 
using namespace std;

// simple structure to store city coordinates
// could also use std::pair<double> 
// or define a class

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

double dist(double long1, double lat1, double long2, double lat2)
{
  /*R = earth’s radius (mean radius = 6,371km)
    Δlat = lat2 − lat1
    Δlong = long2 − long1
    a = sin²(Δlat/2) + cos(lat1)·cos(lat2)·sin²(Δlong/2)
    c = 2·atan2(√a, √(1−a))
    d = R·c */
  double dlat=(lat2-lat1)*3.14159/180;
  double dlong=(long2-long1)*3.14159/180;
  double a=sin(dlat/2)*sin(dlat/2)+cos(lat1*3.14159/180)*cos(lat2*3.14159/180)*sin(dlong/2)*sin(dlong/2);
  double c = 2 * atan2(sqrt(a), sqrt(1-a));
  double d = 6371 * c;
  
  /*Zone*/
  double x1,x2,y1,y2;

  if(long1>=-85.5){x1=0;}
  else if(long1>-102){x1=1;}
  else if(long1> -114){x1=2;}
  else {x1=3;}

  if(long2>=-85.5){x2=0;}
  else if(long2>-102){x2=1;}
  else if(long2> -114){x2=2;}
  else {x2=3;}

  if(lat1>= 39.8){y1=0;}
  else {y1=1;}

  if(lat2>= 39.8){y2=0;}
  else {y2=1;}

  d=d+2000*abs(y1-y2) + 2000*abs(x1-x2);
  return d;
  
}



int main(int argc, char *argv[]){
  const int NMAX=2500;
  COORD cities[NMAX];
  int ncity=GetData(argv[1],cities);
  double arr_city[ncity][2];/*Storing*/
  printf("Read %d cities from data file\n",ncity);
  printf("Longitude  Latitude\n");
  for (int i=0; i<ncity; i++){
    //printf("%lf %lf\n",	cities[i].lon,cities[i].lat);
    arr_city[i][0]=cities[i].lon;
    arr_city[i][1]=cities[i].lat;}

  /*Print for checking the array*/
  for (int i=0; i<ncity; i++){
    //cout<<"longtitude:"<<arr_city[i][0]<<" & "<<"latitude:"<<arr_city[i][1]<<endl;
    
  }
  cout<<"number of cities:"<<ncity<<endl;
  //cout<<cities[1].lon<<endl;
  //cout<<"Distance: "<<dist(arr_city[1][0],arr_city[1][1],arr_city[2][0],arr_city[2][1])<<"km"<< endl;

  /*Get original all distances*/
  double Tmax=50;//1e20;
  double dT=Tmax/1e7;//1e-7*Tmax;


  double AllDist=0;
  for (int i=0; i<ncity; i++){
    AllDist+=dist(arr_city[i][0],arr_city[i][1],arr_city[i+1][0],arr_city[i+1][1]);
    //cout<<"Distance: "<< AllDist <<"km"<<" at city "<< i+1 << endl;
  }

  cout<<"All Distance: "<< AllDist <<"km"<< endl;
  double newDist=AllDist;
  /*Try to kept value in dat file*/
  ofstream output;
  output.open("salescities2cdistvsT.dat");
  while(Tmax>0.000001){
  /*Swaping*/
  //srand (time(NULL));/* initialize random seed: */

  int swap1=rand()%ncity; //swap1 in range 0 to 23
  int swap2=rand()%ncity;
  //int swap3=rand()%ncity; //swap1 in range 0 to 23
  //int swap4=rand()%ncity;
  while((swap2==swap1)){//||(swap3==swap4)||(swap2==swap3)||(swap1==swap4)){ 
    swap2=rand()%ncity;
    /*swap3=rand()%ncity;
    swap1=rand()%ncity;
    swap4=rand()%ncity;*/
  }
  
  //cout << swap1 << " and " << swap2 <<endl;
  /*cout <<"city swap1:"<< arr_city[swap1][0] << " and " << arr_city[swap1][1] <<endl;
  cout <<"city swap2:"<< arr_city[swap2][0] << " and " << arr_city[swap2][1] <<endl;
  */

  double temp[1][2];
  temp[0][0]=arr_city[swap1][0];
  temp[0][1]=arr_city[swap1][1];
  /*
  temp[1][0]=arr_city[swap2][0];
  temp[1][1]=arr_city[swap2][1];

  temp[2][0]=arr_city[swap3][0];
  temp[2][1]=arr_city[swap3][1];
  */
  arr_city[swap1][0]=arr_city[swap2][0];
  arr_city[swap1][1]=arr_city[swap2][1];
  
  arr_city[swap2][0]=temp[0][0];
  arr_city[swap2][1]=temp[0][1];
  /*
  arr_city[swap3][0]=arr_city[swap4][0];
  arr_city[swap3][1]=arr_city[swap4][1];
  
  arr_city[swap4][0]=temp[2][0];
  arr_city[swap4][1]=temp[2][1];
  */
  /*
  cout<<"after swapping"<<endl;
  cout <<"city swap1:"<< arr_city[swap1][0] << " and " << arr_city[swap1][1] <<endl;
  cout <<"city swap2:"<< arr_city[swap2][0] << " and " << arr_city[swap2][1] <<endl;
  */

  /*Get all distances after swapping*/
  double AllDist2=0;
  for (int i=0; i<ncity; i++){
    AllDist2+=dist(arr_city[i][0],arr_city[i][1],arr_city[i+1][0],arr_city[i+1][1]);
    //cout<<"Distance: "<< AllDist <<"km"<<" at city "<< i+1 << endl;
  }

  //cout<<"All Distance after swapping: "<< AllDist2 <<"km"<< endl;
  if(AllDist2<newDist){newDist=AllDist2;}
  if((AllDist2>newDist) && exp(-abs(AllDist2-newDist)/Tmax)> ((double) rand() / (RAND_MAX))) {newDist=AllDist2;} 

  Tmax-=dT;//1e-3*Tmax;//0.01*Tmax;//dT;
  cout<<"All Distance after accepting: "<< newDist <<"km"<< " at T="<<Tmax+dT<<endl;
  

  output<<setprecision(9)<<newDist<<"   "<<Tmax+dT<<endl;

  }
  cout<<"Original Distance="<<AllDist<<endl;
  cout<<"All Distance after accepting: "<< newDist <<"km"<< " at T="<<setprecision(9)<<Tmax+dT<<endl;

  //close ofile
  
  output.close();
  /*Try to kept value in dat file*/
  ofstream output2;
  output2.open("salescities2c.dat");
  for (int i=1; i<ncity; i++){   
    
    output2<<arr_city[i][0]<<"   "<<arr_city[i][1]<<endl;
//tf->Close();
}





  return 0;
}
