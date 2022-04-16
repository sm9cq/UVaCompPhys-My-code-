# very basic example of a Makefile
# xxx: These are targets, the first one is the default
#      Typing "make target" will execute specific build instructions 
#      Following the target are dependencies.  If the files designated
#      as dependencies have not changed since running the previous make,
#      then the code will not be rebuilt.  This is very useful if many
#      files need to be compiled for a large program.  Only updates and
#      files depending on them will be recompiled.
#    
#      Following the target line are lines beginning w/ a TAB character.
#      These lines define the build (or other instructions)

all: decay bessel

decay: decay.cpp
	g++ -O -Wall -odecay decay.cpp

bessel: bessel.cpp
	g++ -O -Wall -obessel bessel.cpp

# using make to automate some work we might do on the command line
runbessel:
	./bessel 0; mv -v bessel.dat bessel0.dat
	./bessel 2; mv -v bessel.dat bessel2.dat
	./bessel 5; mv -v bessel.dat bessel5.dat

clean:
	rm -f decay bessel *.dat *png *pdf *~

