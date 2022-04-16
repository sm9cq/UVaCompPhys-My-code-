// simple example of using ROOT libraries in a C++ program with graphics
// and use of TASImage class

#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TASImage.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TColor.h"

#include "assert.h"

#include <iostream>
#include <stdio.h>
#include <cmath>
using namespace std;

typedef struct{
  int red,green,blue;
} COLOR;

double color_dist(unsigned r1,unsigned g1,unsigned b1,unsigned r2,unsigned g2,unsigned b2){
  double dist = sqrt(pow((r1-r2),2)+pow((g1-g2),2)+pow((b1-b2),2));
  return dist;
}

double swap(COLOR *first,COLOR *second,int size){
  int swap1 = rand()%size;
  int swap2 = rand()%size;
  double dist_change = 0;
  while (swap1 == swap2){ swap2 = rand()%size; }
  double swap1R = first[swap1].red;
  double swap1G = first[swap1].green;
  double swap1B = first[swap1].blue;
  double swap2R = first[swap2].red;
  double swap2G = first[swap2].green;
  double swap2B = first[swap2].blue;
  
  double old_dist = color_dist(swap1R, swap1G, swap1B, second[swap1].red, second[swap1].green, second[swap1].blue ) + color_dist(swap2R, swap2G, swap2B, second[swap2].red, second[swap2].green, second[swap2].blue);

  first[swap1].red = swap2R;
  first[swap1].green = swap2G;
  first[swap1].blue = swap2B;
  first[swap2].red = swap1R;
  first[swap2].green = swap1G;
  first[swap2].blue = swap1B;
  
  dist_change = -(old_dist - color_dist(swap2R, swap2G, swap2B, second[swap1].red, second[swap1].green, second[swap1].blue)-color_dist(swap1R, swap1G, swap1B, second[swap2].red, second[swap2].green, second[swap2].blue));

  return dist_change;
} 

