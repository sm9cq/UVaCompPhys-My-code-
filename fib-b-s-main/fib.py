# Read in an integer (n) and print the 1st Fibonacci number with n digits
# The output of your program must follow the template below exactly
#
#    $ python fib.py 2
#    F7 = 13
#    $ python fib.py 5 
#    F21 = 10946
#    10946
#    $ python fib.py 21
#    F98 = 135301852344706746049
#
import sys

if len(sys.argv) != 2:
    print('Usage: python fib.py int')
    sys.exit(1)

n = int(sys.argv[1])
if n<1:
    print("integer must be > 0")
    sys.exit(1)





#########################################################################################
####
A=1
B=1

k=0
i=2
while k<n:


	Fib=A+B
	A=B
	B=Fib
	k=len(str(Fib))
	i=i+1



print("F"+str(i)+ " = "+str(Fib))


#########################################################################################




#nfib = 123  # sequence of number of solution
#result =  "Value (see note at top of source file)"
#print(f'F{nfib} = {result}')
