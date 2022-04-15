# a very basic Makefie
all: nSphere plot

nSphere: nSphere.cpp
	g++ -O -Wall -onSphere nSphere.cpp

plot:
	./nSphere > nSphere.dat
	gnuplot nSphere.plt

clean:
	rm -f nSphere nSphere.dat nSphere.png *~