double color_diff(UInt_t*firstPix,UInt_t*secondPix, COLOR *first,COLOR *second, COLOR *minpic, int numPix, int j, int flag){
  double distance=0; 
  for (int i=0;i< numPix; i++){

    if (j == 0) {
      first[i].blue = unsigned((((1<<8)-1)&(firstPix[i]<<0))>>0);
      first[i].green = unsigned((((1<<16)-1)&(firstPix[i]<<0))>>8);
      first[i].red = unsigned((((1<<24)-1)&(firstPix[i]<<0))>>16);

      second[i].blue = unsigned((((1<<8)-1)&(secondPix[i]<<0))>>0);
      second[i].green = unsigned((((1<<16)-1)&(secondPix[i]<<0))>>8);
      second[i].red = unsigned((((1<<24)-1)&(secondPix[i]<<0))>>16);
    
      distance += color_dist(first[i].red,first[i].green,first[i].blue,second[i].red,second[i].green,second[i].blue);
    }
    else{
      distance += color_dist(first[i].red,first[i].green,first[i].blue,second[i].red,second[i].green,second[i].blue);

    }
  }
  
  return distance;
}



  
int main(int argc, char **argv){

  if (argc<3) {
    cout << "Usage: simapix_start image1 image2 <output=out.png>" << endl;
    return 0; 
  }
  
  TString fsrc=argv[1];
  TString ftgt=argv[2];
  TString fout;
  argc>3 ? fout = argv[3] : fout="out.png";
  cout << "Reading images: source= " << fsrc << " target= " << ftgt << endl;
  cout << "Output= " << fout << endl;

  TApplication theApp("App", &argc, argv);

  // create image objects
  TASImage *src = new TASImage(fsrc.Data());
  TASImage *tgt = new TASImage(ftgt.Data());
  TASImage *out = new TASImage(*src); // start with copy of source

  // Test image geometry, exit if they are not the same dimensions
  assert ( src->GetWidth() == tgt->GetWidth() && src->GetHeight() == tgt->GetHeight() );
  cout << "Pixel Geometry: " << src->GetWidth() << " x " << src->GetHeight() << endl;
  Long_t numPix=src->GetWidth()*src->GetHeight();
  
  COLOR first[numPix];//sourcepic
  COLOR second[numPix];//targetpic
  COLOR oldspic[numPix];
  COLOR minpic[numPix];
  // *** The work happens here
  // access the pixels for the output image 
  // each pixel is a 32-bit word, 1 byte each for (alpha,red,green,blue)
  // don't touch alpha (bits 31:28)
  UInt_t *outPix = out->GetArgbArray();
  UInt_t *srcPix = src->GetArgbArray();
  UInt_t *tgtPix = tgt->GetArgbArray();

  // examples of pixel manipulations 
 
  
  double mindist=0;
  double colordist=0;
  double T;
  int flag = 0;
  int steps=1e6;

  for (int i=0;i< steps; i++){
    T = 5-double(i)/steps;
    double change = 0;    

    if (i==0){
      colordist = color_diff(outPix,tgtPix,first,second,minpic,numPix,i,flag);
      mindist = colordist;
      cout<<i<<" "<<mindist<<endl;
    }
    else{
      
      change = swap(first,second,numPix);
      colordist = mindist;
      colordist = colordist + change;       
      
    }
    
    if (colordist<mindist){
      flag=1;
      mindist = colordist;
      for (int i=0;i<numPix;i++){
        minpic[i].red = first[i].red;
        minpic[i].green = first[i].green;
        minpic[i].blue = first[i].blue;
      }
    }
    else if ((rand()%10000)/10000 + 0.1<exp(-(colordist-mindist)/T)){
      flag = 1;
      mindist = colordist;
      for (int i=0;i<numPix;i++){
        minpic[i].red = first[i].red;
        minpic[i].green = first[i].green;
        minpic[i].blue = first[i].blue;
      }
    }
    if (flag == 1){
      for (int i=0;i<numPix; i++){
        first[i].red = minpic[i].red;
        first[i].green = minpic[i].green;
        first[i].blue = minpic[i].blue; 
      }
    }
    if(i%100000 == 0){
      cout<< i<< " " << mindist <<"\n";
    }
  }
    
  
  unsigned a = 255;

  for (int i=0;i<numPix; i++){
    outPix[i] = (a<<24) | (minpic[i].red<<16) | (minpic[i].green<<8) | minpic[i].blue;
  }/*
  // *** The work happens here
  // access the pixels for the output image 
  // each pixel is a 32-bit word, 1 byte each for (alpha,red,green,blue)
  // don't touch alpha (bits 31:28)
  UInt_t *outPix = out->GetArgbArray();  
  UInt_t *outPix2= tgt->GetArgbArray();
  // examples of pixel manipulations 
  for (int i=0;i< numPix; i++){
    //outPix[i]&=0xff00ffff;  // turn off red
      //outPix[i]&=0xffff00ff;  // turn off green 4278221056
      //outPix[i]&=0xffffff00;  // turn off blue 4278190*167*
    //cout << hex << outPix[i]<<endl;  // print pixel values in hex
    //cout << outPix[i]<<endl;
      
  }
    
  unsigned pix,pix2;
  int red[numPix],green[numPix],blue[numPix];
  int red2[numPix],green2[numPix],blue2[numPix];

  for (int i=0;i< numPix; i++){
    //from first image//
    pix = outPix[i]; // some pixel
    blue[i] = pix & 0xff;
    green[i] = (pix & 0xff00) >> 8;
    red[i] = (pix & 0xff0000) >> 16;  
    //cout << red[i] << ',' << green[i] << ',' << blue[i] << endl;
    //for the target image
    pix2= outPix2[i];
    blue2[i] = pix2 & 0xff;
    green2[i] = (pix2 & 0xff00) >> 8;
    red2[i] = (pix2 & 0xff0000) >> 16; 
  }

  // flip the image
  for (int i=0;i< numPix-2; i++){//(int i=0;i< numPix/2; i++)
    unsigned pxl=outPix[i];
    //outPix[i]=outPix[numPix-i-1];
    //outPix[numPix-i-1]=pxl;
    outPix[i]=(outPix[i+1]-outPix[i+2]);
    //outPix[i]&=0xff00ffff;  // turn off red
    //outPix[i]&=0xffff00ff;  // turn off green 4278221056
    //outPix[i]&=0xffffff00;  // turn off blue 4278190*167*
  }
   */
  // *************************


  // print the results
  TCanvas *c1 = new TCanvas("c1", "images", 640, 480);
  c1->Divide(2,2);

  c1->cd(1);
  c1->Draw();
  src->Draw("X");
  c1->cd(2);
  tgt->Draw("X");
  c1->cd(3);
  out->Draw("X");
  c1->Print("collage.png");
  
  // save the new image
  out->WriteImage(fout.Data());

  // coment out the lines for running in batch mode
  cout << "Press ^c to exit" << endl;
  theApp.SetIdleTimer(300,".q");  // set up a failsafe timer to end the program  
  theApp.Run();
}
