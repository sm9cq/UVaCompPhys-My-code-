
// Read in an integer (n) and print the 1st Fibonacci number with n digits
// The output of your program must follow the template below exactly
//
//    $ fib 2
//    F7 = 13
//    $ fib 5 
//    F21 = 10946
//    10946
//    $ fib 21
//    F98 = 135301852344706746049

#include <iostream>
#include <cstdio>
#include<math.h>
#include<bits/stdc++.h>
#include<string>

using namespace std;
//Define a str +str for large number

string SchoolSum(string str1,string str2)
{
  if (str1.length() > str2.length())
    {swap(str1,str2);}
  //define an empty string for assigning the value of each digit
  string str ="";
  int n1=str1.length(), n2=str2.length();
//sum digit by digit
  /*int n;
  if(n1>n2)
  {
     n=n2;
  }else{
     n=n1;
  }*/

  reverse(str1.begin(),str1.end());
  reverse(str2.begin(),str2.end());                     
  
  int carry=0;
  for(int i=0;i<n1;i++)
  {
  int sum = ((str1[i]-'0')+(str2[i]-'0')+carry);
  //string-'0' is a conversion from _char_ to _int_
  //focus on current digit+digit+carry(from previous sum)
  str.push_back(sum%10+'0');
  //(int+'0') convert int to str and then stack a digit back
  carry = sum/10;
  }
 //Add remaining digits of larger number
  for (int i=n1;i<n2;i++)
  {
  int sum = ((str2[i]-'0')+carry);
  str.push_back(sum%10 + '0');
  carry = sum/10;
  }
  //when the carry is remaining
  if (carry)
  {
  str.push_back(carry+'0');
  }
  /*if (carry>0 && n1 == n2)
  {
    str.push_back( 1 +'0');//carry+'0');
  }
  else if(carry ==0 && (n1 < n2))
  {
  carry=carry+int(str2[n]-'0');
  str.push_back(carry+'0');

  str.push_back(int(str2[n-1] - '0')); 
  }
  else if(carry>0 && (n1 < n2))
  {
  carry=carry+int(str2[n]-'0');
  str.push_back(carry+'0');
  }
  //else if(carry ==0 && (n1 ==n2)
  */
  //after we finish sum from ending -> we need to reverse the str to be in order
  reverse(str.begin(),str.end());
  return str;

}

int main(int argc, char* argv[]){
  if (argc<2){
    cout << "Usage: fib int" << endl;
    return 1;
  }
  int n=atoi(argv[1]);
  if (n<1){
    cout << "integer must be > 0" << endl;
    return 1;
  }
 
  /* 
     Your code here.  Feel free to use your own functions as necessary
  */
    string F1="1",F2="1";
  //  Fn = Fn−1 + Fn−2, where F1 = 1 and F2 = 1
  //while loop execution

    string Fn;
    int nfib=2;
  while( int (Fn.length()) < n)//( ( floor(log10(Fn))+1)< n)
  //for (int k=0; k<51;k++)
  {
  Fn=SchoolSum(F1,F2);//Fn = F1 + F2;
  F1=F2;
  F2=Fn;
  nfib++;
  //cout<<"F2"<<F2<<endl;
  //cout << "F" <<nfib<<"="<<  Fn ;
  //cout << " : number of digit:" << Fn.length()<<endl;// floor(log10(Fn))+1 << endl;
  //Fn=stoi(Fn);
   
  }
  //cout << Fn << ":number of digit :" << Fn.length() << endl;
  //The problem is we can get only up to 10
  //for more than 10 the Fn can't carry all that stuff
  //so we will do reverse to string and do a school sum like we were young
  //After this here will be the space of that school sum
 
  // print the answer, modify this code as needed
  //int nfib=123;  // sequence of number of solution
  //cout << "F" << nfib << "=" << Fn << endl;
  printf("F%d = %s\n",nfib,Fn.c_str());//"Value (see note at top of source file)\n");
    
  return 0;
}
























