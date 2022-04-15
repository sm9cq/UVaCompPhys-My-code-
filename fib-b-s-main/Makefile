all: fib xor

fib: fib.cpp
	g++ -O -Wall -o fib fib.cpp

xor: xor.cpp
	g++ -O -Wall -o xor xor.cpp

test_cpp: fib xor
	./fib 2021 > fib2021.txt
	./xor fib2021.txt key2021.txt

test_py:
	python fib.py 2021 > fib2021.txt
	./xor fib2021.txt key2021.txt

clean:
	rm -f fib xor
