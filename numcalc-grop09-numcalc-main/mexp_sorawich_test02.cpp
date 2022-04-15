// C++ implementation of the approach
#include <stdio.h>
// Recursive Function with static
// variables p and f

double e(int x, int n)
{
   static double p = 1, f = 1;
   double r;
   //Termination condition
   if (n == 0)
	return 1;
   // Recursive call
   r = e(x, n - 1);
   // Update the power of x
   p = p * x;
   // Factorial
   f = f * n;
   
   return (r + p / f);
}

// Driver code
int main()
{
     	int x = -20, n = 1000;

	printf("%lf \n", e(x, n));
	return 0;
}

