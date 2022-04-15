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

using namespace std;




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
  



  //////////////////////////////////////////////////////////////////////////////////////////////


   int A=1;
   int B=1;
   int num=0;
   int i=0;


  while (i < n) {
 
   A=A+B;
   B=B+A;


    int k, n1, num = 0;

    k= A;

    n1 = k; //storing the original number

    //Logic to count the number of digits in a given number
    while (k != 0)
    {
        k /= 10; //to get the number except the last digit.
        num++; //when divided by 10, updated the count of the digits
    }


    std::cout<<A;
 cout<<"\n";
  std::cout<<B;
  cout<<"\n";
  
  i=num;
 


  }
 
  //    cout << "\n\nThe number of digits in the entered number: " << n1 << " is " << num;
  //   cout << "\n\n\n";






  ///////////////////////////////////////////////////////////////////////////////////////////////






  // print the answer, modify this code as needed
  int nfib=123;  // sequence of number of solution
  printf("F%d = %s",nfib,"Value (see note at top of source file)\n");
    
  return 0;
}
